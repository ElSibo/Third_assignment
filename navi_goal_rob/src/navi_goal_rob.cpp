#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>		//libraries
#include <actionlib/client/simple_action_client.h>
#include "geometry_msgs/Point32.h"
    
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient; //defined action client
ros::Publisher pub; //declared the pulisher  


void goPoint(float n){

   //tell the action client that we want to spin a thread by default
    MoveBaseClient ac("move_base", true);
      ROS_INFO("Waiting for action server to start.");
  // wait for the action server to start
  ac.waitForServer(); //will wait for infinite time

float x,y;

	int cont=0;
	std::cout<<"POINT first x and after y \n ";
	std::cin>>x>>y;
	
	while(cont<=0){
		if((x>0.2 && y>3)	||
		  (x<0.2 && y<-6)	||
		  (0.2<x<2.5 && y>0.2) ||
		  (x<-6.5 || x>6 || y<-8.5 || y>9)){				//control if the point is in the the perimeter
		std::cout<<"coordinate outside of perimeter give me other: ";
		std::cin>>x>>y;
		cont--;}
		cont++;
		}
	

	geometry_msgs::Point32 my_vel;
	my_vel.x =0.0;
	my_vel.y =0.0; 
	my_vel.z=n;			// to stop the controller
	pub.publish(my_vel);	

  
     move_base_msgs::MoveBaseGoal goal;
   
      //we'll send a goal to the robot to move 1 meter forward
     goal.target_pose.header.frame_id = "map";
     goal.target_pose.header.stamp = ros::Time::now();
     goal.target_pose.pose.position.x = x;
     goal.target_pose.pose.position.y = y;			//set the data for the  goal 
     goal.target_pose.pose.orientation.w = 1.0;
  
     ROS_INFO("Sending goal");
    ac.sendGoal(goal);				// sending goal 
    
    std::cout<<"you yant to stop : Y (for yes )";
    char ii;					//asking if user wont to cancel the goal 
    std::cin>>ii;
    if(ii=='y'|| ii=='Y') {ac.cancelGoal();ROS_INFO("Goal has been cancelled");}
    else{
   		ROS_INFO("must be attend 25 second to have the rusult");
     bool finished_before_timeout = ac.waitForResult(ros::Duration(25.0));	//activetion of timeout
     
   if (finished_before_timeout){					// getting result 
     if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("WE are arrive at the GOAL !!");
     else{					
      ROS_INFO("WE DONT arrive in time");
      ac.cancelGoal(); 
      ROS_INFO("Goal has been cancelled");}}}
 }


void controller(float n){

	char a;
	float b=0,c=0,z;
	z=n;
	
	while (1){//loop no endless
	
	
	std::cout<<"enter E:up D:down S:left F:right \n";	//user interface 
	std::cout<<"	r:RESET 	a:EXIT\n";			
	std::cout<<"velocity lin:"<<b<<" ang:"<<c<<std::endl;
	std::cin>>a;
	
	if(a=='e' ||a=='E' ){
		b=b+0.4;
		geometry_msgs::Point32 my_vel;
		my_vel.x =b;
		my_vel.y = c;					//increase linear velocity
		my_vel.z=z;
		pub.publish(my_vel);
	}
	else if(a=='d'||a=='D'){
		b=b-0.4;
		geometry_msgs::Point32 my_vel;
		my_vel.x =b;				//decrease linear velocity
		my_vel.y = c;
		my_vel.z=z;
		pub.publish(my_vel);
	}
	else if(a=='s'||a=='S' ){
		c=c-0.8;
		geometry_msgs::Point32 my_vel;
		my_vel.x =b;
		my_vel.y = c;				 //set and pulish the data to turn left
		my_vel.z=z;
		pub.publish(my_vel);
	}
	else if(a=='f' ||a=='F'){
		c=c+0.8;
		geometry_msgs::Point32 my_vel;
		my_vel.x =b;
		my_vel.y = c;				 //set and pulish the data to turn right
		my_vel.z=z;	
		pub.publish(my_vel);
	}else if(a=='a'||a=='A' ){
	
		break;				//stop the loop
	
	}else if(a=='r'||a=='R' ){
		b=0;
		c=0;
		geometry_msgs::Point32 my_vel;
		my_vel.x =b;
		my_vel.y = c; 				//reset velocity of robot(stop robot)
		my_vel.z=z;
		pub.publish(my_vel);
	
	}
	
	}



}

    
int main(int argc, char** argv){

  	ros::init(argc, argv, "navi_goal_rob");

	ros::NodeHandle nn;	//defined the nodehandler  
	pub = nn.advertise<geometry_msgs::Point32>("/vel_cont",1);//defined the pulisher
	
	
	
	char s;
	while(1){
	
	std::cout<<" Go_to_point:A  Controller:B  Asscontroller:C\n";
	std::cout<<"		TO STOP THE PROGRAM: S \n";			//user interface 
	std::cin>>s;
	if(s=='a' || s=='A'){ goPoint(-1.0);}
	if(s=='b' || s=='B'){ controller(1.0);}
	if(s=='c' || s=='C'){ controller(2.0);}	//call the function choise by user
	if(s=='s' || s=='S'){ break;}
	
  	}
 	
 
     return 0;
  }
