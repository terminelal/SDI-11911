/*
 * robot.cpp
 *
 *  Created on: 28/08/2016
 *      Author: sebas
 */

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <sstream>
using namespace std;

ros::Publisher velocity_publisher;
turtlesim::Pose turtlesim_pose;

const double PI = 3.1415;

double degrees2radians(double degrees);
void moveGoal(double x, double y,double segundos);
double getDistance(double x1,double x2, double y1, double y2);
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);
void rotar(double angulo);


int main(int argc, char **argv){
	ros::init(argc,argv,"robot");
	ros::NodeHandle n;
	double x,y,seconds,theta;
	velocity_publisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",1);
	ros::Subscriber pose_subscriber = n.subscribe("/turtle1/pose",1,poseCallback);
	bool correr = true;
	char respuesta;

	//esperar  recibir informacion de turtlesim
	while(turtlesim_pose.x==0){

	ros::spinOnce();
	}

	while(correr){
	cout<<turtlesim_pose;
	cout<<"Coordenada X [0-11]: ";
	cin>>x;
	while(x<0 || x>11){
		cout<<"El valor debe de ser entre 0 y 11, mi casa no es tan grande: ";
			cin>>x;
	}
	cout<<"Coordenada Y [0-11]: ";
	cin>>y;
	while(y<0 || y>11){
			cout<<"El valor debe de ser entre 0 y 11, mi casa no es tan grande: ";
				cin>>y;
		}
	cout<<"Angulo (grados): ";
	cin>>theta;
	cout<<"Tiempo (segundos) (>3): ";
	cin>>seconds;
	while(seconds<3){
		cout<<"Soy una tortuga, me tardo al menos 3 segundos: ";
			cin>>seconds;
	}

	moveGoal(x,y,seconds);
	rotar(degrees2radians(theta)-turtlesim_pose.theta);
	cout<<"Termine, ¿quieres que lo haga de nuevo? (s/n): ";
	cin>>respuesta;
	while(respuesta != 's' && respuesta != 'n'){
		cout<<"No te entendi, ¿quieres que lo haga de nuevo? (s/n): ";
		cin>>respuesta;
	}
	if(respuesta =='n'){
		cout<<"ok, adios! \n";
		correr=false;
	}
	else
		correr=true;
	}
}

void rotar(double angulo){

	//arbitrariamente decidi que el gira final durara 1 segundo, el cual en ese segundo girara
		//la diferencia de angulos
	geometry_msgs::Twist vel_msg;
		ros::Rate loop_rate(10);
		int i=10;
		do{
			vel_msg.angular.x=0;
			vel_msg.angular.y=0;
			vel_msg.angular.z= angulo;
			velocity_publisher.publish(vel_msg);
			ros::spinOnce();
			loop_rate.sleep();
			i--;
		}
		while(i>0);
				vel_msg.angular.z=0;
				velocity_publisher.publish(vel_msg);
}

double degrees2radians(double degrees){
	return degrees*PI /180.0;
}

double getDistance(double x1,double x2, double y1, double y2){
	return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

void moveGoal(double x, double y, double segundos){
	segundos=segundos-1;
	geometry_msgs::Twist vel_msg;
	ros::Rate loop_rate(10);
	double angulo_objetivo=atan2(y-turtlesim_pose.y,x-turtlesim_pose.x)-turtlesim_pose.theta;
	double t0 = ros::Time::now().toSec();
		do{
			double t1 = ros::Time::now().toSec();
			//Velocidad lineal, la cual garantiza que terminara en el tiempo indicado
			vel_msg.linear.x = abs(getDistance(turtlesim_pose.x, x, y, turtlesim_pose.y)/(segundos-(t1-t0)));
			vel_msg.linear.y = 0;
			vel_msg.linear.z = 0;

			//velocidad angular
			vel_msg.angular.x = 0;
			vel_msg.angular.y = 0;
			vel_msg.angular.z = 4*(atan2(y - turtlesim_pose.y, x - turtlesim_pose.x)-turtlesim_pose.theta);
			velocity_publisher.publish(vel_msg);

			//Esperar para tener la frecuencia de 10 Hz
			ros::spinOnce();
			loop_rate.sleep();

		}while(getDistance(turtlesim_pose.x, x, turtlesim_pose.y, y)>.1);
	vel_msg.linear.x=0;
	velocity_publisher.publish(vel_msg);
	ros::spinOnce();
}

void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
	turtlesim_pose.theta=pose_message->theta;
}
