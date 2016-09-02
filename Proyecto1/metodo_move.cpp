#include <ros/ros.h>
#include <geometry_msgs/Twist.h> 
#include <turtlesim/Pose.h>
#include <iomanip>
#include <stdlib.h>
#include <sstream>

using namespace std;

ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;

void rotar(double vel_ang, double angulo, bool sentido);
double grados_radianes(double angGrados);		
double orient(double ang);
void turtleM(turtlesim::Pose do_pose, double time, double distT);
double getDistance(double x1, double y1, double x2, double y2);
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);


int main(int argc, char **argv) {
	ros::init(argc, argv, "hello_ros");
	ros::NodeHandle nh;

	velocity_publisher = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
	pose_subscriber = nh.subscribe("/turtle1/pose", 10, poseCallback);	
	ros::Rate loop_rate(0.5);

	turtlesim::Pose do_pose;
	double time;
        double convGrados;

        cout<<"INGRESE CORDENADAS Y ANGULO: ";
	cout<<"ingrese x: ";
	cin>>do_pose.x;
	cout<<"ingrese y: ";
	cin>>do_pose.y;
	cout<<"ingrese grados: ";
	cin>>convGrados;
	cout<<"ingrese tiempo: ";
	cin>>time;
        do_pose.theta=grados_radianes(convGrados);
	turtleM(do_pose, time, 0.01);

	loop_rate.sleep();	

	ros::spin();

	return 0;

}



void turtleM(turtlesim::Pose do_pose, double time, double distT){

	geometry_msgs::Twist vel_msg;

	double t0 = ros::Time::now().toSec();
	double dt = 0;

	ros::Rate rate(10);

	do {
		dt = time - (ros::Time::now().toSec()-t0);
		//linear velocity 
		vel_msg.linear.x = getDistance(turtlesim_pose.x, turtlesim_pose.y, do_pose.x, do_pose.y)/dt;
		vel_msg.linear.y = 0;
		vel_msg.linear.z = 0;
		//angular velocity
		vel_msg.angular.x = 0;
		vel_msg.angular.y = 0;
		vel_msg.angular.z = 4*(atan2(do_pose.y - turtlesim_pose.y, do_pose.x - turtlesim_pose.x)-turtlesim_pose.theta);

		velocity_publisher.publish(vel_msg);

		ros::spinOnce();
		rate.sleep();

	}while((getDistance(turtlesim_pose.x, turtlesim_pose.y, do_pose.x, do_pose.y)>distT) && (dt > 0));
	vel_msg.linear.x = 0;
	vel_msg.angular.z = 0;
	velocity_publisher.publish(vel_msg);
	orient(do_pose.theta);

}

double getDistance(double x1, double y1, double x2, double y2){
	return sqrt(pow((x2-x1),2) + pow((y2-y1),2));
}

void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
	turtlesim_pose.theta=pose_message->theta;
}
void rotar(double vel_ang, double angulo, bool sentido){

	geometry_msgs::Twist vel_msg;
	   vel_msg.linear.x =0;
	   vel_msg.linear.y =0;
	   vel_msg.linear.z =0;
	 
	   vel_msg.angular.x = 0;
	   vel_msg.angular.y = 0;
	   if (sentido)
	   	vel_msg.angular.z =-abs(vel_ang);
	   else
	   	vel_msg.angular.z =abs(vel_ang);

	   double t0 = ros::Time::now().toSec();
	   double ang0 = 0.0;
	   ros::Rate loop_rate(1000);
	   do{
		   velocity_publisher.publish(vel_msg);
		   double t1 = ros::Time::now().toSec();
		   ang0 = vel_ang * (t1-t0);
		   ros::spinOnce();
		   loop_rate.sleep();
		  
	   }while(ang0<angulo);
	   vel_msg.angular.z =0;
	   velocity_publisher.publish(vel_msg);
}



double grados_radianes(double angGrados){
	return angGrados *3.1416 /180.0;
}


double orient(double ang)
{	
	double angulo_radians = ang - turtlesim_pose.theta;
	bool sentido = ((angulo_radians<0)?true:false);
	rotar (abs(angulo_radians), abs(angulo_radians), sentido);
}



