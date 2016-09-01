/*
 *  magdiel.cpp
 *  Author: Job Magdiel Gonzalez Gonzalez
 */

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <sstream>

using namespace std;

/* Objetos */
ros::Publisher publisher;
ros::Subscriber subscriber;	
turtlesim::Pose turtlesim_pose;

/* Constantes */
const double x_min = 0.0;
const double y_min = 0.0;
const double x_max = 11.0;
const double y_max = 11.0;
const double PI = 3.14159265359;

/* Metodos del programa */
void objetivo(double posFinX, double posFinY, double posTime);
double calcDist(double posIniX, double posIniY, double posFinX, double posFinY);
void posActualiza(const turtlesim::Pose::ConstPtr & pose_message);
void camina(turtlesim::Pose goal_pose, double tolerancia, double posTime);
double degrees2radians(double degrees);
void rotar(double angulo);

/* Main */
int main(int argc, char **argv)
{
	// Initialize the ROS system and become a node.
	ros::init(argc, argv, "Magdiel_Proyecto_Uno");
	ros::NodeHandle nh;

	/* Variables locales */
	double posFinX, posFinY, posTime, theta;
	bool posNueva = false;

	//Publisher object.
	publisher = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);

	//Subscriber object.
	subscriber = nh.subscribe("/turtle1/pose", 10, posActualiza);

	// Loop at 10Hz until the node is shut down.
	ros::Rate loop_rate(10);
	
	bool bandera = true;	

	while(bandera){
		do {			
			//Pide y lee posX
			cout<<"Posicion final X [0-11]: ";
			cin>>posFinX;
			while(posFinX<0 || posFinX>11){
				cout<<"Posicion final X [0-11]: ";
				cin>>posFinX;
			}

			//Pide y lee posY
			cout<<"posicion final Y [0-11]: ";
			cin>>posFinY;
			while(posFinY<0 || posFinY>11){
				cout<<"Posicion final Y [0-11]: ";
				cin>>posFinY;
			}

			//Pide y lee angulo
			cout<<"Angulo final [0-360 grados]: ";
			cin>>theta;
			while(theta<0 || theta>360){
				cout<<"Angulo [0-360 grados]: ";
				cin>>theta;
			}

			//Pide y lee posTime
			cout<<"Tiempo de ejecucion [mayor de 2 segundos]: ";
			cin>>posTime;
			while(posTime<2){
				cout<<"Tiempo de ejecucion [mayor de 2 segundos]: ";
				cin>>posTime;
			}
		
			double t0 = ros::Time::now().toSec();

			//Moverse a posicion final
			objetivo(posFinX, posFinY, posTime);

			//Rotar
			rotar(degrees2radians(theta)-turtlesim_pose.theta);

			double t1 = ros::Time::now().toSec();
			double time = t1-t0;

			//Pregunta si se desea ingresar otro punto
			cout<<"Desea ingresar otro punto? (Si=1, No=0) [Tiempo:" << time <<"]: " ; 
			cin>>posNueva;		
		}
		while(posNueva);

		// Wait until it's time for another iteration.
		loop_rate.sleep();

		//Exit
		return 0;

	}

	//Let ROS take over.
	ros::spin();

	//Exit
	return 0;
}

/**
 * Fija la posicion objetivo
 *
 */
void objetivo(double posFinX, double posFinY, double posTime){
	//Posicion objetivo
	turtlesim::Pose goal_pose;	
	goal_pose.x = posFinX;
	goal_pose.y = posFinY;
	goal_pose.theta = 0;

	//Moverse a la posicion objetivo
	camina(goal_pose, 0.1, posTime);
}

/**
 *  Durante la ejecucion, se mantiene actualizada 
 *  la posiscion actual con los datos publicados por Turtlesim
 */
void posActualiza(const turtlesim::Pose::ConstPtr & pose_message){
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
	turtlesim_pose.theta=pose_message->theta;
}

/** 
 * Calcula la velocidad lineal y angular. 
 * Publica dicahs velocidades
*/
void camina(turtlesim::Pose goal_pose, double tolerancia, double posTime){
	
	posTime = posTime-1;

	//Instancia mensaje
	geometry_msgs::Twist msg_vel;

	//Loop at 10Hz until the node is shut down.
	ros::Rate loop_rate(10);
	double t0 = ros::Time::now().toSec();

	do{
		//Velocidad lineal
		double t1 = ros::Time::now().toSec();
		//msg_vel.linear.x = calcDist(turtlesim_pose.x, turtlesim_pose.y, goal_pose.x, goal_pose.y)/(posTime/4.25);
		msg_vel.linear.x = calcDist(turtlesim_pose.x, turtlesim_pose.y, goal_pose.x, goal_pose.y)/(posTime-(t1-t0));
		msg_vel.linear.y = 0;
		msg_vel.linear.z = 0;

		//Velocidad angular
		msg_vel.angular.x = 0;
		msg_vel.angular.y = 0;
		msg_vel.angular.z = 4*(atan2(goal_pose.y - turtlesim_pose.y, goal_pose.x - turtlesim_pose.x)-turtlesim_pose.theta);

		//Escribe las velocidades
		publisher.publish(msg_vel);
		ROS_INFO_STREAM("VELOCIDADES:" << " linear=" << msg_vel.linear.x << " angular=" << msg_vel.angular.z);

		//Wait until it's time for another iteration.
		ros::spinOnce();
		loop_rate.sleep();

	}while(calcDist(turtlesim_pose.x, turtlesim_pose.y, goal_pose.x, goal_pose.y)>tolerancia);

	//Se detiene
	msg_vel.linear.x = 0;
	msg_vel.angular.z = 0;
	publisher.publish(msg_vel);

}

/**
 *  Calcula la distancia euclidiana entre dos puntos
 */
double calcDist(double iniX, double iniY, double finX, double finY) {
	return sqrt(pow((iniX-finX),2) + pow((iniY-finY),2));
}

/**
 *  Se posiciona en posicion horizontal la tortuga
 */
void rotar(double angulo) {

	geometry_msgs::Twist msg_vel;
	ros::Rate loop_rate(10);
	int i=10;
	do{
		msg_vel.angular.x=0;
		msg_vel.angular.y=0;
		msg_vel.angular.z= angulo;
		publisher.publish(msg_vel);
		ros::spinOnce();
		loop_rate.sleep();
		i--;
	}
	while(i>0);
	msg_vel.angular.z=0;
	publisher.publish(msg_vel);

	/*double vel_angular = 1000000000000000000;

	//Instancia mensaje
	geometry_msgs::Twist msg_vel;

	//Velocidad lineal = 0
	msg_vel.linear.x = 0;
	msg_vel.linear.y = 0;
	msg_vel.linear.z = 0;

	//Velocidad angular = 0
	msg_vel.angular.x = 0;
	msg_vel.angular.y = 0;
	msg_vel.angular.z = vel_angular;

	//Actualizar angulo
	double t0 = ros::Time::now().toSec();
	double angulo_actual = 0.0;
	ros::Rate loop_rate(10);
	do{
	   publisher.publish(msg_vel);
	   double t1 = ros::Time::now().toSec();
	   angulo_actual = vel_angular ;
	   ros::spinOnce();
	   loop_rate.sleep();
	}while(angulo_actual<angulo);
	msg_vel.angular.z =0;
	publisher.publish(msg_vel);*/
}

/** 
 * Funcion de conversion de grados a radianes 
 */
double degrees2radians(double degrees){
	return degrees*PI /180.0;
}
