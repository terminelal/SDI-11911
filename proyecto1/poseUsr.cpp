#include <ros/ros.h>
#include <turtlesim/Pose.h>
#include <geometry_msgs/Twist.h>
#include <iomanip>
#include <stdio.h>
#include <math.h> 
#include <time.h> 

const int RATE = 20;
const double THOLD_D = 0.1;
const double THOLD_A = 0.1;
double x,y, theta,vel;
int seg, updater;
clock_t  start_s,stop_s;
//Objeto pub
ros::Publisher pub;

void poseMessageReceived(const turtlesim::Pose& msg){
	updater++;	
	double distancia;
	if(vel < 0 ){
		distancia = sqrt ((x-msg.x)*(x-msg.x)+(y-msg.y)*(y-msg.y));
		vel = distancia / seg;
		//debug
		//ROS_INFO_STREAM("La velocidad es: " << vel << " distancia " << distancia <<" tiempo "<< seg);
		ROS_INFO_STREAM(std::setprecision(2)<< std::fixed << "La pose inicial es = ("<< msg.x <<","<<msg.y <<") direccion=" << msg.theta);	
		start_s=clock();	
	}
	distancia = sqrt ((x-msg.x)*(x-msg.x)+(y-msg.y)*(y-msg.y));
	if(!(updater % RATE))
		ROS_INFO_STREAM(std::setprecision(2)<< std::fixed << "La pose actual es = ("<< msg.x <<","<<msg.y <<") direccion=" << msg.theta << " distancia al obj: "<< distancia);
	if( distancia > THOLD_D ){
		geometry_msgs::Twist pubmsg;
		pubmsg.linear.x = vel;
		pubmsg.angular.z = atan2f( y-msg.y,x-msg.x) - msg.theta;
		//Publicar
		pub.publish(pubmsg);
	}
	else{
		//ROS_INFO_STREAM("rotando " << fabs(msg.theta - theta)  <<"_______"<< msg.theta );
		if(fabs(msg.theta - theta) > THOLD_A ) {
			geometry_msgs::Twist pubmsg;
			pubmsg.linear.x = 0;
			pubmsg.angular.z = fabs(msg.theta - theta);	
			//Publicar
			pub.publish(pubmsg);
		}	else {
			  stop_s=clock();
				printf("tiempo final: %.2lf \n",((double)(stop_s-start_s)*1000)/(CLOCKS_PER_SEC*10));
				ros::shutdown();	
			}
	}
	
}


int main(int argc, char **argv){
	//Inicializacion y nodo
	ros::init(argc,argv,"pose_of_user");
	ros::NodeHandle nh;
	
	vel = -1;
	updater = 0;
	
	pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",1000);
	//Loop at 10hz until the node is shutdown
	ros::Rate rate(10);

	//Preguntar por pose usuario
	printf("\nIngrese pose final: ");
	printf("\nValor en X: ");
	scanf("%lf",&x);
	printf("Valor en Y: ");
	scanf("%lf",&y);
	printf("Angulo: ");
	scanf("%lf",&theta);
	printf("Ingrese tiempo de ejecucion (segundos): ");
	scanf("%d",&seg);
	int flag=1;
	while(seg < 6 && flag){ 
		printf("Valores menores a 6 segundos producen \nun comportamiento erratico por la velocidad desea continuar? [y/n]: ");
		char c; 
		scanf(" %c",&c);
		if(c == 'Y' || c == 'y')
			flag =0;
		else {
			printf("Ingrese tiempo de ejecucion (segundos): ");
			scanf("%d",&seg);
			if(seg >= 6)
				flag = 0;
		}
			
	}
	
	//debugging code
	//printf("Tus valores son x: %lf y: %lf theta: %lf y t: %lf vel\n",x,y,theta,vel);

	//quitamos un segundo para la orientación final
	seg--; 
	//Objecto sub
	ros::Subscriber sub = nh.subscribe("turtle1/pose",1000,&poseMessageReceived);
	//PRINT INITIAL POSE
	while(ros::ok()){
		//Esperar al siguiente momento de otra iteracion
		ros::spinOnce();
		rate.sleep();
	}
  return 0;
}
