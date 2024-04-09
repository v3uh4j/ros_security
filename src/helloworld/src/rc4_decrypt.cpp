#include <iostream>
#include <string>
#include <vector>
#include<sstream>
#include<std_msgs/String.h>
#include "ros/ros.h"

using namespace std;

// RC4 加密算法
string rc4_encrypt(string plaintext, string key) {
    int s[256];
    unsigned char k[256];
    string ciphertext;

    // 初始化 S-Box
    for (int i = 0; i < 256; i++) {
        s[i] = i;
        k[i] = key[i % key.length()];
    }

    // 混淆 S-Box
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + s[i] + k[i]) % 256;
        swap(s[i], s[j]);
    }

    // 加密
    int i = 0;
    j = 0;
    for (char c : plaintext) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;
        swap(s[i], s[j]);
        unsigned char t = (c ^ s[(s[i] + s[j]) % 256]);
        ciphertext += t;
    }

    return ciphertext;
}

// RC4 解密算法
string rc4_decrypt(string ciphertext, string key) {
    return rc4_encrypt(ciphertext, key);
}
// void hexdump(const std::string& input) {
//     int length = input.length();
//     const unsigned char* puc_data = reinterpret_cast<const unsigned char*>(input.c_str());

//     for (int i = 0; i < length; i++) {
//         printf("%02X", puc_data[i]);
//         if (i && (i + 1) % 16 == 0) {
//             putchar('\n');
//         }
//     }
//     printf("\n");
// }
std::string hexdump(const std::string& input) {
    int length = input.length();
    const unsigned char* puc_data = reinterpret_cast<const unsigned char*>(input.c_str());
    std::ostringstream oss;

    for (int i = 0; i < length; i++) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(puc_data[i]);
        if (i && (i + 1) % 16 == 0) {
            oss << "\n";
        }
    }
    oss << "\n";

    return oss.str();
}
// string hexToString(const std::string& hexDump) {
//     std::stringstream ss;
//     std::string result;

//     for (size_t i = 0; i < hexDump.length(); i += 2) {
//         std::string byteStr = hexDump.substr(i, 2);
//         char byte = static_cast<char>(std::stoi(byteStr, nullptr, 16));
//         ss << byte;
//     }

//     return ss.str();
// }
std::string hexToString(const std::string& hexDump) {
    std::stringstream ss;
    std::string result;

    for (size_t i = 0; i < hexDump.length(); i += 2) {
        std::string byteStr = hexDump.substr(i, 2);
        std::stringstream byteStream;
        byteStream << std::hex << byteStr;
        int byte;
        byteStream >> byte;
        result.push_back(static_cast<char>(byte));
    }

    return result;
}
void doMsg(const std_msgs::String::ConstPtr& msg, string ros_key, ros::Publisher pub) {
    cout << "Plaintext: " << msg->data << endl; 
    // string decrypted = rc4_decrypt(ciphertext, ros_key);
    // cout << "Decrypted: " << decrypted << endl;
    std_msgs::String decrypted_msg;
    //string hexstring = hexToString(msg->data);
    // cout << "hexstring:"<< hexstring << endl;
    decrypted_msg.data = rc4_decrypt(msg->data,ros_key);
    cout << "decrypted: " << decrypted_msg.data<< endl;
    // 发布加密后的消息到"encrypt"话题
    pub.publish(decrypted_msg);
}
int main(int argc, char **argv) {
    // 初始化 ROS 节点
    ros::init(argc, argv, "rc4_decryptnode");
    ros::NodeHandle nh;

    // 获取 ROS 密钥
    string ros_key;
    nh.getParam("rc4_key", ros_key);
    //实例化 订阅者对象
    ros::Publisher pub = nh.advertise<std_msgs::String>("decrypt_msg", 100);
    ros::Subscriber sub = nh.subscribe<std_msgs::String>("encrypt_msg",100, boost::bind(&doMsg, _1, ros_key, pub));
    
    ros::spin();
    // 示例用法
    // string plaintext = "Hello, World!";
    // string ciphertext = rc4_encrypt(plaintext, ros_key);
    // string decrypted = rc4_decrypt(ciphertext, ros_key);

    // hexdump(ciphertext); 
    // cout << "Decrypted: " << decrypted << endl;
    // //ROS_INFO("Plaintext:%s",plaintext);

    return 0;
}