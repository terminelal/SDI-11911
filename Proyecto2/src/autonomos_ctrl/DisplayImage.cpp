#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
cv::Mat imgOld;
cv::Mat img;
bool bol=false;
int i=0;
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    img =  cv_bridge::toCvShare(msg, "bgr8")->image;
        std::vector<cv::Mat> ch;
    
    cv::split(img,ch);
     
    //get image only the range 0 to 85
    cv::inRange(img,cv::Scalar(100,100, 100), cv::Scalar(255, 255, 255), img);
    
    //cv::inRange(ch[1],cv::Scalar(0, 100, 0), cv::Scalar(255, 200, 255), ch[1]);
    
    //cv::inRange(ch[0],cv::Scalar(0, 100, 0), cv::Scalar(255, 200, 255), ch[0]);
    

    //cv::merge(ch,img);
    cv::threshold(img,img, 101, 255, CV_THRESH_BINARY);
      cv::imshow("view", img);

       char str[10];

      sprintf(str, "str%d.png", i);
      i++;
      cv::imwrite(str,img);
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

  if (!bol)
  {
      bol =true;
      //imgOld =img;
      //split image in layer (channels

     /*code */
  }
  ros::spin();

  //cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
  //cv::waitKey(30);
  cv::destroyWindow("view");
}