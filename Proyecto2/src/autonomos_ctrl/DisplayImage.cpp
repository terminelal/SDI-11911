#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv/highgui.h>
#include <cv_bridge/cv_bridge.h>

void imageCallback(const sensor_msgs::ImageConstPtr& msg) 
{
  //cv::Mat im =  cv_bridge::toCvShare(msg, msg "bgr8")->image;
  //cv_bridge::toCvCopy(msg, "bgr8"); 
  //cv_bridge::CvImageConstPtr cv_ptr; 
  //printf("\n\n%s\n",msg->encoding.c_str() );
  //cv_bridge::CvImagePtr cv_ptr(new cv_bridge::CvImage);
  //cv_ptr->encoding = "bgr8";
  //cv_ptr->image = cv::imread("lena.jpg",CV_LOAD_IMAGE_COLOR);
   //cv::imshow("view",  cv::imread("lena.jpg",CV_LOAD_IMAGE_COLOR));
  //cv_bridge::CvImagePtr cv_ptr;
  try
  {
          //cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    //cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg.img, "bgr8");
    
    cv::waitKey(30);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
  /*
   try {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
        cv::imwrite("rgb.bmp", cv_ptr->image);
        cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
        cv::waitKey(30);
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.",
                msg->encoding.c_str());
    }
  */
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("camera/rgb/image_raw", 1, imageCallback);
  ros::spin();
  cv::destroyWindow("view");
}