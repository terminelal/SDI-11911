#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
cv::Mat imgOld;
cv::Mat img;
bool bol=false;
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    img =  cv_bridge::toCvShare(msg, "bgr8")->image;
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    bol =false;
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("/app/camera/rgb/image_raw", 1, imageCallback);
  while (ros::ok())
  {

  if (!bol)
  {
      bol =true;
      imgOld =img;
      //split image in layer (channels
    std::vector<cv::Mat> ch;
    cv::split(img,ch);
     
    //get image only the range 0 to 85
    cv::inRange(ch[2], cv::Scalar(0),cv::Scalar(80), ch[2]);
      cv::imshow("view", ch[2]);
    /* code */
  }
  }

  //cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
  //cv::waitKey(30);
  cv::destroyWindow("view");
}