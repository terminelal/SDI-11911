#include <ros/ros.h>
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
#include <stdlib.h>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/QR>
#include <Eigen/LU>
#include <math.h>
#include <stdio.h>
#include <Eigen/Geometry>
#include "gazebo_msgs/ApplyJointEffort.h"
#include <math.h>

using namespace Eigen;

geometry_msgs::Twist robot_position;
geometry_msgs::Twist velocity;

//rate_hz assignment
double rate_hz = 5;

double velMots[2];

// Transform robot velocities (x,y,w) to motor velocities (m1,m2,r1)
double* getMotorValue(int x_velocity, int y_velocity, int w_velocity){

    //Asignamos un valor arbitrario a la velocidad de las llantas.
	velMots[0] = x_velocity;
	//El ángulo del volante sí se actualiza.
	velMots[1] = (w_velocity*0.69);

    return velMots;
}

void get_vel_vec(const geometry_msgs::Twist& msg) {
	velocity.linear.x = msg.linear.x;
	velocity.linear.y = msg.linear.y;
    velocity.angular.z = msg.linear.z; 
}


int main(int argc, char **argv){
	ros::init(argc,argv,"a_robot_velocity_node");
	ros::NodeHandle nh;
	ROS_INFO_STREAM("a_robot_velocity_node initialized");
	ROS_INFO_STREAM(ros::this_node::getName());	

	
	// Suscribe to Gazebo service ApplyJointEffor
    //Definimos el arreglo de eff_msg para 3 joints.
	ros::ServiceClient client = nh.serviceClient<gazebo_msgs::ApplyJointEffort>("/gazebo/apply_joint_effort");
	gazebo_msgs::ApplyJointEffort eff_msg[3];

    //Se suscribe al tópico que entrega la velocidad.
	ros::Subscriber sub_vel = nh.subscribe("/target_vel_topic", 1000, &get_vel_vec);


	double tiempo = 0;

	//define the rate
	ros::Rate rate(rate_hz);
	ros::Time start_time ;
	ros::Duration duration ;


	double effort[3];
	
	while (ros::ok())
	{
	double* velMots = getMotorValue(velocity.linear.x,velocity.linear.y,velocity.angular.z);
	
	effort [0] = velMots[0];
	effort [1] = velMots[0];
	effort [2] = velMots[1];	
  
		if(client.exists()){
			
			start_time.sec = 0;
			start_time.nsec = 0;
			duration.sec = 1/rate_hz;
			duration.nsec = 0;

			// Wheel-Joint 1
			eff_msg[0].request.joint_name = "back_left_wheel_joint";
			eff_msg[0].request.duration = duration;
			eff_msg[0].request.effort = effort[0];
			eff_msg[0].request.start_time = start_time;

			// Wheel-Joint 2
			eff_msg[1].request.joint_name = "back_right_wheel_joint";
			eff_msg[1].request.duration = duration;
			eff_msg[1].request.effort = effort[1];
			eff_msg[1].request.start_time = start_time;

			// Angle-Joint
			eff_msg[2].request.joint_name = "steer_joint";
			eff_msg[2].request.duration = duration;
			eff_msg[2].request.effort = effort[2];
			eff_msg[2].request.start_time = start_time;

			client.call(eff_msg[0]);
			client.call(eff_msg[1]);																																																																								
			client.call(eff_msg[2]);
			ROS_INFO_STREAM("Joints ==> 1: " << ((eff_msg[0].response.success == 1) ? "TRUE" : "FALSE") <<
			" 2: " << ((eff_msg[0].response.success == 1) ? "TRUE" : "FALSE") <<
			" 3: " << ((eff_msg[1].response.success == 1) ? "TRUE" : "FALSE") <<
			" 4: " << ((eff_msg[2].response.success == 1) ? "TRUE" : "FALSE"));
		}																																																																																																
		
		ros::spinOnce();
		rate.sleep();
    }
    return 0;
}
