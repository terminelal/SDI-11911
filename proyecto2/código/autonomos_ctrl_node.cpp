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
#include <Eigen/Geometry>
#include "gazebo_msgs/LinkStates.h"

//Diego Fernando Cifuentes Jiménez, Enrique José Padilla Terrones, Alejandro Terminel Lembert
//Robótica 2016, ITAM

using namespace Eigen;

geometry_msgs::Twist robot_position;
geometry_msgs::Twist target_position; 

//Asignamos un rate en Hz.
double rate_hz = 5; 

//Método que obtiene la posición del robot a través de gazebo.
//Agregamos la orientación para conseguir alinear el robot.
void getRobotPose(const gazebo_msgs::LinkStates& msg) {
	robot_position.linear.x = msg.pose[1].position.x;
	robot_position.linear.y = msg.pose[1].position.y;
	robot_position.angular.z = 0;
}

//Método que obtiene la posición objetivo.
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

//Función que genera una velocidad constante entre la pose actual y la pose deseada.
geometry_msgs::Twist generateConstantVelocity(double constant_speed, geometry_msgs::Twist p_start, geometry_msgs::Twist p_goal){

	geometry_msgs::Twist velocity;

    //Agregamos el modelo cinemático del robot.
	double alfa = atan2(p_goal.linear.y, p_goal.linear.x);
	double steer = alfa; 
	double norma;

	//Es necesario limitar el ángulo que puede girar el volante.
	//Limitaciones del modelo del robot.
	if(steer > 0.5)
		steer = 0.5;
	if(steer < -0.5)
		steer = -0.5;

	norma = sqrt(pow(p_goal.linear.x, 2) + pow(p_goal.linear.y, 2))*constant_speed;

	velocity.linear.x = norma;
	velocity.linear.y = norma;
	velocity.angular.z = steer;

	return velocity;
}

// Función que mantiene al robot dentro de los límites de velocidad.
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
	ros::init(argc, argv, "autonomos_ctrl_node");
	ros::NodeHandle nh;
	ROS_INFO_STREAM("autonomos_ctrl_node initialized");
	ROS_INFO_STREAM(ros::this_node::getName());

	//Tópico de publicación de velocidad. Publicamos en "/turtle/cmd_vel".

	ros::Publisher pub_vel_turtle = nh.advertise<geometry_msgs::Twist>("/target_vel_topic", rate_hz);

	//Nos suscribimos a tópicos para conocer la posición actual y deseada del robot, target_pose se obtiene del nodo de visión.

	ros::Subscriber sub_robot_pos = nh.subscribe("/gazebo/model_states",1, &getRobotPose);
	ros::Subscriber sub_ball_pos = nh.subscribe("/target_position_topic", 1, &getTargetPose);

	//geometry_msgs::Twist estimated_velocity;
	geometry_msgs::Twist desired_velocity;

	double time = 0;
	
	double cruise_speed = 0.1;
	//Asignamos el rate.
	ros::Rate rate(rate_hz);

	while(ros::ok){
	
	ROS_INFO_STREAM("Actual position:" <<" X="<<robot_position.linear.x<<", Y="<<robot_position.linear.y<<", W="<<robot_position.angular.z);
	
	
	ROS_INFO_STREAM("Target position:" <<" X="<<target_position.linear.x<<", Y="<<target_position.linear.y<<", W="<<target_position.angular.z);

	if(isGoalFar(robot_position, target_position)){
		desired_velocity = generateConstantVelocity(cruise_speed, robot_position, target_position);
		desired_velocity= boundVelocity(desired_velocity);
	}
	else{
		desired_velocity.linear.x = 0;
		desired_velocity.linear.y = 0;
		desired_velocity.angular.z = 0;
		
	}
	//ROS_INFO_STREAM used for debugging
	ROS_INFO_STREAM("Desired Velocity: "
		<<"X: "<< desired_velocity.linear.x
		<<",Y:  "<< desired_velocity.linear.y
		<<",W : "<< desired_velocity.linear.z);
	//publica la velocidad.
	pub_vel_turtle.publish(desired_velocity);
		
	ros::spinOnce();
	rate.sleep();
        time+=(1/rate_hz); 
    }
    return 0;
}








