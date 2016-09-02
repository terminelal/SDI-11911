#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <sstream>

using namespace std;

ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;
const double PI = 3.14159265359;

void move(double speed, double distance, bool isForward);
void rotate(double angular_speed, double angle, bool cloclwise);
double degrees2radians(double angle_in_degrees);		
double setDesiredOrientation(double desired_angle_radians);	
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);
void moveGoal(turtlesim::Pose goal_pose, double distance_tolerance, double t);
double getDistance(double x1, double y1, double x2, double y2);


int main(int argc, char **argv)
{
	ros::init(argc, argv, "publicador");
	ros::NodeHandle n;
	double speed, angular_speed;
	double distance, angle, X, Y, theta, Time, t, theta2,dist;
	bool isForward, clockwise, flag=1;

	velocity_publisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
	pose_subscriber = n.subscribe("/turtle1/pose", 10, poseCallback);
	ros::Rate loop_rate(10);

	ROS_INFO("\n\n\n BIENVENIDO \n");

	do{
	cout<<"Ingresa coordenadas destino: \n";
	cout<<"X =    ";
	cin>>X;
	cout<<"\n";
	cout<<"Y =    ";
	cin>>Y;
	cout<<"\n";
	cout<<"Angulo final =    ";
	cin>>theta;
	cout<<"\n";
	cout<<"Tiempo deseado =    ";
	cin>>t;
	
	turtlesim::Pose goal_pose;
	goal_pose.x = X;
	goal_pose.y = Y;
	goal_pose.theta = theta;
	moveGoal(goal_pose, 0.5, t);
	setDesiredOrientation(degrees2radians(theta));	
	
	cout<<"Desea ingresar nuevas coordenadas? (1=si 0=no)\n";
	loop_rate.sleep();
	cin>>flag;
}while(flag);
	

	ros::spin();
exit(0);
}

void move(double speed, double distance, bool isForward){
	geometry_msgs::Twist vel_msg;
   if (isForward)
	   vel_msg.linear.x =abs(speed);
   else
	   vel_msg.linear.x =-abs(speed);
   vel_msg.linear.y =0;
   vel_msg.linear.z =0;
   vel_msg.angular.x = 0;
   vel_msg.angular.y = 0;
   vel_msg.angular.z =0;

   double t0 = ros::Time::now().toSec();
   double current_distance = 0.0;
   ros::Rate loop_rate(100);
   do{
	   velocity_publisher.publish(vel_msg);
	   double t1 = ros::Time::now().toSec();
	   current_distance = speed * (t1-t0);
	   ros::spinOnce();
	   loop_rate.sleep();
   }while(current_distance<distance);
   vel_msg.linear.x =0;
   velocity_publisher.publish(vel_msg);

}


void rotate (double angular_speed, double relative_angle, bool clockwise){

	geometry_msgs::Twist vel_msg;
	   vel_msg.linear.x =0;
	   vel_msg.linear.y =0;
	   vel_msg.linear.z =0;
	   vel_msg.angular.x = 0;
	   vel_msg.angular.y = 0;
	   if (clockwise)
	   	vel_msg.angular.z =-abs(angular_speed);
	   else
	   	vel_msg.angular.z =abs(angular_speed);

	   double t0 = ros::Time::now().toSec();
	   double current_angle = 0.0;
	   ros::Rate loop_rate(1000);
	   do{
		   velocity_publisher.publish(vel_msg);
		   double t1 = ros::Time::now().toSec();
		   current_angle = angular_speed * (t1-t0);
		   ros::spinOnce();
		   loop_rate.sleep();
	   }while(current_angle<relative_angle);
	   vel_msg.angular.z =0;
	   velocity_publisher.publish(vel_msg);
	
}



double degrees2radians(double angle_in_degrees){
	return angle_in_degrees *PI /180.0;
}

double setDesiredOrientation(double desired_angle_radians)
{	
	double relative_angle_radians = desired_angle_radians - turtlesim_pose.theta;
	bool clockwise = ((relative_angle_radians<0)?true:false);
	rotate (abs(relative_angle_radians), abs(relative_angle_radians), clockwise);
}



void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
	turtlesim_pose.theta=pose_message->theta;
}


void moveGoal(turtlesim::Pose goal_pose, double distance_tolerance, double tiempo){
	
	geometry_msgs::Twist vel_msg;

	ros::Rate loop_rate(10);
	do{
		//linear velocity 
		vel_msg.linear.x = 1.75* getDistance(turtlesim_pose.x, turtlesim_pose.y, goal_pose.x, goal_pose.y)/tiempo;
		vel_msg.linear.y = 0;
		vel_msg.linear.z = 0;
		//angular velocity
		vel_msg.angular.x = 0;
		vel_msg.angular.y = 0;
		vel_msg.angular.z = 4*(atan2(goal_pose.y - turtlesim_pose.y, goal_pose.x - turtlesim_pose.x)-turtlesim_pose.theta);

		velocity_publisher.publish(vel_msg);
		ros::spinOnce();
		loop_rate.sleep();
		
	}while(getDistance(turtlesim_pose.x, turtlesim_pose.y, goal_pose.x, goal_pose.y)>distance_tolerance);
	cout<<"Listo!"<<endl;
	vel_msg.linear.x = 0;
	vel_msg.angular.z = 0;
	velocity_publisher.publish(vel_msg);

}

double getDistance(double x1, double y1, double x2, double y2){
	return sqrt(pow((x2-x1),2) + pow((y2-y1),2));
}
