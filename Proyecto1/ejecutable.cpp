//Incluimos las paqueterías necesarias para que el código funcione.
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <sstream>
#include <turtlesim/Spawn.h>

using namespace std;

//Inicializamos las suscripciones y publicaciones necesarias. 
ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;cc

const double PI = 3.14159265359;

double degrees2radians(double angle_in_degrees);
double setDesiredOrientation(double desired_angle_radians);
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);
void moveGoal(turtlesim::Pose goal_pose, double distance_tolerance, double tdepz);
double getDistance(double x1, double y1, double x2, double y2);

//Código principal.
int main(int argc, char **argv)
{
	ros::init(argc, argv, "turtlesim_cleaner");
	ros::NodeHandle n;
	double speed, angular_speed;
	double distance, angle, posX, posY;
	bool seguir;
	double tdepz, ang;

	velocity_publisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
	pose_subscriber = n.subscribe("/turtle1/pose", 10, poseCallback);	
	ros::Rate loop_rate(10); //Se define la frecuencia a la cual se mandarán los mensajes.

	ROS_INFO("\n\n\n ********COMIENZA EL CÓDIGO*********\n");
	seguir = true;
	while(seguir){
	//Pedimos los parámetros necesarios para que funcione el código.
	cout<<"Posicion Final X: ";
	cin>>posX;
	cout<<"Posicion Final Y: ";
	cin>>posY;
	cout<<"Tiempo de ejecución: ";
	cin>>tdepz;
	cout<<"Ángulo final: ";
    cin>>ang;

	turtlesim::Pose goal_pose;
	goal_pose.x = posX;
	goal_pose.y = posY;
	goal_pose.theta = 15;
	moveGoal(goal_pose, 1, tdepz);
	setDesiredOrientation(degrees2radians(ang));

    //Preguntamos si se quiere seguir dando instrucciones a la tortuga.
	cout<<"¿Quieres seguir?";
    cin>>seguir;
	}
	loop_rate.sleep();
        ros::spin();
        return 0;
}

//Este método convierte los ángulos de grados a radianes.
double degrees2radians(double angle_in_degrees){
	return angle_in_degrees *PI /180.0;
}

//Este método rota a la tortuga a un ángulo específico después de que llega a
//la posición final.
void rotate (double angular_speed, double relative_angle, bool clockwise){
	   geometry_msgs::Twist vel_msg;
	   //Sirve para definir la velocidad en el eje x
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
	       //Publica la velocidad al nodo de la tortuga.
		   velocity_publisher.publish(vel_msg);
		   double t1 = ros::Time::now().toSec();
		   current_angle = angular_speed * (t1-t0);
		   ros::spinOnce();
		   loop_rate.sleep();
	   }while(current_angle<relative_angle);
	   vel_msg.angular.z =0;
	   velocity_publisher.publish(vel_msg);
}

double setDesiredOrientation(double desired_angle_radians){	
	double relative_angle_radians = desired_angle_radians - turtlesim_pose.theta;
	bool clockwise = ((relative_angle_radians<0)?true:false);
	rotate (abs(relative_angle_radians), abs(relative_angle_radians), clockwise);
}

void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
	turtlesim_pose.theta=pose_message->theta;
}

//Método principal que reune en sí el análisis geométrico y los métodos anteriores
//para llevar a la tortuga de la posición actual a la final.
void moveGoal(turtlesim::Pose goal_pose, double distance_tolerance, double tdepz){
	geometry_msgs::Twist vel_msg;
	double k=1.75;

	ros::Rate loop_rate(10);
	do{
		//Velocidad lineal. 
		vel_msg.linear.x =k*((getDistance(turtlesim_pose.x, turtlesim_pose.y, goal_pose.x, goal_pose.y))/tdepz);
		vel_msg.linear.y = 0;
		vel_msg.linear.z = 0;
		//Velocidad angular.
		vel_msg.angular.x = 0;
		vel_msg.angular.y = 0;
		vel_msg.angular.z = 4*(atan2(goal_pose.y - turtlesim_pose.y, goal_pose.x - turtlesim_pose.x)-turtlesim_pose.theta);

		velocity_publisher.publish(vel_msg);

		ros::spinOnce();
		loop_rate.sleep();

	}
	while(getDistance(turtlesim_pose.x, turtlesim_pose.y, goal_pose.x, goal_pose.y)>distance_tolerance);
	cout<<"end move goal"<<endl;
	vel_msg.linear.x = 0;
	vel_msg.angular.z = 0;
	velocity_publisher.publish(vel_msg);

}

//Calcula la distancia de la posición actual a la final.
double getDistance(double x1, double y1, double x2, double y2){
	return sqrt(pow((x2-x1),2) + pow((y2-y1),2));
}
