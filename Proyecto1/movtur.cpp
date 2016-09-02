#include <ros/ros.h>
#include <geometry_msgs/Twist.h> 
#include <turtlesim/Pose.h>
#include <iomanip>
#include <stdlib.h>
#include <sstream> // Allow string streams

using namespace std; // Simplify the use of the terminal in the code

// Declare topics
ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;

// Declare auxiliar methods
void getDesiredPose(turtlesim::Pose goal_pose, double desired_time, double distance_tolerance);
double getDistance(double x1, double y1, double x2, double y2);
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);
void rotate(double angle, bool clockwise, double desired_rotate_time);
double getDesiredDirection(double desired_direction);
double deg2rad(double angle);

int main(int argc, char **argv) {
    // Init ros
	ros::init(argc, argv, "Move_my_turtle");
	ros::NodeHandle nh;

    // Declare publisher and subscriber
	velocity_publisher = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
	pose_subscriber = nh.subscribe("/turtle1/pose", 10, poseCallback);
    
    // Declare useful variables
	ros::Rate loop_rate(0.5);
	turtlesim::Pose desired_pose;
	double desired_time;
	double angle_deg;

    // Init communication with terminal
	cout<<"enter x coordinate: ";
	cin>>desired_pose.x;
	cout<<"enter y coordinate: ";
	cin>>desired_pose.y;
	cout<<"enter angle in degrees: ";
	cin>>angle_deg;
	cout<<"enter time: ";
	cin>>desired_time;

    // Change degrees to radians
	desired_pose.theta = deg2rad(angle_deg);
    // Call the principal method
	getDesiredPose(desired_pose, desired_time, 0.01);

    // Stop
	loop_rate.sleep();	

	ros::spin();

	return 0;

}

// Principal method
void getDesiredPose(turtlesim::Pose desired_pose, double desired_time, double dist_tol){

    // Get the specific attribute for the velocity (movement) of the turtle
	geometry_msgs::Twist vel_msg;

    // Auxiliar variables
	double t0 = ros::Time::now().toSec();
	double dt = 0;

    // Set the rate at 10Hz
	ros::Rate rate(10);

	do {
        // Set our difference of time between the desired one and the time that has passed
		dt = desired_time - (ros::Time::now().toSec() - t0);
        
        // Use of Proportional Control for the speed just like cars
		// Set the linear velocity
            // Velocity = Distance / time
		vel_msg.linear.x = 1.5*getDistance(turtlesim_pose.x, turtlesim_pose.y, desired_pose.x, desired_pose.y) / dt;
		vel_msg.linear.y = 0;
		vel_msg.linear.z = 0;
		// Set theangular velocity
		vel_msg.angular.x = 0;
		vel_msg.angular.y = 0;
            // Steering angle = arctan( (desired y - actual y) / (desired x - actual x) ) - actual direction angle
		vel_msg.angular.z = 4*(atan2(desired_pose.y - turtlesim_pose.y, desired_pose.x - turtlesim_pose.x)-turtlesim_pose.theta);

        // Publish the velocity we had set
		velocity_publisher.publish(vel_msg);

		ros::spinOnce();
		rate.sleep();

	}while((getDistance(turtlesim_pose.x, turtlesim_pose.y, desired_pose.x, desired_pose.y)>dist_tol) && (dt > 0));
    // Repeat until the distance reached and the desired one is equal or less than 0.01 (our distance tolerance),
    // AND our difference of time is equal or less than 0
    
    // Stop the movement
	vel_msg.linear.x = 0;
	vel_msg.angular.z = 0;
    // Publish that the turtle must stop now
	velocity_publisher.publish(vel_msg);

    // Get the specific direction of the pose
	getDesiredDirection(desired_pose.theta);
}

// Method that gets the euclidean distances between two points
double getDistance(double x1, double y1, double x2, double y2){
	return sqrt(pow((x2-x1),2) + pow((y2-y1),2));
}

// Method that retrieves the actual pose
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
	turtlesim_pose.theta=pose_message->theta;
}

// Method that makes the turtle rotate
void rotate (double angular_speed, double rotate_angle, bool clockwise){

    // Get the specific attribute for the velocity (movement) of the turtle
	geometry_msgs::Twist vel_msg;
	   // Set the linear velocity (in this case, all are 0)
	   vel_msg.linear.x =0;
	   vel_msg.linear.y =0;
	   vel_msg.linear.z =0;
	   // Set the angular velocity
	   vel_msg.angular.x = 0;
	   vel_msg.angular.y = 0;
	   if (clockwise) // Check if the rotation is positive or negative
	   	vel_msg.angular.z =-abs(angular_speed);
	   else
	   	vel_msg.angular.z =abs(angular_speed);

       // Auxiliar variables
	   double t0 = ros::Time::now().toSec();
       double t1 = 0;
	   double current_angle = 0.0;
       // Set rate at 1kHz to avoid errors
	   ros::Rate loop_rate(1000);
	   do{
           // Set the velocity
		   velocity_publisher.publish(vel_msg);
		   t1 = ros::Time::now().toSec();
		   current_angle = angular_speed * (t1-t0); // Distance = Velocity * Times
		   ros::spinOnce();
		   loop_rate.sleep();
		   
	   }while(current_angle<rotate_angle);
       // Repeat until the angle rotated is equal to the desired one
    
       // Stop rotating and make the turtle knows it
	   vel_msg.angular.z =0;
	   velocity_publisher.publish(vel_msg);
}

// Method that calls rotation and decides if positive or negative rotation is better
double getDesiredDirection(double desired_direction)
{
	double rotate_angle = desired_direction - turtlesim_pose.theta;
	bool clockwise = ((rotate_angle<0)?true:false);
	rotate (abs(rotate_angle), abs(rotate_angle), clockwise);
}

// Method that converts degrees to radians
double deg2rad(double angle){
	double pi = 3.14159265359; // Pi value retrieved from Google
	return angle *pi /180.0;
}
