#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <iostream>
#include <string> 
#include <stdio.h>
#include <std_msgs/Int16.h>

#define PI 3.14159265

//using namespace Eigen;

//For geometry_msgs::Twist using:
// 		dummy.linear.x
// 		dummy.linear.y
// 		dummy.angular.z
geometry_msgs::Twist robot_position;
geometry_msgs::Twist velocity_msg;
geometry_msgs::Pose2D vel;

geometry_msgs::Twist target_position;
ros::ServiceClient clientSteer;

//rate_hz assignment
double rate_hz = 5;


void get_vel_vec(const geometry_msgs::Pose2D& msg) {
	vel.x = msg.x;
	vel.y = msg.y;
	vel.theta = msg.theta; 
}

void getTargetPose(const geometry_msgs::Twist& msg) {
    target_position.linear.x = msg.linear.x;
    target_position.linear.y = msg.linear.y;
    target_position.angular.z = msg.angular.z; 
}


int main(int argc, char **argv){
	ros::init(argc,argv,"auto_velocity_node");
	ros::NodeHandle nh;
	ROS_INFO_STREAM("auto_velocity_node initialized");
	ROS_INFO_STREAM(ros::this_node::getName());



	ros::Subscriber sub_ball_pos = nh.subscribe("/target_pose", 1, &getTargetPose);

	ros::Publisher pub = nh.advertise<std_msgs::Int16>(nh.resolveName("/manual_control/speed"), 1);
	ros::Publisher pub_ste = nh.advertise<std_msgs::Int16>(nh.resolveName("/manual_control/steering"), 1);
	
	// double tiempo = 0;
	double cruise_speed = 50;	
	
	double vel;
	const std::string PARAM1 = "~vel";
	bool okx = ros::param::get(PARAM1, vel);
	if(!okx) {
		ROS_FATAL_STREAM("No se pudo obtener el parametro " << PARAM1);
		exit(1);
	}
	else{
		std::cout<<"\nVelocidad: "<<vel;
		cruise_speed = vel;
	}

	//define the rate
	ros::Rate rate(rate_hz);
	ros::Time start_time ;
	ros::Duration duration ;

	
	while (ros::ok())
	{
		std_msgs::Int16 value_ste;
		double distancia = sqrt(pow(target_position.linear.x,2) + pow(target_position.linear.y,2));
		double volante = 0.0;
		printf("\n distancia: %f", distancia);
		if(distancia > 0){
			
			volante=target_position.angular.z;
			//else
			//	volante=-(noventa-alpha); // derecha es steer joint negativo

			printf("\nvolante: %f, objetivo: (%f, %f)", volante, target_position.linear.x, target_position.linear.y);
			
			if(volante < 45)
				volante = 45;
			if(volante > 170)
				volante = 170;
			
			value_ste.data = volante;
			
		} else {
			distancia=0.0;
		}
		
		std_msgs::Int16 value_motor;
		
		if(distancia>0)
			value_motor.data = cruise_speed;
		else
			value_motor.data = 0;

		value_ste.data = volante;
		
		pub.publish(value_motor); 
		pub_ste.publish(value_ste); 
 
		ROS_INFO_STREAM("\nvals: (" << value_motor.data << " , " << value_ste.data << " )");	
																																																																																									
		
		ros::spinOnce();
		rate.sleep();
    }
    return 0;
}
