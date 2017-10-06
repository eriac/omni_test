#include "ros/ros.h"
  
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"

#include "math.h"
#include <sstream>
#include <string>


geometry_msgs::Twist twist_last;
bool twist_enable;
void twist_callback(const geometry_msgs::Twist& twist_msg){
	twist_last=twist_msg;
	twist_enable=true;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "gun_driver");
	ros::NodeHandle n;
	ros::NodeHandle pn("~");
	//publish
	ros::Publisher yaw_pub = n.advertise<std_msgs::Float64>("/yaw", 1000);
	ros::Publisher pitch_pub = n.advertise<std_msgs::Float64>("/pitch", 1000);
	//Subscribe
	ros::Subscriber joy_sub     = n.subscribe("/twist", 10, twist_callback); 

	float yaw_velocity=1.0;
	float pitch_velocity=1.0;
	float yaw_lower_limit=-1.0;
	float yaw_upper_limit=1.0;
	float pitch_lower_limit=-1.0;
	float pitch_upper_limit=1.0;

	pn.getParam("yaw_lower_limit", yaw_lower_limit);
	pn.getParam("yaw_upper_limit", yaw_upper_limit  );
	pn.getParam("pitch_lower_limit", pitch_lower_limit);
	pn.getParam("pitch_upper_limit", pitch_upper_limit  );
	pn.getParam("yaw_velocity",   yaw_velocity);
	pn.getParam("pitch_velocity", pitch_velocity);
		
	float dt=1.0/20;
	ros::Rate loop_rate(20); 
	while (ros::ok()){
		if(twist_enable){
			static float yaw=0.0;
			static float pitch=0.0;
			float temp1=yaw+yaw_velocity*twist_last.angular.z*dt;
			if(yaw_lower_limit<=temp1 && temp1<=yaw_upper_limit){
				yaw=temp1;
			}
			float temp2=pitch+pitch_velocity*twist_last.angular.y*dt;
			if(pitch_lower_limit<=temp2 && temp2<=pitch_upper_limit){
				pitch=temp2;
			}
			std_msgs::Float64 data[2];
			data[0].data=yaw;
			data[1].data=pitch;
			yaw_pub.publish(data[0]);
			pitch_pub.publish(data[1]);
		}
		
		ros::spinOnce();
		loop_rate.sleep();
	} 
 	return 0;
}

