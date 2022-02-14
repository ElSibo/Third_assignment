#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Point32.h"
#include "geometry_msgs/Twist.h"


using namespace std;
float a, b,z;
ros::Publisher pub1;
void vel(const geometry_msgs::Point32::ConstPtr& msg){

	a=msg->x;
	b=msg->y;
	z=msg->z;
}
void assistContr(const sensor_msgs::LaserScan::ConstPtr& msg){
	float Rt=0,ft=0,Lt=0;
	
	
	
	for(int i=0;i<260;i++){ Rt = Rt + msg->ranges[i]; }
	//do the average for all right data
	Rt= Rt/260;
	//cout<<"right : "<<Rt<<endl;
	
	for(int i=280;i<440;i++){ ft = ft + msg->ranges[i]; }
	//do the average for all front data
	ft= ft/200;
	//cout<<"front :        "<<ft<<endl;
	
	for(int i=460;i<720;i++){ Lt = Lt + msg->ranges[i]; }
	//do the average for all left data
	Lt= Lt/260;
	//cout<<"left :  "<<Lt<<endl;
	
	if(z>=2.0){
	if (ft<0.5 & a>0){ a=0; }
	if (Rt<0.7 & b>0){ b=0; }	//attivation of assistent controller
	if (Lt<0.7 & b<0){ b=0; }}
	if(z>0){
	geometry_msgs::Twist my_vel;
	my_vel.linear.x =a;
	my_vel.angular.z = b;  	//publication on cdm_vell the velocity(linear and angular) of the robot
	pub1.publish(my_vel);}		//if z<0 that mean the go_to_point active



}


ros::Publisher pub; //declared the pulisher for the same reason 
int main(int argc, char **argv){ //main
	
	
	
	ros::init(argc,argv,"contmove_rob"); // defined the node contmove_rob
	ros::NodeHandle nn2;	//defined the nodehandler  
	pub1 = nn2.advertise<geometry_msgs::Twist>("/cmd_vel",1);//defined the pulisher
	
	while(ros::ok()){
	
	ros::Subscriber sub2 = nn2.subscribe("/vel_cont",1,vel); //defined and callingthe subscriber
	ros::Subscriber sub = nn2.subscribe("/scan",1,assistContr);
	ros::spin();
	}
	
}

