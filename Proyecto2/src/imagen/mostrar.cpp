#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <geometry_msgs/Twist.h>
#include <sstream>
#include <string>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	cv::namedWindow("Mostrar imagen" );
	cv::Mat src = cv_bridge::toCvShare(msg , "bgr8")->image;
  	cv::imshow( "Mostrar imagen", src);	
}




int main(int argc, char** argv )
{
	ros::init(argc, argv, "mostrar_node");
  	ros::NodeHandle nh;
  	ros::Rate loop_rate(1);

	cv::startWindowThread();
	ros::Subscriber sub = nh.subscribe<sensor_msgs::Image>("/target_image", 10, imageCallback);
	
	ros::spin();
	cv::destroyWindow("view");

}
