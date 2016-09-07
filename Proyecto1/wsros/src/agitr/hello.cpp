// ROS version of the standard "hello, world" program

// header that defines the standard ROS classes
#include <ros/ros.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	//initialize the ROS system
	ros::init(argc, argv, "hello_ros");
	
	//establish this program as a ROS node
	ros::NodeHandle nh;
	
	const std::string PARAM1 = "~x_fin";
	double x_fin;
	
	bool ok = ros::param::get(PARAM1, x_fin);
	if(!ok) {
		ROS_FATAL_STREAM("No se pudo obtener el parametro " << PARAM1);
		exit(1);
	}
	
	//send some output as a log message
	ROS_INFO_STREAM("Hello, ROS!, x_fin: " << x_fin);
}
