#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>


double rate_hz = 5; 
geometry_msgs::Twist goal;

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;


void destino(const PointCloud::ConstPtr& msg){
  printf ("Cloud: width = %d, height = %d\n", msg->width, msg->height);
  int line  = 0;
  int point = 0;
  int auxPoint = -1;
  double p1 [3];
  double p2 [3];


  BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)
  {
    printf ("Line: %d\tPoint: %d",line, point);
    printf ("\t(%f, %f, %f)\n", pt.x, pt.y, pt.z);
    point++;
    if(point % msg -> width == 0)
    {
      line++;
      point = 0;
    }
    
    //Detecta las líneas del camino
    if (abs(pt.x) >= 0 && abs(pt.y) >= 0 &&  abs(pt.z) >= 0 && line == 0){
      p1[0] = pt.x;
      p1[1] = pt.y;
      p1[2] = pt.z;
    }else if (abs(pt.x) >= 0 && abs(pt.y) >= 0 &&  abs(pt.z) >= 0 && line == 2){
      p2[0] = pt.x;
      p2[1] = pt.y;
      p2[2] = pt.z;

    }

    //Genera un punto medio entre las líneas identificadas.
    goal.linear.x = (p1[0] + p2[0])/2;
    goal.linear.y = (p1[1] + p2[1])/2;
    goal.linear.z = (p1[2] + p2[2])/2;
  }
}

//MAIN DE KEV. 
int main(int argc, char** argv){
  ros::init(argc, argv, "perception_node");

  ros::NodeHandle nh;

  ros::Rate loop_rate(rate_hz);
  ros::Publisher pub_destino = nh.advertise<geometry_msgs::Twist>("/target_position_topic", rate_hz);
  ros::Subscriber sub = nh.subscribe<PointCloud>("pointCloud_vision", 1, destino);
  while(nh.ok())
  {
    ros::spinOnce();
    loop_rate.sleep();  
    ROS_INFO_STREAM("Desired Destiny:"
    <<"X:"<<goal.linear.x
    <<",Y:"<<goal.linear.y
    <<",W:"<<goal.linear.z);
    pub_destino.publish(goal);
  }

  return 0;
};
