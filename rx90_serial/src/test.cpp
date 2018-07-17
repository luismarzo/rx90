#include "Rx90.h"
#include "gazebo_msgs/ApplyJointEffort.h"
#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "stdio.h"
#include <thread>
#include "std_msgs/Duration.h"
#include "std_msgs/Time.h"




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
	std_msgs::Float64 msg;
	msg.data = 0;
	int end = 0;
	int alert = 0;
	float tiempo = 4;
	ros::NodeHandle n;


  ros::Publisher pub_shoulder_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_SHOULDER_JOINT_position_controller/command", 1000);
  ros::Publisher pub_arm_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_ARM_JOINT_position_controller/command", 1000);
  ros::Publisher pub_elbow_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_ELBOW_JOINT_position_controller/command", 1000);
  ros::Publisher pub_forearm_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_FOREARM_JOINT_position_controller/command", 1000);
  ros::Publisher pub_wrist_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_WRIST_JOINT_position_controller/command", 1000);
  ros::Publisher pub_flange_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_FLANGE_JOINT_position_controller/command", 1000);
  ros::Publisher pub_lgripper_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_LGRIPPER_JOINT_position_controller/command", 1000);
  ros::Publisher pub_rgripper_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_RGRIPPER_JOINT_position_controller/command", 1000);
  ros::Publisher pub_mgripper_1 = n.advertise<std_msgs::Float64>("/rx90_1/RX90_MGRIPPER_JOINT_position_controller/command", 1000);

  ros::Publisher pub_shoulder_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_SHOULDER_JOINT_position_controller/command", 1000);
  ros::Publisher pub_arm_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_ARM_JOINT_position_controller/command", 1000);
  ros::Publisher pub_elbow_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_ELBOW_JOINT_position_controller/command", 1000);
  ros::Publisher pub_forearm_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_FOREARM_JOINT_position_controller/command", 1000);
  ros::Publisher pub_wrist_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_WRIST_JOINT_position_controller/command", 1000);
  ros::Publisher pub_flange_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_FLANGE_JOINT_position_controller/command", 1000);
  ros::Publisher pub_lgripper_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_LGRIPPER_JOINT_position_controller/command", 1000);
  ros::Publisher pub_rgripper_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_RGRIPPER_JOINT_position_controller/command", 1000);
  ros::Publisher pub_mgripper_2 = n.advertise<std_msgs::Float64>("/rx90_2/RX90_MGRIPPER_JOINT_position_controller/command", 1000);



  ros::Rate loop_rate(10);

      char x_p,y_p,z_p,r_p,p_p,w_p;
      ROS_INFO("READY! give me a position (x,y,z,roll,pitch,yaw)");
	  ROS_INFO("\nx:");
	  std::cin>>x_p;
	  ROS_INFO("\ny:");
	  std::cin>>y_p;
	  ROS_INFO("\nz:");
	  std::cin>>z_p;
	  ROS_INFO("\nroll:");
	  std::cin>>r_p;
	  ROS_INFO("\npitch:");
	  std::cin>>p_p;
	  ROS_INFO("\nyaw:");
	  std::cin>>w_p;
	 
	 msg.data=x_p;
	 pub_shoulder_1.publish(msg);
	 msg.data=y_p;
	 pub_arm_1.publish(msg);
	 msg.data=z_p;
	 pub_elbow_1.publish(msg);
	 msg.data=r_p;
	 pub_forearm_1.publish(msg);
	 msg.data=p_p;
	 pub_wrist_1.publish(msg);
	 msg.data=w_p;
	 pub_flange_1.publish(msg);



	// Rx90
	Rx90 rx90("/dev/ttyUSB0", "122,-77,-19,55,45,-43"); //Posicion buena
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
