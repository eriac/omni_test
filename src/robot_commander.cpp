#include "ros/ros.h"
  
#include "math.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/JointState.h"
#include "sensor_msgs/Joy.h"

#include <string>
#include <iostream>
#include <sstream>

#define PS3_Select 0
#define PS3_L3     1
#define PS3_R3     2
#define PS3_Start  3
#define PS3_Up     4
#define PS3_Right  5
#define PS3_Down   6
#define PS3_Left   7
#define PS3_L2     8
#define PS3_R2     9
#define PS3_L1    10
#define PS3_R1    11
#define PS3_Triangle 12
#define PS3_circle   13
#define PS3_cross    14
#define PS3_square   15
#define PS3_PS    16

float input[6]={0};
ros::Publisher move_twist_pub;
ros::Publisher gun_twist_pub;
ros::Publisher gun_command_pub;

void joy_callback(const sensor_msgs::Joy& joy_msg){
	if(!joy_msg.buttons[PS3_L1]){
		geometry_msgs::Twist twist_data;
		twist_data.linear.x =joy_msg.axes[1];
		twist_data.linear.y =joy_msg.axes[0];
		twist_data.angular.z=joy_msg.axes[2];
		move_twist_pub.publish(twist_data);
		printf("x:%f, y:%f, rz:%f\n",
		  twist_data.linear.x,twist_data.linear.y,twist_data.angular.z);
	}
	else{
		geometry_msgs::Twist twist_data;
		twist_data.angular.z =joy_msg.axes[2];
		twist_data.angular.y =joy_msg.axes[3];
		gun_twist_pub.publish(twist_data);
	}
}
int main(int argc, char **argv)
{
	ros::init(argc, argv, "twist_commander");
	ros::NodeHandle n;
	
	move_twist_pub  = n.advertise<geometry_msgs::Twist>("/move_twist", 1000); 
	gun_twist_pub   = n.advertise<geometry_msgs::Twist>("/gun_twist", 1000); 
	gun_command_pub = n.advertise<std_msgs::String>("/gun_command", 1000); 
	ros::Subscriber joy_sub   = n.subscribe("/joy", 10, joy_callback); 

	ros::Rate loop_rate(50); 
	while (ros::ok()){		
		ros::spinOnce();
		loop_rate.sleep();
	} 
 	return 0;
}

