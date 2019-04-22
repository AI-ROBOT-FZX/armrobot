#include "ros/ros.h"
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include "sensor_msgs/JointState.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cstddef>
using std::string;
using std::stringstream;
#define M_PI 3.1415926
#define max 100
int range_tf(float str1,float str2,float str3,float str4,float str5);
void jointstatesCallback(const sensor_msgs::JointStateConstPtr& msg);
string to_String(int n);


char ConvertHexChar(char ch);

serial::Serial ros_ser;       //声明串口对象

//回调函数
void jointstatesCallback(const sensor_msgs::JointStateConstPtr& msg)
{
	stringstream str;
    
	str<<"link1_joint: pos:"<<msg->position[0]
	   <<"link2_joint: pos:"<<msg->position[1]
	   <<"link3_joint: pos:"<<msg->position[2]
       <<"link4_joint: pos:"<<msg->position[3]
	   <<"link5_joint: pos:"<<msg->position[4]
	   <<"plat_joint: pos:"<<msg->position[5]<<std::endl;
     ROS_INFO_STREAM("Writing to serial port" );
     ROS_INFO_STREAM(range_tf(1000,2500,M_PI/2,-M_PI/2,msg->position[0]));
     ROS_INFO_STREAM(range_tf(1300,2400,M_PI/2,-M_PI/2,msg->position[1])); 
     ROS_INFO_STREAM(range_tf(900,2000,M_PI/2,-M_PI/2,msg->position[2]));
     ROS_INFO_STREAM(range_tf(600,2400,M_PI/2,-M_PI/2,msg->position[3]));
     ROS_INFO_STREAM(range_tf(600,1800,M_PI/2,-M_PI/2,msg->position[4]));
     ROS_INFO_STREAM(range_tf(900,2000,M_PI/2,-M_PI/2,msg->position[5]));
        
    string start="{";
    string ID0="#000P";
    string ID1="#001P";
    string ID2="#002P";
    string ID3="#003P";
    string ID4="#004P";
    string ID5="#005P";
    string TIM0="T100!";
    string end="}";

    string P0=to_String(range_tf(1000,2500,M_PI/2,-M_PI/2,msg->position[0]));
    string P1=to_String(range_tf(1300,2400,M_PI/2,-M_PI/2,msg->position[1]));
    string P2=to_String(range_tf(900,2000,M_PI/2,-M_PI/2,msg->position[2]));
    string P3=to_String(range_tf(600,2400,M_PI/2,-M_PI/2,msg->position[3]));
    string P4=to_String(range_tf(600,1800,M_PI/2,-M_PI/2,msg->position[4]));
    string P5=to_String(range_tf(900,2000,M_PI/2,-M_PI/2,msg->position[5]));
    
    string value = start+ID0+P0+TIM0+ID1+P1+TIM0+ID2+P2+TIM0+ID3+P3+TIM0+ID4+P4+TIM0+ID5+P5+TIM0+end+"0";
    
    ROS_INFO_STREAM(value);
    ros_ser.write(value);
}

int range_tf(float str1,float str2,float str3,float str4,float str5)
{

    float sl = str1;
    float sh = str2;
    float al = str3;
    float ah = str4;
    float ip = str5;
    int out=int(((ip-al)*(sh-sl))/(ah-al)+sl);
    
    return out;
}
string to_String(int n)
  {
     int m = n;
     char s[max];
     char ss[max];
     int i=0,j=0;
     if (n < 0)// 处理负数
     {
         m = 0 - m;
         j = 1;
         ss[0] = '-';
     }    
     while (m>0)
     {
         s[i++] = m % 10 + '0';
         m /= 10;
     }
     s[i] = '\0';
     i = i - 1;
     while (i >= 0)
     {
         ss[j++] = s[i--];
     }    
     ss[j] = '\0';    
     return ss;
 }
/******************************************
                    转换函数
******************************************/

int main (int argc, char** argv)
{
     ros::init(argc, argv, "armrobot_serial_control");  //初始化节点
     ros::NodeHandle n;                        //声明节点句柄
     //订阅主题/joint_states，并配置回调函数
     ros::Subscriber command_sub = n.subscribe("/joint_states", 1000, jointstatesCallback);
     //发布主题sensor
     ros::Publisher sensor_pub = n.advertise<std_msgs::String>("sensor", 1000);
 
     try
     {
         //设置串口属性，并打开串口
         ros_ser.setPort("/dev/ttyUSB0");
         ros_ser.setBaudrate(115200);
         serial::Timeout to = serial::Timeout::simpleTimeout(1000);
         ros_ser.setTimeout(to);
         ros_ser.open();
     }
     catch (serial::IOException& e)
     {
         ROS_ERROR_STREAM("Unable to open port ");
         return -1;
     }
 
     //检测串口是否已经打开，并给出提示信息 
     if(ros_ser.isOpen()){
         ROS_INFO_STREAM("Serial Port opened");
     }
     else
        {
         return -1;
        }
 
     //指定循环的频率
     ros::Rate loop_rate(10);
 
     while(ros::ok()){
 
         //处理ROS的信息，比如订阅消息,并调用回调函数
         ros::spinOnce();
 
         if(ros_ser.available())
         {
             ROS_INFO_STREAM("Reading from serial port");
             std_msgs::String serial_data;
             //获取串口数据

             serial_data.data = ros_ser.read(ros_ser.available());
             ROS_INFO_STREAM("Read: " << serial_data.data);
             //将串口数据发布到主题sensor
             sensor_pub.publish(serial_data);
         }
         loop_rate.sleep();
     }
 }
