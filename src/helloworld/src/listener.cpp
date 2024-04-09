#include <iostream>
#include <string>
#include <vector>
#include<sstream>
#include<std_msgs/String.h>
#include "ros/ros.h"
using namespace std;

void doMsg(const std_msgs::String::ConstPtr& msg) {
    std::cout << msg->data << std::endl;
}
int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "listener");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe<std_msgs::String>("decrypt_msg", 100, doMsg);

    ros::spin();
    return 0;
}
