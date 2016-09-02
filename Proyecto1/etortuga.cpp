#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <sstream>
#include <turtlesim/Spawn.h>

//Enrique Jose Padilla Terrones, 142685.
//Robotica, ITAM.
using namespace std;

ros::Publisher publicaV; //Publisher dentro de TurtleSim, determina la velocidad de la tortuga.
ros::Subscriber suscribePose; //Subscriber de la pose de la tortuga, para conocerla en todo momento.
turtlesim::Pose poseTortuga; //Variable del tipo Pose, propia de la tortuga del simulador. 

const double pi = 3.14159265359;

//Declaramos los m'etodos a usar en el ejecutable. 
double convGradRad(double angulo); //Conversi'on de rados a radianes.
double giraObjetivo(double faseObjetivo); //Rotaci'on de la tortuga a un 'angulo especificado por el usuario. 


void poseCallback(const turtlesim::Pose::ConstPtr & pose_message); //Funci'on de callback ejecutada cuando se publica un mensaje en la pose de la tortuga.
void tortugaAObjetivo(turtlesim::Pose poseObjetivo, double tolerancia, double t); //M'etodo principal del ejecutable, mueve la tortuga a la pose deseada.
double calculaDistancia(double x1, double y1, double x2, double y2); //F'ormula simple de distancia entre dos puntos.


int main(int argc, char **argv) 
{
    
    ros::init(argc, argv, "Trayectoria_de_tortuga");
    ros::NodeHandle n;
    //Declaraci'on de variables. 
    double vel, velocidadAngular;
    double distancia, angle, posX, posY;
    bool continuar;
    double t, ang;

    //Comandos para publicaci'on y subscripci'on de la tortuga.
    publicaV = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
    suscribePose = n.subscribe("/turtle1/pose", 10, poseCallback); 
    ros::Rate loop_rate(10); //Define la frecuencia a la que das los msg.

    ROS_INFO("\n\n\n ********EMPIEZA A PROBAR, MARCO.*********\n");
    continuar = true;
    //Ciclo para asegurar la ejecuci'on del c'odigo hasta que el usuario ya no desee continuar.
    while(continuar){
    //Lectura de variables proporcionadas por el usuario. 
    cout<<"Posición Objetivo X: ";
    cin>>posX;
    cout<<"Posición Objetivo Y: ";
    cin>>posY;
    cout<<"Tiempo de ejecución: ";
    cin>>t;
    cout<<"Ángulo Objetivo: ";
        cin>>ang;

    turtlesim::Pose poseObjetivo;
    poseObjetivo.x = posX;
    poseObjetivo.y = posY;
    poseObjetivo.theta = 15;
    //Mando llamar los m'etodos necesarios para el movimiento de la tortuga
    tortugaAObjetivo(poseObjetivo, 1, t);
    giraObjetivo(convGradRad(ang));

    //Preguntar al usuario si desea continuar ejecutando el programa. 
    cout<<"¿Continuar ejecución?";
        cin>>continuar;

    }
    loop_rate.sleep();

        ros::spin();

        return 0;

}

//M'etodos y funciones.
double convGradRad(double angulo){
    return angulo *pi /180.0;
}
void rotate (double velocidadAngular, double anguloRelativo, bool clockwise){

    geometry_msgs::Twist vel_msg;
       //Velocidad lineal aleatoria en x.
       vel_msg.linear.x =0;
       vel_msg.linear.y =0;
       vel_msg.linear.z =0;
       //Velocidad lineal aleatoria en y.
       vel_msg.angular.x = 0;
       vel_msg.angular.y = 0;
       if (clockwise)
           vel_msg.angular.z =-abs(velocidadAngular);
       else
           vel_msg.angular.z =abs(velocidadAngular);

       double t0 = ros::Time::now().toSec();
       double anguloActual = 0.0;
       ros::Rate loop_rate(1000);
       do{
           publicaV.publish(vel_msg);
           double t1 = ros::Time::now().toSec();
           anguloActual = velocidadAngular * (t1-t0);
           ros::spinOnce();
           loop_rate.sleep();
       }while(anguloActual<anguloRelativo);
       vel_msg.angular.z =0;
       publicaV.publish(vel_msg);
}
double giraObjetivo(double faseObjetivo)
{    
    double radianesRel = faseObjetivo - poseTortuga.theta;
    bool clockwise = ((radianesRel<0)?true:false);
    rotate (abs(radianesRel), abs(radianesRel), clockwise);
}

void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
    poseTortuga.x=pose_message->x;
    poseTortuga.y=pose_message->y;
    poseTortuga.theta=pose_message->theta;
}


void tortugaAObjetivo(turtlesim::Pose poseObjetivo, double tolerancia, double t){
    geometry_msgs::Twist vel_msg;
    double k=1.766;

    ros::Rate loop_rate(10);
    do{
        //Velocidad lineal de la tortuga.
        vel_msg.linear.x =k*((calculaDistancia(poseTortuga.x, poseTortuga.y, poseObjetivo.x, poseObjetivo.y))/t);
        vel_msg.linear.y = 0;
        vel_msg.linear.z = 0;
        //Velocidad angular de la tortuga.
        vel_msg.angular.x = 0;
        vel_msg.angular.y = 0;
        vel_msg.angular.z = 4*(atan2(poseObjetivo.y - poseTortuga.y, poseObjetivo.x - poseTortuga.x)-poseTortuga.theta);

        publicaV.publish(vel_msg);

        ros::spinOnce();
        loop_rate.sleep();

    }while(calculaDistancia(poseTortuga.x, poseTortuga.y, poseObjetivo.x, poseObjetivo.y)>tolerancia);
    cout<<"Finalizado"<<endl;
    vel_msg.linear.x = 0;
    vel_msg.angular.z = 0;
    publicaV.publish(vel_msg);

}

double calculaDistancia(double x1, double y1, double x2, double y2){
    return sqrt(pow((x2-x1),2) + pow((y2-y1),2));
}


