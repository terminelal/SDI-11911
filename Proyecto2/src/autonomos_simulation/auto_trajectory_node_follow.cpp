#include <ros/ros.h>
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <stdlib.h>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/QR>
#include <Eigen/LU>
#include <math.h>
#include <stdio.h>
//#include <turtlesim/Pose.h>
#include <Eigen/Geometry>
#include "gazebo_msgs/LinkStates.h"
// #include <LinkStates.h>

#include <gazebo_msgs/GetJointProperties.h>

using namespace Eigen;

//For geometry_msgs::Twist using:
// 		dummy.linear.x
// 		dummy.linear.y
// 		dummy.angular.z
geometry_msgs::Pose rposition;
geometry_msgs::Pose rpositionOld;
geometry_msgs::Twist target_position;
geometry_msgs::Twist tposition_old;

//rate_hz assignment
double rate_hz = 5;

//Assign the position of the robot (from other topic) to robot_position.
//Assuming the topic that generate the robot position uses geometry_msgs::Twist
//and the information is in *.linear. This might need to be modifed
void getRobotPose(const gazebo_msgs::LinkStates& msg) {
    // msg.name[1]
    rposition.position.x = msg.pose[1].position.x;
    rposition.position.y = msg.pose[1].position.y;
    // rposition.orientation.x = msg.pose[1].orientation.x; 
    // rposition.orientation.y = msg.pose[1].orientation.y; 
    // rposition.orientation.z = msg.pose[1].orientation.z; 
    // rposition.orientation.w = msg.pose[1].orientation.w; 

    double dblVel = sqrt(pow(rpositionOld.position.x - rposition.position.x,2) + pow   (rpositionOld.position.y-rposition.position.y,2))/(1/rate_hz);
    printf("\nVelocity: %f \n", dblVel);

    rpositionOld.position.x    = rposition.position.x;
    rpositionOld.position.y    = rposition.position.y;
}

//Assign the position of the target (from other topic) to target_position.
//Assuming the topic that generate the robot position uses geometry_msgs::Twist
//and the information is in *.linear. This might need to be modifed
void getTargetPose(const geometry_msgs::Twist& msg) {

    tposition_old = target_position;


    target_position.linear.x = msg.linear.x;
    target_position.linear.y = msg.linear.y;
    target_position.angular.z = msg.angular.z; 
}

//Function to generate a Linear Constant Velocity from robot's position to the target's position
geometry_msgs::Twist generateConstantVelocity(double constant_speed, geometry_msgs::Twist p_start, geometry_msgs::Twist p_goal){

    // Compute direction to goal
    Vector3d p_start_vector(p_start.linear.x,p_start.linear.y,p_start.angular.z);
    Vector3d p_goal_vector(p_goal.linear.x, p_goal.linear.y, p_goal.angular.z);
    // Vector3d goal_direction_vector = p_goal_vector-p_start_vector;
    Vector3d goal_direction_vector = p_start_vector - p_goal_vector;

    // Compute speed in the direction to goal
    Vector3d velocity_vector = constant_speed * (goal_direction_vector/ goal_direction_vector.norm());

    geometry_msgs::Twist velocity;
    velocity.linear.x = velocity_vector.x();
    velocity.linear.y = velocity_vector.y();
    velocity.angular.z = velocity_vector.z();

    return velocity;
}

// Function to keep velocity under the allowed robot limits
geometry_msgs::Twist boundVelocity(geometry_msgs::Twist velocity) {
	double max_linear_speed = 30;
	double min_linear_speed = 0;
	double max_angular_speed = M_PI*4;
	double min_angular_speed = M_PI/16;

	if (velocity.linear.x > max_linear_speed)
		velocity.linear.x = max_linear_speed;
	else if (velocity.linear.x < -max_linear_speed)
		velocity.linear.x = -max_linear_speed;
	if (velocity.linear.y > max_linear_speed)
		velocity.linear.y = max_linear_speed;
	else if (velocity.linear.y < -max_linear_speed)
		velocity.linear.y = -max_linear_speed;
	if (velocity.angular.z > max_angular_speed)
		velocity.angular.z = max_angular_speed;
	else if (velocity.angular.z < -max_angular_speed)
		velocity.angular.z = -max_angular_speed;

    // Lower speed bounds
	if (velocity.linear.x > 0 && velocity.linear.x < min_linear_speed)
		velocity.linear.x = min_linear_speed;
	else if (velocity.linear.x < 0 && velocity.linear.x > -min_linear_speed)
		velocity.linear.x = -min_linear_speed;
	if (velocity.linear.y > 0 && velocity.linear.y<min_linear_speed)
		velocity.linear.y = min_linear_speed;
	else if (velocity.linear.y < 0 && velocity.linear.y > -min_linear_speed)
		velocity.linear.y = -min_linear_speed;
	if (velocity.angular.z >0 && velocity.angular.z < min_angular_speed)
		velocity.angular.z = min_angular_speed;
	else if (velocity.angular.z < 0 && velocity.linear.z > - min_angular_speed )
		velocity.angular.z = -min_angular_speed;
}


int main(int argc, char **argv){
	ros::init(argc,argv,"turtle_trajectory_node_follow");
	ros::NodeHandle nh;
	ROS_INFO_STREAM("turtle_trajectory_node initialized");
	ROS_INFO_STREAM(ros::this_node::getName());
	
	//Topic to publish velocity command, queue size equals rate_hz to keep up with the rate at which messages are generated,

    //Publish to the turtle topic "/turtle1/cmd_vel at rate_hz"
	ros::Publisher pub_vel_turtle = nh.advertise<geometry_msgs::Twist>("/target_lidar_topic", rate_hz);

	//Topics to acquire robot position 
	ros::Subscriber sub_robot_pos = nh.subscribe("/gazebo/model_states", 1, &getRobotPose);
	//Topics to acquire target position (from the vision node) 
	ros::Subscriber sub_ball_pos = nh.subscribe("/target_lidar", 1, &getTargetPose);

	

    //Twist variable to publish velocity (trajectories)
	geometry_msgs::Twist desired_velocity;
	double tiempo = 0;

    //define the max speed
	double cruise_speed = 30;

    //define the rate
	ros::Rate rate(rate_hz);

	while (ros::ok())
	{
		ROS_INFO_STREAM("Target position lidar:"
			<<" X="<<target_position.linear.x
			<<",Y="<<target_position.linear.y
			<<",W="<<target_position.angular.z);

		if(tposition_old.linear.x != 0)
		{
		
			double distancia = sqrt(pow(target_position.linear.x-tposition_old.linear.x,2) + pow(target_position.linear.y-tposition_old.linear.y,2))/1/rate_hz;
			if(distancia > 0){
				double Kp = 0.2; // constante velocidad+
				double Ka = 0.5; // constante angulo
				double Kb = 0.5; // constante angulo
				double alpha = atan2(target_position.linear.y, target_position.linear.x); // invertidos por modelo deberia ser (y,x)
				double noventa = 1.57; // 90 grados
				double volante=0.0;
				if(alpha>noventa)
					volante=alpha-noventa;
				else
					volante=-(noventa-alpha); // derecha es steer joint negativo

				printf("\noriginal: %f, modificacion: %f, (%f, %f)", alpha, volante, target_position.linear.x, target_position.linear.y);

				if(volante > 0.5)
					volante = 0.5;
				if(volante < -0.5)
					volante = -0.5;

			
						
			
			
				desired_velocity.linear.x = distancia;
				desired_velocity.linear.y = 0; // dist
				desired_velocity.angular.z = volante;
			} else {
		    // Goal has been reach ==> dont move
				desired_velocity.linear.x = 0;
				desired_velocity.linear.y = 0;
				desired_velocity.angular.z = 0;
			}
			//ROS_INFO_STREAM use for debugging 
			ROS_INFO_STREAM("Desired Velocity:"
				<<"X:"<<desired_velocity.linear.x
				<<",Y:"<<desired_velocity.linear.y
				<<",W:"<<desired_velocity.angular.z);
			//publish the new velocity
			pub_vel_turtle.publish(desired_velocity);
		}
		ros::spinOnce();
		rate.sleep();
        tiempo+=(1/rate_hz); 
    }
    return 0;
}

