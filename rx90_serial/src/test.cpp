#include "Rx90.h"
#include "gazebo_msgs/ApplyJointEffort.h"
#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "stdio.h"
#include <thread>
#include "std_msgs/Duration.h"
#include "std_msgs/Time.h"

#define pi 3.14159265359


int main(int argc, char **argv)
{
    sleep(12);
	printf("\n");
    printf("//////////////////////////////////////////////////\n");
    printf("\n");
    printf("            RX 90 SIMULATION AND CONTROL          \n");
    printf("\n");
    printf("\n");
    printf("                       GRVC                       \n");
    printf("//////////////////////////////////////////////////\n\n");


	//Ros publisher
	ros::init(argc, argv, "publisher");
	// std_msgs::Float64 msg;
	// msg.data = 0;
	// int end = 0;
	// int alert = 0;
	// float tiempo = 4;
	// ros::NodeHandle n;


//   ros::Publisher pub_shoulder_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_SHOULDER_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_arm_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_ARM_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_elbow_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_ELBOW_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_forearm_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_FOREARM_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_wrist_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_WRIST_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_flange_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_FLANGE_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_lgripper_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_LGRIPPER_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_rgripper_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_RGRIPPER_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_mgripper_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_MGRIPPER_JOINT_position_controller/command", 1000);

//   ros::Publisher pub_shoulder_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_SHOULDER_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_arm_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_ARM_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_elbow_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_ELBOW_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_forearm_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_FOREARM_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_wrist_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_WRIST_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_flange_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_FLANGE_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_lgripper_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_LGRIPPER_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_rgripper_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_RGRIPPER_JOINT_position_controller/command", 1000);
//   ros::Publisher pub_mgripper_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_MGRIPPER_JOINT_position_controller/command", 1000);



//   ros::Rate loop_rate(10);

//       float j1,j2,j3,j4,j5,j6;
//       ROS_INFO("READY! give methe joints in degrees (j1,j2,j3,j4,j5,j6)");
// 	  ROS_INFO("\nj1:");
// 	  std::cin>>j1;
// 	  ROS_INFO("\nj2:");
// 	  std::cin>>j2;
// 	  ROS_INFO("\nj3:");
// 	  std::cin>>j3;
// 	  ROS_INFO("\nj4:");
// 	  std::cin>>j4;
// 	  ROS_INFO("\nj5:");
// 	  std::cin>>j5;
// 	  ROS_INFO("\nj6:");
// 	  std::cin>>j6;
	 
// 	  //Changing to radians and changing the sign  Lo cambio aquí porque es peor cambiar el urdf
// 	  //Este cambio solo sirve para la simulación de gazebo
// 	  j1=j1*2*pi/360;
// 	  j1=j1* (-1);
// 	  j2=j2+90;
// 	  j2=j2*2*pi/360;
// 	  j3=j3-90;
// 	  j3=j3*2*pi/360;
// 	  j4=j4*2*pi/360;
// 	  j5=j5*2*pi/360;
// 	  j6=j6*2*pi/360;



// 	 msg.data=j1;
// 	 pub_shoulder_2.publish(msg);
// 	 msg.data=j2;
// 	 pub_arm_2.publish(msg);
// 	 msg.data=j3;
// 	 pub_elbow_2.publish(msg);
// 	 msg.data=j4;
// 	 pub_forearm_2.publish(msg);
// 	 msg.data=j5;
// 	 pub_wrist_2.publish(msg);
// 	 msg.data=j6;
// 	 pub_flange_2.publish(msg);



	// Rx90
	Rx90 rx90("/dev/ttyUSB0", "0,-90,90,0,0,0"); //Posicion buena  "122,-77,-19,55,45,-43"
	//59.06,-88.79,37.29,90.59,6.34,-43
	char key;

	bool play = true;
	while (play)
	{

		Rx90::Action action = Rx90::NONE;
		std::cout << "Usage> w:up, s:down, a:left, d:right, e:catch, q:quit, p:position\n"
			    << std::endl;
		std::cin >> key;
		switch (key)
		{
		case 'w':
			action = Rx90::UP;
			break;
		case 'a':
			action = Rx90::LEFT;
			break;
		case 's':
			action = Rx90::DOWN;
			break;
		case 'd':
			action = Rx90::RIGHT;
			break;
		case 'e':
			action = Rx90::CATCH;
			break;
		case 'q':
			play = false;
			break;
		case 'p':
			action = Rx90::POSITION;
			break;
		default:
			std::cout << "Repeat key\n"
				    << std::endl;
		}

		Rx90::printAction(action);
		rx90.move(action);
	}
}
