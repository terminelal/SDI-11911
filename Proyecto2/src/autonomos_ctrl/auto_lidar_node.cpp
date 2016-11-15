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
//#include <turtlesim/Pose.h>
#include <Eigen/Geometry>
#include "gazebo_msgs/ApplyJointEffort.h"
#include <math.h>
#include <sensor_msgs/LaserScan.h>
#include <gazebo_msgs/GetJointProperties.h>
using namespace Eigen;


void processLaserScan(const sensor_msgs::LaserScan::ConstPtr& scan){
    // scan->ranges[] are laser readings
    printf("soy yo\n");
	printf("%f\n", scan->angle_max );
   
}
int main(int argc, char **argv){
	ros::init(argc,argv,"auto_lidar_node");
	ros::NodeHandle nh;
	ROS_INFO_STREAM("auto_lida_node initialized");
	ROS_INFO_STREAM(ros::this_node::getName());
	
	ros::Subscriber scanSub;
	scanSub=nh.subscribe<sensor_msgs::LaserScan>("/laser_scan",10,&processLaserScan);
	while (ros::ok())
	{
			//processLaserScan();

		ros::spinOnce();
		//rate.sleep();
		}
    return 0;
}

