#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <sstream>

using namespace std;

ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;	
turtlesim::Pose turtlesim_pose;

const double PI = 3.14159265359;




void pros(bool pro);
void move(double speed, double distance, bool isForward);
void rotate(double angular_speed, double angle, bool cloclwise);	//this will rotate the turtle at specified angle from its current angle
double degrees2radians(double angle_in_degrees);		
double setDesiredOrientation(double desired_angle_radians);	//this will rotate the turtle at an absolute angle, whatever its current angle is
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);	//Callback fn everytime the turtle pose msg is published over the /turtle1/pose topic.
void moveGoal(turtlesim::Pose goal_pose, double distance_tolerance,double time);	//this will move robot to goal
double getDistance(double x1, double y1, double x2, double y2);

int main(int argc, char **argv)
{
	
	double angle,X,Y,time;
	bool pro;
	ros::init(argc, argv, "turtlesim_cleaner");
	ros::NodeHandle n;
	

	velocity_publisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
	pose_subscriber = n.subscribe("/turtle1/pose", 10, poseCallback);	//call poseCallback everytime the turtle pose msg is published over the /turtle1/pose topic.
	ros::Rate loop_rate(10);

	ROS_INFO("\n\n\n ***Arranca***\n");


	pros(true);
	
	

	return 0;
}

void pros(bool pro){
	
	double X,Y,time,theta;
	bool p;
	turtlesim::Pose goal_pose;
	
	X=0;
	Y=0;
	theta=0;
	time=0;

	if (pro){

	cout<<"Posicion final x: ";
	cin>>X;
	cout<<"Posicion final y: ";
	cin>>Y;
	cout<<"Orientacion final en grados: ";
	cin>>theta;
	cout<<"Por favor da un tiempo aproximado: ";
	cin>>time;

	
	
	goal_pose.x = X;
	goal_pose.y = Y;
	
	
	moveGoal(goal_pose,0.5,time);
	theta=degrees2radians(theta);

	setDesiredOrientation(theta);

       

	cout<<"Desea Continuar: ";
	cin>>p;
	pros(p);

	}else{
		exit(0);	
	}
}



void rotate (double angular_speed, double relative_angle, bool clockwise){

	geometry_msgs::Twist vel_msg;
	   //set a random linear velocity in the x-axis
	   vel_msg.linear.x =0;
	   vel_msg.linear.y =0;
	   vel_msg.linear.z =0;
	   //set a random angular velocity in the y-axis
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
		   //cout<<(t1-t0)<<", "<<current_angle <<", "<<relative_angle<<endl;
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
	//if we want to turn at a perticular orientation, we subtract the current orientation from it
	bool clockwise = ((relative_angle_radians<0)?true:false);
	//cout<<desired_angle_radians <<","<<turtlesim_pose.theta<<","<<relative_angle_radians<<","<<clockwise<<endl;
	rotate (abs(relative_angle_radians), abs(relative_angle_radians), clockwise);
}

/**
 *  callback function to update the pose of the robot  
 */

void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
	turtlesim_pose.theta=pose_message->theta;
}


void moveGoal(turtlesim::Pose goal_pose, double distance_tolerance, double time){
	//We implement a Proportional Controller. We need to go from (x,y) to (x',y'). Then, linear velocity v' = K ((x'-x)^2 + (y'-y)^2)^0.5 where K is the constant and ((x'-x)^2 + (y'-y)^2)^0.5 is the Euclidian distance. The steering angle theta = tan^-1(y'-y)/(x'-x) is the angle between these 2 points.
	geometry_msgs::Twist vel_msg;

	ros::Rate loop_rate(10);
	do{
		//linear velocity 
		vel_msg.linear.x = 1.5 * getDistance(turtlesim_pose.x, turtlesim_pose.y, goal_pose.x, goal_pose.y)/time *3;
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
	cout<<"end move goal"<<endl;
	vel_msg.linear.x = 0;
	vel_msg.angular.z = 0;
	velocity_publisher.publish(vel_msg);

}

double getDistance(double x1, double y1, double x2, double y2){
	return sqrt(pow((x2-x1),2) + pow((y2-y1),2));
}
