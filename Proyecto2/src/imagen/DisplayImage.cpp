#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	try
	{
		cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
		// cv::Mat _img = cv_bridge::toCvShare(msg , "bgr8")->image;
		cv::waitKey(30);
	}
	catch (cv_bridge::Exception& e)
	{
		ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
	}
}

int main(int argc, char** argv )
{
	ros::init(argc, argv, "image_node");
  	ros::NodeHandle nh;
  	ros::Rate loop_rate(1);

	cv::namedWindow("view");
	cv::startWindowThread();
	
	ros::Subscriber sub = nh.subscribe<sensor_msgs::Image>("/app/camera/rgb/image_raw", 1, imageCallback);
	ros::spin();
	cv::destroyWindow("view");

}
