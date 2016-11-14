#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "my_filter");

    //My_Filter filter;

    ros::spin();

    return 0;
}