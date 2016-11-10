#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>


double rate_hz = 5;
typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

void callback(const PointCloud::ConstPtr& msg)
{
  // width = max number of points in each line
  // height = max number of lines
  // not all points are valid, not all lines have valid points.
  
  int line  = 0;
  int point = 0;

  // iterate and print all the points in the pointcloud
  BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)
  {
    //printf ("Line: %d\tPoint: %d",line, point);
    //printf ("\t(%f, %f, %f)\n", pt.x, pt.y, pt.z);
    point++;
    if(point % msg -> width == 0)
    {
      line++;
      point = 0;
    }
  }
}

int main(int argc, char** argv){
  ros::init(argc, argv, "perception_node");
  ros::NodeHandle nh;
  ros::Rate loop_rate(rate_hz);
  geometry_msgs::Twist msg_velocity;
  ros::Subscriber sub = nh.subscribe<PointCloud>("pointCloud_vision", 1, callback);

  ros::Publisher pub_velocity = nh.advertise<geometry_msgs::Twist>("/target_vel_topic", rate_hz);
  while(nh.ok())
  {

        msg_velocity.linear.x = 0.01;
        msg_velocity.linear.y = 0.01;
	msg_velocity.angular.z = 0;
	pub_velocity.publish(msg_velocity);
    
    ros::spinOnce();
    loop_rate.sleep();

  }
  return 0;
};
