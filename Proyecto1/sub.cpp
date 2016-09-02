#include <ros/ros.h>
#include <turtlesim/Pose.h>
#include <iomanip>

void poseMessageRecieved(const turtlesim::Pose& msg){
	ROS_INFO_STREAM(std::setprecision(2) << std::fixed <<"Posicion=" << msg.x <<"," << msg.y << "(" << "direccion=" << msg.theta);
}

int main(int argc, char **argv){
	ros::init(argc, argv, "suscribe_to_pose");
	ros::NodeHandle nh;
	ros::Subscriber sub=nh.subscribe("turtle1/pose", 1000, &poseMessageRecieved);
	ros::spin();
}

