#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <iostream>
#include <stdio.h>
#include <std_msgs/Int16.h>

//using namespace Eigen;

//For geometry_msgs::Twist using:
// 		dummy.linear.x
// 		dummy.linear.y
// 		dummy.angular.z
geometry_msgs::Twist robot_position;
geometry_msgs::Twist velocity_msg;
geometry_msgs::Pose2D vel;

ros::ServiceClient clientSteer;
//gazebo_msgs::GetJointProperties joint_msg[1];
int *motor_values;

//rate_hz assignment
double rate_hz = 5;

// Transform robot velocities (x,y,w) to motor velocities (m1,m2,m3,m4)
void getMotorValue(double x_velocity, double y_velocity, double w_velocity){
    
    // double R = 1;
    double r = 3.4;
    
    // double Rx = 
    double velWMod = 0.0;

    double factor_velocidad = (0.02)/2;
    
	//commented the following line just to compile in car, something else should replace it
    //clientSteer.call(joint_msg[0]);
	//int angulo = (int)w_velocity*180/M_PI;
	int angulo = w_velocity;
	if(-30 < angulo && angulo < 30)
	{
		angulo += 30;
		motor_values[1] = angulo;
	} 	

   /* if(joint_msg[0].response.success == 1) {
    }*/
    
    double velMots[3];
    
    printf("\nvelocidad: %f, angulo: %d", x_velocity/6, angulo);
    
    velMots[0] = x_velocity*factor_velocidad;
    velMots[1] = x_velocity*factor_velocidad;
    
    velMots[2] = velWMod;
	x_velocity = x_velocity / 6;
	if(x_velocity < 700) 
	{
    		motor_values[0] = 200; //x_velocity;
	} else {
		ROS_INFO_STREAM("Vel mayor a 700");
	}
    //return velMots;
}

void get_vel_vec(const geometry_msgs::Pose2D& msg) {
	vel.x = msg.x;
	vel.y = msg.y;
	vel.theta = msg.theta; 
}


int main(int argc, char **argv){
	ros::init(argc,argv,"auto_velocity_node");
	ros::NodeHandle nh;
	ROS_INFO_STREAM("auto_velocity_node initialized");
	ROS_INFO_STREAM(ros::this_node::getName());

	std::string nombre;
	motor_values = new int[2];
/*	const std::string PARAM1 = "~lidar";
	bool okx = ros::param::get(PARAM1, nombre);
	if(!okx) {
		ROS_FATAL_STREAM("No se pudo obtener el parametro " << PARAM1);
		exit(1);
	}
	else
		std::cout<<"\nSolo concentrarse en lidar:"<<nombre;
*/	
	// Suscribe to Gazebo service ApplyJointEffor
//	ros::ServiceClient client = nh.serviceClient<gazebo_msgs::ApplyJointEffort>("/gazebo/apply_joint_effort");
//	gazebo_msgs::ApplyJointEffort eff_msg[3];
	int eff_msg[3];	
	// Suscribe to Gazebo service get_joint_properties
//	clientSteer = nh.serviceClient<gazebo_msgs::GetJointProperties>("/gazebo/get_joint_properties");	
//	joint_msg[0].request.joint_name = nombre+"::steer_joint";
	
	ros::Subscriber sub_vel = nh.subscribe("/target_vel_topic", 100, &get_vel_vec);

	//publisher to control motor for linear movment of the car	
//	const std::string topic_ctrl = "manual_control/speed";
//	uint32_t queue_size = 1;
//	ros::Publisher pub = nh.advertise (topic_ctrl, queue_size);
	ros::Publisher pub = nh.advertise<std_msgs::Int16>(nh.resolveName("/manual_control/speed"), 1);
	ros::Publisher pub_ste = nh.advertise<std_msgs::Int16>(nh.resolveName("/manual_control/steering"), 1);
		

	double tiempo = 0;

    	//define the max speed
	double cruise_speed = 50;

	//define the rate
	ros::Rate rate(rate_hz);
	ros::Time start_time ;
	ros::Duration duration ;

	double effort[3];
	
	while (ros::ok())
	{
	getMotorValue(vel.x, vel.y, vel.theta);
	/*	double* velMots = getMotorValue(velocity_msg.linear.x,velocity_msg.linear.y,velocity_msg.angular.z);
		effort [0] = velMots[0];
		effort [1] = velMots[1];
		effort [2] = velMots[2];
*/
		std_msgs::Int16 value_motor;
		std_msgs::Int16 value_ste;
		value_motor.data = motor_values[0];
		value_ste.data = motor_values[1];
		pub.publish(value_motor); 
		pub_ste.publish(value_ste); 
 
		ROS_INFO_STREAM("vals: (" << value_motor.data << " , " << value_ste.data << " )");	
	/*if(client.exists()){
			
			start_time.sec = 0;
			start_time.nsec = 0;
			duration.sec = 0;
			duration.nsec = (1/rate_hz)*pow(10,9);
			
			printf("\n effort: %f %f %f", effort[0],effort[1],effort[2]);
			
			// Wheel-Joint 1
			eff_msg[0].request.joint_name = nombre+"::back_right_wheel_joint";
			eff_msg[0].request.duration = duration;
			eff_msg[0].request.effort = effort[0];
			eff_msg[0].request.start_time = start_time;

			// Wheel-Joint 2
			eff_msg[1].request.joint_name = nombre+"::back_left_wheel_joint";
			eff_msg[1].request.duration = duration;
			eff_msg[1].request.effort = effort[1];
			eff_msg[1].request.start_time = start_time;

			// Wheel-Joint 3
			eff_msg[2].request.joint_name = nombre+"::steer_joint";
			eff_msg[2].request.duration = duration;
			eff_msg[2].request.effort = effort[2];
			eff_msg[2].request.start_time = start_time;

			client.call(eff_msg[0]);
			client.call(eff_msg[1]);
			client.call(eff_msg[2]);

			ROS_INFO_STREAM("\nJoints ==> 1: " << ((eff_msg[0].response.success == 1) ? "TRUE" : "FALSE") << " 2: " << ((eff_msg[1].response.success == 1) ? "TRUE" : "FALSE") << " 3: " << ((eff_msg[2].response.success == 1) ? "TRUE" : "FALSE") );
		}*/																																																																																												
		
		ros::spinOnce();
		rate.sleep();
    }
    return 0;
}
