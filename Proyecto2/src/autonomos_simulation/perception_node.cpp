#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>

//For geometry_msgs::Twist using:
// 		dummy.linear.x
// 		dummy.linear.y
// 		dummy.angular.z
geometry_msgs::Twist destiny_position;
double rate_hz = 5;

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

void callback(const PointCloud::ConstPtr& msg)
{
  // width = max number of points in each line
  // height = max number of lines
  // not all points are valid, not all lines have valid points.
  printf ("Cloud: width = %d, height = %d\n", msg->width, msg->height);
  int line  = 0;
  int point = 0;
  int j=0;
  bool linea_iniciada = false;
  
  int min;
  double [2][2] puntos_medios;
  
  // iterate and print all the points in the pointcloud
  BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)
  {
	// workaround to detect NaN numbers
	if(pt.x != pt.x || pt.y != pt.y || pt.z != pt.z)
	{
	    // isNaN
	}
	else {
	    if(line != 1 && !linea_inicializada)
	    {
		min = j;
		linea_inicializada = true;
	    }
	    printf ("Line: %d\tPoint: %d", line, point);
	    printf ("\t(%f, %f, %f)\n", pt.x, pt.y, pt.z);
	}
	point++;
	if(point % msg -> width == 0)
	{
	    // calcular punto medio linea anterior
	    puntos_medios[line][0] = (msg->points[min].x + msg->points[j].x)/2;
	    puntos_medios[line][1] = (msg->points[min].y + msg->points[j].y)/2;
		
	    line++;
	    point = 0;
	    linea_iniciada = false;
	}
	j++;
  }

	for(int i=0;i<2;i++)
	{
		double x = (puntos_medios[0][0] + puntos_medios[1][0])/2;
		double y = (puntos_medios[0][1] + puntos_medios[1][1])/2;
		printf("\n Punto a desplazarse: (%f,%f)", x,y);
	}
/*
  destiny_position.linear.x;
  destiny_position.linear.y;
  destiny_position.linear.z;

  destiny_position.angular.x;
  destiny_position.angular.y;
  destiny_position.angular.z;
*/
}

int main(int argc, char** argv){
  ros::init(argc, argv, "perception_node");
  ros::NodeHandle nh;
  ros::Rate loop_rate(rate_hz);

  ros::Subscriber sub = nh.subscribe<PointCloud>("pointCloud_vision", 1, callback);

  while(nh.ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
};
