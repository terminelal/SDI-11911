// this program publishes randomly-generated velocity 
// messages for turtlesim

#include <ros/ros.h>
#include <geometry_msgs/Twist.h> // for geometry_msgs::Twist
#include <stdlib.h> // for rand() and RAND_MAX
#include <turtlesim/Pose.h>
#include <iomanip> // for std::setprecision and std::fixed
#include <math.h>

ros::Publisher pub;
double x_fin, y_fin, theta_fin, tiempo;
double velocidad_general = 0.0;
bool calcular_velocidad = false;

void poseMessageReceived(const turtlesim::Pose& msg) {
	

	// pose origen, pose destino
	// msg.x, msg.y, msg.theta
	// x_fin, y_fin
	// obtener angulo
	
	double xprima = x_fin - msg.x;
	double yprima = y_fin - msg.y;

	double distance = sqrt(pow(xprima,2)+pow(yprima,2));
	
	ROS_INFO_STREAM("position=(" << msg.x << "," << msg.y << ")" << " direction=" << msg.theta << "fin_x:" << x_fin << "," << "fin_y:"<<y_fin);
	
	// obtener angulo al cual se debe mover para alcanzar coordenadas
	double angle = atan2 (yprima, xprima);
	angle = angle - msg.theta;
	
	double velocidad = velocidad_general;
	// si se requiere linea recta, primero corrige posicion y luego avanza
	// if (angle < 0.1){
		// la velocidad se mantiene, solo se calcula una vez
		if(!calcular_velocidad) {
			velocidad_general = distance / tiempo;
			calcular_velocidad = true;
		}
	// }
	double holgura = 0.5;
	if(distance < holgura) {
		ROS_INFO_STREAM("Llego a (x:"<<x_fin<<",y:"<<y_fin<<") ");
		velocidad = 0;
		double dif = theta_fin - msg.theta;
		if(dif < 0.01 && dif > -0.01) {
			ROS_INFO_STREAM("POSE FINAL ALCANZADA");
			ros::shutdown();
		}
		else {
			// enviar un angulo para buscar la pose final
			angle = theta_fin - msg.theta;	
			ROS_INFO_STREAM("Corrigiendo pose ");
		}
	}
	
	ROS_INFO_STREAM("angle: " << angle << ", velocity:" << velocidad <<", distancia:"<<distance);
	
	// create and fill in the message. the other four 
	// fields, which are ignored by turtlesim, default to 0.
	geometry_msgs::Twist msg2;
	msg2.linear.x = velocidad;
	msg2.angular.z = angle;
	
	// publish the message
	pub.publish(msg2);
	
	// send the message to rosout with the details
	ROS_INFO_STREAM("Sending command:" << " linear= " << msg2.linear.x << " angular=" << msg2.angular.z);
}


int main(int argc, char **argv) {
	// initialize the ROS syste and become a node
	ros::init(argc, argv, "publish_rute");
	ros::NodeHandle nh;

	// create a publisher object
	pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);

	// seed the random number generator
	srand(time(0));

	// read x,y final
	const std::string PARAM1 = "~x_fin";
	const std::string PARAM2 = "~y_fin";
	const std::string PARAM3 = "~theta_fin";
	const std::string PARAM4 = "~tiempo";
	
	bool okx = ros::param::get(PARAM1, x_fin);
	bool oky = ros::param::get(PARAM2, y_fin);
	bool oktheta = ros::param::get(PARAM3, theta_fin);
	bool okvel = ros::param::get(PARAM4, tiempo);
	if(!okx) {
		ROS_FATAL_STREAM("No se pudo obtener el parametro " << PARAM1);
		exit(1);
	}
	if(!oky) {
		ROS_FATAL_STREAM("No se pudo obtener el parametro " << PARAM2);
		exit(1);
	}
	if(!oktheta) {
		ROS_FATAL_STREAM("No se pudo obtener el parametro " << PARAM3);
		exit(1);
	}
	if(!okvel) {
		ROS_FATAL_STREAM("No se pudo obtener el parametro " << PARAM4);
		exit(1);
	}

	ros::Subscriber sub = nh.subscribe("turtle1/pose", 1000, &poseMessageReceived);
	
	// loop at 10Hz until the node is shut down
	ros::Rate rate(10);
	while(ros::ok()) {
		ROS_INFO_STREAM("check 1");		
		
		// let ROS take over
		ros::spinOnce();
		
		// wait until it's time for another iteration
		rate.sleep();
		ROS_INFO_STREAM("check 2");
	}
}
