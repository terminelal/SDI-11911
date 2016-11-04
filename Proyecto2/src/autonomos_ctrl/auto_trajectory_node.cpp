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
geometry_msgs::Twist rposition;
geometry_msgs::Twist target_position;

//rate_hz assignment
double rate_hz = 5;

//Assign the position of the robot (from other topic) to robot_position.
//Assuming the topic that generate the robot position uses geometry_msgs::Twist
//and the information is in *.linear. This might need to be modifed
/*void getRobotPose(const gazebo_msgs::LinkStates& msg) {

	// msg.name[1]
	rposition.position.x = msg.pose[1].position.x;
    rposition.position.y = msg.pose[1].position.y;
    rposition.orientation.x = msg.pose[1].orientation.x; 
    rposition.orientation.y = msg.pose[1].orientation.y; 
    rposition.orientation.z = msg.pose[1].orientation.z; 
    rposition.orientation.w = msg.pose[1].orientation.w; 
}*/

//Assign the position of the target (from other topic) to target_position.
//Assuming the topic that generate the robot position uses geometry_msgs::Twist
//and the information is in *.linear. This might need to be modifed
void getTargetPose(const geometry_msgs::Twist& msg) {
	target_position.linear.x = msg.linear.x;
	target_position.linear.y = msg.linear.y;
    target_position.angular.z = msg.angular.z; 
}

//Function to determine if the goal is still far.
//Variables epsilon_x and epsilon_y used to determined if the goal has been reach
bool isGoalFar(geometry_msgs::Twist p_start, geometry_msgs::Twist p_goal) {
	double epsilon_x, epsilon_y;
	epsilon_x = .2; 
	epsilon_y = .2;
	double distance_x = abs(p_goal.linear.x - p_start.linear.x);
	double distance_y = abs(p_goal.linear.y - p_start.linear.y);
	if (distance_x > epsilon_x || distance_y > epsilon_y)
		return true;
	else
		return false;
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
	ros::init(argc,argv,"turtle_trajectory_node");
	ros::NodeHandle nh;
	ROS_INFO_STREAM("turtle_trajectory_node initialized");
	ROS_INFO_STREAM(ros::this_node::getName());
	 //Publish to the turtle topic "/turtle1/cmd_vel at rate_hz"
	ros::Publisher pub_vel_turtle = nh.advertise<geometry_msgs::Twist>("/target_vel_topic", rate_hz);
	//Twist variable to publish velocity (trajectories)
	geometry_msgs::Twist desired_velocity;
	double tiempo = 0;

    //define the max speed
	double cruise_speed = 30;

    //define the rate
	ros::Rate rate(rate_hz);
	int counter=0;
	int speed=8;
	while (ros::ok()) { //ESTACIONAMIENTO DEL LADO DERECHO
		if(counter < 30) { //AVANZA UNOS METROS
			ROS_INFO_STREAM("FORWARD");
			desired_velocity.linear.x = speed;
			desired_velocity.linear.y = 0; 
			desired_velocity.angular.z = 0;
		}else{
			if(counter < 65){ //COMIENZA A DETENERTE Y HACERTE PARA ATRAS
				ROS_INFO_STREAM("STOPPING AND BACKWARDS");
				desired_velocity.linear.x = speed>=-26?speed:0;
				desired_velocity.linear.y = 0;
				desired_velocity.angular.z = 0;
				speed--;
			}
			else{
			    if(counter <= 100) {
			    	ROS_INFO_STREAM("WHEELS RIGHT");
			    	desired_velocity.linear.x = 0;
					desired_velocity.linear.y = 0;
					desired_velocity.angular.z = -0.32;
				}
				else{
					if(counter <= 150) {
						if(counter <= 150) ROS_INFO_STREAM("WHEELS LEFT");
			    		desired_velocity.linear.x = 0;
						desired_velocity.linear.y = 0;
						desired_velocity.angular.z = 0.32;
					}
				}
			}
		}
/*else{
			if(counter < 40){
				ROS_INFO_STREAM("back");
				desired_velocity.linear.x = -5;
				desired_velocity.linear.y = 0; // dist
				desired_velocity.angular.z = 0;
			}else{
					ROS_INFO_STREAM("STAHP");
					desired_velocity.linear.x = 0;
					desired_velocity.linear.y = 0; // dist
					desired_velocity.angular.z = 0;
			}
		}else{
			//STAHP //DETENTE
			if(counter < 80 || counter > 250) { //DETENERSE EN EL MOMENTO DE VER EL ESPACIO Y ESTAR PARALELO AL SEGUNDO AUTO (80) Y DETENERESE AL FINAL TOTALMENTE (>250) 
				ROS_INFO_STREAM("STOP!");
				desired_velocity.linear.x = 0;
				desired_velocity.linear.y = 0; // dist
				desired_velocity.angular.z = 0;
			}else{
				if(counter < 160 ){	//ESTE MOMENTO SE ESTA METIENDO EN EL ESPACIO
					ROS_INFO_STREAM("RIGHT AND BACKWARD!");
					desired_velocity.linear.x = -5; //EN REVERSA SEGúN
					desired_velocity.linear.y = 0; // dist
					desired_velocity.angular.z = 0;//TODO: Modificar el angulo para q las ruedas apunten a la derecha max
				}
				else{
					if(counter < 240 ){ //SE ALINEA EN EL ESPACIO
						ROS_INFO_STREAM("LEFT AND BACKWARD!");
						desired_velocity.linear.x = -5; //EN REVERSA SEGúN
						desired_velocity.linear.y = 0; // dist
						desired_velocity.angular.z = 0;// TODO: Ahora modificar el angulo para que las ruedas apunten hacia la izquierda Max
					}
				}
			}
			
		}*/
		
		if(counter <= 160) ROS_INFO_STREAM("counter "<<counter);
		//publish the new velocity
		if(counter <= 160) pub_vel_turtle.publish(desired_velocity);
		counter++;
		ros::spinOnce();
		rate.sleep();
        tiempo+=(1/rate_hz); 
	}
	return 0;
}

