#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>


double rate_hz = 5;
double intDistanceL0;
double intDistanceL3;

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

void callback(const PointCloud::ConstPtr& msg)
{
  // width = max number of points in each line
  // height = max number of lines
  // not all points are valid, not all lines have valid points.
  
  int line  = 0;
  int point = 0;
  int arreX[100];
  int arreY[100];
  double intXMinL1 = 0;
  double intXMaxL1 = 0;
  double intYMinL1 = 0;
  double intYMaxL1 = 0;
  double intXMinL2 = 0;
  double intXMaxL2 = 0;
  double intYMinL2 = 0;
  double intYMaxL2 = 0;
  bool boolPX1=true;
  bool boolPX2=true;
  
  double meanX1;
  double meanY1;
  double meanX2;
  double meanY2;
  // iterate and print all the points in the pointcloud
  BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)
  {
    if (pt.x==pt.x)
    {
      if (line==1)
      {
        if(boolPX1){
          intXMinL1 = pt.x;
          intYMinL1 = pt.y;
          boolPX1=false;
        }
        intXMaxL1 = pt.x;
        intYMaxL1 = pt.y;
      }if (line==2)
      {
        if(boolPX2){
          intXMinL2 = pt.x;
          intYMinL2 = pt.y;
          boolPX2=false;
        }
        intXMinL2 = pt.x;
        intYMaxL2 = pt.y;
      }
      //printf ("Line: %d\tPoint: %d",line, point);
      //printf ("\t(%f, %f, %f)\n", pt.x, pt.y, pt.z);
    }
    point++;
    if(point % msg -> width == 0)
    {
      line++;
      point = 0;
    }

    
      
  }
  // Idea: hacer que mantenga una distancia sobre la media del punto.
  // s´olo debe de cambiar el giro
  //linea  0  es el primer arrego, si fuera el 1 o 2 tendría que sumarles el tamñao de msg uno o dos 
  intDistanceL0 = fabs(intXMaxL1-intXMinL1);
    intDistanceL3 = fabs(intXMaxL2-intXMinL2);
     //printf ("minX: %f\tmxx: %f\n",intXMinL1, intXMaxL1);
     //printf ("L1: %f\tl2: %f\n",intDistanceL0, (intXMaxL1-intXMinL1));

}

int main(int argc, char** argv){
  ros::init(argc, argv, "perception_node");

  ros::NodeHandle nh;


  ros::Rate loop_rate(rate_hz);

  geometry_msgs::Twist desired_velocity;
  ros::Subscriber sub = nh.subscribe<PointCloud>("pointCloud_vision", 1, callback);

  ros::Publisher pub_vel_turtle = nh.advertise<geometry_msgs::Twist>("/target_vel_topic", rate_hz);
  while(nh.ok())
  {

        desired_velocity.linear.x = 0.01;
        desired_velocity.linear.y = 0;
        //printf ("distance: l0 = %f",intDistanceL0);
        //printf ("L1 = %f\n",intDistanceL3);
    
    if (intDistanceL3>  .1 && intDistanceL3<.20)
    {
        desired_velocity.angular.z = -10;
        printf("pido\n");
    }
  else
    { 
        if (intDistanceL0>  .1 && intDistanceL0<.20)
            desired_velocity.angular.z = 10;
        else{
           desired_velocity.angular.z = 10;
        }
       
    }
     printf ("L1 = %f\t L2 = %f\n",intDistanceL0,intDistanceL3); 
    //ROS_INFO_STREAM use for debugging 

  
    ROS_INFO_STREAM("Desired Velocity:"
      <<"X:"<<desired_velocity.linear.x
      <<",Y:"<<desired_velocity.linear.y
      <<",W:"<<desired_velocity.angular.z);

    //publish the new velocity
    pub_vel_turtle.publish(desired_velocity);
    
    ros::spinOnce();
    loop_rate.sleep();

  }
  return 0;
};