#include"ros/ros.h"

int main(int argc, char  *argv[])
{
    ros::init(argc, argv, "param_node");
    ros::NodeHandle nh;
    nh.setParam("rc4_key", "ROS");
    return 0;
}
