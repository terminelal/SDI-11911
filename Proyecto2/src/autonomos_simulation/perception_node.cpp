#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Header.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>
#include <math.h>
#include <iostream>

#define PI 3.14159265

//For geometry_msgs::Twist using:
// 		dummy.linear.x
// 		dummy.linear.y
// 		dummy.angular.z
geometry_msgs::Twist destiny_position;
double rate_hz = 5;
ros::Publisher pub_pose;
ros::Publisher pub_lidar;
std::string nombre;


struct coords
{
      int object[100];
      double x[100];
      double y[100];
}; 


typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

// PERCEPCION DE LIDAR
void processLaserScan(const sensor_msgs::LaserScan::ConstPtr& scan){
     //scan->ranges[] are laser readings
     coords perspectiva;
     printf("\nLIDAR");
     int max =0;
     bool objetoiniciado = false;
     std::cout << "\nPerspectiva: " << scan->header.frame_id;

     if(scan->header.frame_id != nombre)
	return;     

     // std::string persp = scan->header.frame_id;
     int obj = -1;
     int v = 0;
     for(int i=0;i<360;i++)
     {
	// evitar la carcaza del carro, carcaza: 52-62, 141-154, 205-218, 295-308
	// || (i>62 && i<141) || (i>154 && i<205) || (i>218 && i<295) ||
	// solo considera el frente
	// SE QUITO LA PERCEPCION TRASERA (i>154 && i<205) ||
	// Y LATERAL (i>62 && i<141) ||  (i>218 && i<295) ||
	if(i<52 || i>308)
	{
		if(scan->ranges[i] > 0.1 && scan->ranges[i] < 3.0) { // antes intensity > 0
			if(!objetoiniciado) {
				max = i;
				objetoiniciado = true;
				obj++;
			}
			// printf("\n%d, %f", i, scan->ranges[i]);
			double x1 = scan->ranges[i] * cos(i*PI/180);
			double y1 = scan->ranges[i] * sin(i*PI/180);
			
			if(max != i)
			{
				double x2 = scan->ranges[max] * cos(max*PI/180);
				double y2 = scan->ranges[max] * sin(max*PI/180);
				double dist = sqrt(pow(x2-x1,2) + pow(y2-y1,2));
				if(abs(max-i)>1 || abs(dist)>0.05)
				{
					// cambio de objeto
					obj++;
					printf("\ni: %d max: %d, distancia: %f obj: %d", i, max, dist, obj);
				}
			}

			perspectiva.object[v] = obj;
			perspectiva.x[v] = x1;
			perspectiva.y[v] = y1;
			v++;
			
			max = i;
		}
	}
     }

	
	// itera sobre objetos identificados, agrupa vertices por promedio
	double vertobjs [obj][2];
	int nobj = -1;
	int vxobj =0;
	for(int j=0;j<v;j++)
     	{
		if(nobj != perspectiva.object[j]) {
			if(nobj > -1)
			{
				vertobjs[nobj][0] /= vxobj;
				vertobjs[nobj][1] /= vxobj;
			}
			nobj = perspectiva.object[j];
			printf("\nObjeto %d", nobj);
			vxobj=0;
		}
		printf("\n(%f, %f)", perspectiva.x[j], perspectiva.y[j]);
		
		vxobj++;
		vertobjs[nobj][0] += perspectiva.x[j];
		vertobjs[nobj][1] += perspectiva.y[j];	

		if(j == v-1){
			vertobjs[nobj][0] /= vxobj;
			vertobjs[nobj][1] /= vxobj;
		}
	}
	
	// vertices de objetos observados
	// convertir a pcl::PointCloud<pcl::PointXYZ> y publicar
	geometry_msgs::Twist car_follow;
	printf("\nVertices");
	double x=0.0;
	double y=0.0;
	for(int j=0;j<obj+1;j++)
     	{
		printf("\n %d: (%f, %f)",j,vertobjs[j][0], vertobjs[j][1]);
		x += vertobjs[j][0];
		y += vertobjs[j][1];
	}
	x/=obj+1;
	y/=obj+1;
	car_follow.linear.x = x;
	car_follow.linear.y = y;
	car_follow.angular.z = 0;
     
	pub_lidar.publish(car_follow);

	/*
	if(min > 0 && max > min)
	{
	double x1 = scan->ranges[max] * cos(max);
	double y1 = scan->ranges[max] * sin(max);
	double x2 = scan->ranges[min] * cos(min);
	double y2 = scan->ranges[min] * sin(min);
	printf("\nObjeto encontrado: \n P1: (%f, %f) \n P2: (%f, %f)", x1, y1, x2, y2);
	}
	*/
}


// PERCEPCION DE VISION
void callback(const PointCloud::ConstPtr& msg)
{
  // width = max number of points in each line
  // height = max number of lines
  // not all points are valid, not all lines have valid points.
  printf ("Cloud: width = %d, height = %d\n", msg->width, msg->height);
  int line  = 0;
  int point = 0;
  int l=0;
  bool linea_iniciada = false;
  bool pendiente_calculada = false;
  double theta=0.0;
  double theta2=0.0;
  
  int min = 0, max = 0;
  double puntos_medios[2][2];

  // FALTA SABER A QUE DIRECCION SE DIRIJE EL AUTO
  int direccion = 1; // 2 izquierda, 1 derecha;
  
  // iterate and print all the points in the pointcloud
  BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)
  {
	// workaround to detect NaN numbers
	if(pt.x != pt.x || pt.y != pt.y || pt.z != pt.z)
	{
	    // isNaN
	}
	else {
	    // indices reales de los puntos
	    int p_real = point + line * msg -> width;
	    if(line != direccion && !linea_iniciada)
	    {
		min = p_real;
		max = p_real;
		linea_iniciada = true;
	    }
	    // printf ("Line: %d\tPoint: %d", line, point);
	    // printf ("\t(%f, %f, %f)\n", pt.x, pt.y, pt.z);
	    if(p_real>max)
		max=p_real;
	}
	point++;
	if(point % msg -> width == 0)
	{
	    line++;
	    point = 0;
	    linea_iniciada = false;
	    if(line != direccion && max > 0){
		    // printf("min: %d, max: %d\n", min, max);
		    // calcular pendiente en grados
		    if(!pendiente_calculada){
			// angulo de la pose final
			double m = (msg->points[max].y - msg->points[min].y) / (msg->points[max].x - msg->points[min].x);
	  		theta = atan(m) * 180 / PI;
		    	pendiente_calculada=true;
			// printf("\n(x1: %f, y1: %f)", msg->points[min].x, msg->points[min].y);
			// printf("\n(x2: %f, y2: %f)", msg->points[max].x, msg->points[max].y);
			// printf("\nm: %f, angulo: %f", m, theta);
		    }
		    // calcular punto medio linea anterior
		    puntos_medios[l][0] = (msg->points[min].x + msg->points[max].x)/2;
		    puntos_medios[l][1] = (msg->points[min].y + msg->points[max].y)/2;
		    // printf("\nPunto medio linea %d: (%f,%f)", line-1,puntos_medios[l][0], puntos_medios[l][1]);
		    l++;
		}
	}
  }
	geometry_msgs::Twist desired_pose;
	// es necesario que el robot vea 2 lineas para emitir una pose
	if(l==2){
		// calcular punto medio de los puntos medios
		double x = (puntos_medios[0][0] + puntos_medios[1][0])/2;
		double y = (puntos_medios[0][1] + puntos_medios[1][1])/2;
		// double alpha = atan2(y, x) * 180 / PI;		
		// double beta = -theta-alpha;
		printf("\n Punto a desplazarse: (%f,%f), con angulo %f \n", x,y, theta);
		//ROS_INFO_STREAM use for debugging 
		desired_pose.linear.x = x;
		desired_pose.linear.y = y;
		desired_pose.angular.z = theta;
	} else {
		
		desired_pose.linear.x = 0;
		desired_pose.linear.y = 0;
		desired_pose.angular.z = 0;
	}
	
	pub_pose.publish(desired_pose);
}

int main(int argc, char** argv){
  ros::init(argc, argv, "perception_node");
  ros::NodeHandle nh;
  ros::Rate loop_rate(rate_hz);

	const std::string PARAM1 = "~lidar";
	bool okx = ros::param::get(PARAM1, nombre);
	if(!okx) {
		ROS_FATAL_STREAM("No se pudo obtener el parametro " << PARAM1);
		exit(1);
	}
	else
		std::cout<<"\nSolo concentrarse en lidar:"<<nombre;

  ros::Subscriber sub = nh.subscribe<PointCloud>("pointCloud_vision", 1, callback);

  pub_pose = nh.advertise<geometry_msgs::Twist>("/target_pose", rate_hz);
  pub_lidar = nh.advertise<geometry_msgs::Twist>("/target_lidar", rate_hz);


	
	

  ros::Subscriber scanSub=nh.subscribe<sensor_msgs::LaserScan>("/laser_scan",1, processLaserScan);
  while(nh.ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
};
