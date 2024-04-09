#include"ros/ros.h"
#include<std_msgs/String.h>
#include"iostream"
#include"string"
int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "talker");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<std_msgs::String>("chatter", 100);

    std_msgs::String msg ;
    msg.data = "hello world";
    ros::Rate r(1);
    while(ros::ok()) {
        pub.publish(msg);
        r.sleep();
        std::cout<<"发送的消息："<<msg;
        ros::spinOnce();
    }
    return 0;
}
