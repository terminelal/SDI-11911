#include <ros/ros.h>
<<<<<<< HEAD
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

#include <gazebo_msgs/GetJointProperties.h>
using namespace Eigen;

//For geometry_msgs::Twist using:
// 		dummy.linear.x
// 		dummy.linear.y
// 		dummy.angular.z
geometry_msgs::Twist robot_position;
geometry_msgs::Twist velocity_msg;

//rate_hz assignment
double rate_hz = 1;

// Transform robot velocities (x,y,w) to motor velocities (m1,m2,m3,m4)
// Bicycle model, using x,y,0
double* getMotorValue(double x_velocity, double y_velocity, double w_velocity){
	double velMots[2];
	double dblL = 32.0;
	double r = 2.0;
	double R = dblL/tan(w_velocity);
	
	double deg1 = 3 * M_PI / 10;
	double deg2 = 3 * M_PI / 10;
	double s1 = sin(deg1), c1 = cos(deg1);
	double s2 = sin(deg2), c2 = cos(deg2);
	

	double velXMod = x_velocity;//	x_velocity * 3.5 / ( 2 * M_PI * r );
    double velYMod = x_velocity;// * 3.5 / ( 2 * M_PI * r );
    //double velWMod =atan (w_velocity)	;; //(x_velocity/dblL)*//w_velocity * 3.5 / ( 2 * M_PI * r );
    //velWMod = R * velWMod;
    
    velMots[0] = x_velocity;//(double)( s1 * velXMod + c1 * velYMod + velWMod);
    //velMots[1] = x_velocity*.01;//(double)( s1 * velXMod + c1 * velYMod + velWMod);
	double toRadianas ;
	
	velMots[1] = w_velocity;;
    return velMots;
}

void get_vel_vec(const geometry_msgs::Twist& msg) {
	velocity_msg.linear.x = msg.linear.x;
	velocity_msg.linear.y = msg.linear.y;
    velocity_msg.angular.z = msg.linear.z; 
}


int main(int argc, char **argv){
	ros::init(argc,argv,"auto_velocity_node");
	ros::NodeHandle nh;
	ROS_INFO_STREAM("auto_velocity_node initialized");
	ROS_INFO_STREAM(ros::this_node::getName());
	
	// Suscribe to Gazebo service ApplyJointEffor
	ros::ServiceClient client = nh.serviceClient<gazebo_msgs::ApplyJointEffort>("/gazebo/apply_joint_effort");
	//ros::ServiceClient client1 = nh.serviceClient<gazebo_msgs::GetJointProperties>("/gazebo/apply_joint_properties");
	gazebo_msgs::ApplyJointEffort eff_msg[4];
	ros::ServiceClient get_jnt_state_client =  nh.serviceClient<gazebo_msgs::GetJointProperties>("/gazebo/get_joint_properties");

	gazebo_msgs::GetJointProperties get_joint_state_srv_msg;
 	get_joint_state_srv_msg.request.joint_name = "steer_joint";
	

	ros::Subscriber sub_vel = nh.subscribe("/target_vel_topic", 1000, &get_vel_vec);

	double tiempo = 0;

    //define the max speed
	double cruise_speed = 50;

    //define the rate
	ros::Rate rate(rate_hz);
	ros::Time start_time ;
	ros::Duration duration ;

	double effort[2];
	
	
	while (ros::ok())
	{

		double* velMots = getMotorValue(velocity_msg.linear.x,velocity_msg.linear.y,velocity_msg.angular.z);
	
	effort [0] =velMots[0];
	//effort [1] =velMots[0];
	effort [1] =velMots [1];
		if(client.exists()){
			start_time.sec = 0;
			start_time.nsec = 0;
			duration.sec = 1/rate_hz;
			duration.nsec = 0;

			get_jnt_state_client.call(get_joint_state_srv_msg);
			double tst = get_joint_state_srv_msg.response.position[0];
			
			printf ("effort \t(%f, %f, %f) \n",effort[0],effort[1],tst);
			//get angle
			//eff_msg1[0].request.joint_name = "steer_joint";
			//double tst = eff_msg1[0].request.position;
			//printf("%f\n", tst);
			// Wheel-Joint 1
			eff_msg[0].request.joint_name = "back_left_wheel_joint";
			eff_msg[0].request.duration = duration;
			eff_msg[0].request.effort =effort [0];
			eff_msg[0].request.start_time = start_time;

			// Wheel-Joint 2
			eff_msg[1].request.joint_name = "back_right_wheel_joint";
			eff_msg[1].request.duration = duration;
			eff_msg[1].request.effort = effort [0];
			eff_msg[1].request.start_time = start_time;

			// Wheel-Joint 3
			eff_msg[2].request.joint_name = "steer_joint";
			eff_msg[2].request.duration = duration;
			eff_msg[2].request.effort = .5*(effort[1]-tst);//falta agregarle el error para que sea i ahorita es pd
			eff_msg[2].request.start_time = start_time;

			client.call(eff_msg[0]);
			client.call(eff_msg[1]);																																																																								
			client.call(eff_msg[2]);
			ROS_INFO_STREAM("Joints ==> 1: " << ((eff_msg[0].response.success == 1) ? "TRUE" : "FALSE") <<
			" 2: " << ((eff_msg[1].response.success == 1) ? "TRUE" : "FALSE") <<
			" 3: " << ((eff_msg[2].response.success == 1) ? "TRUE" : "FALSE") <<
			" 4: " << ((eff_msg[3].response.success == 1) ? "TRUE" : "FALSE"));
		}																																																																																																
		
		ros::spinOnce();
		rate.sleep();
    }
    return 0;
}
=======
#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>

class My_Filter {
     public:
        My_Filter();
        void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
     private:
        ros::NodeHandle node_;
        laser_geometry::LaserProjection projector_;
        tf::TransformListener tfListener_;

        ros::Publisher point_cloud_publisher_;
        ros::Subscriber scan_sub_;
};

My_Filter::My_Filter(){
        scan_sub_ = node_.subscribe<sensor_msgs::LaserScan> ("/scan", 100, &My_Filter::scanCallback, this);
        point_cloud_publisher_ = node_.advertise<sensor_msgs::PointCloud2> ("/cloud", 100, false);
        tfListener_.setExtrapolationLimit(ros::Duration(0.1));
}
void My_Filter::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan){
    sensor_msgs::PointCloud2 cloud;
    projector_.transformLaserScanToPointCloud("base_link", *scan, cloud, tfListener_);
    point_cloud_publisher_.publish(cloud);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "my_filter");

    My_Filter filter;

    ros::spin();

    return 0;
}
>>>>>>> 51c63b4f0d4ab003fd97cdf4915dede5beeaa748
