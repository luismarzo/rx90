#include "Rx90.h"
#include "gazebo_msgs/ApplyJointEffort.h"
#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "stdio.h"
#include <thread>
#include "std_msgs/Duration.h"
#include "std_msgs/Time.h"

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>

#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>
#include <tf/transform_datatypes.h>
#include <moveit_visual_tools/moveit_visual_tools.h>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo_client.hh>




int main(int argc, char **argv)
{
   ros::init(argc, argv, "rx90_rviz_control");
   gazebo::client::setup(argc, argv);
   ros::AsyncSpinner spinner(1);
   spinner.start();

    sleep(20);
	printf("\n");
    printf("//////////////////////////////////////////////////\n");
    printf("\n");
    printf("            RX90 SIMULATION AND CONTROL          \n");
    printf("\n");
    printf("\n");
    printf("                       GRVC                       \n");
    printf("//////////////////////////////////////////////////\n\n");

 // calibrar el robot es en putty:
  //.cal
  //.do ready

	// Rx90
	Rx90 rx90("/dev/ttyUSB0", "122,-77,-19,55,45,-43"); //Posicion de joints en grados
	//en radianes la posición se te queda comoo 
	char key;

	bool play = true;
	while (play)
	{
		
		Rx90::Action action = Rx90::NONE;
		std::cout << "Usage> w:up, s:down, a:left, d:right, e:catch, o:open, q:quit, p:position:\n"
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
		case 'o':
			action = Rx90::OPEN;
			break;
		case 'q':
			play = false;
			break;
		case 'p':
			action = Rx90::POSITION;
			break;
		case 'n':
			action = Rx90::NONE;
			break;
		// case 'k':
		// 	action = Rx90::KNOW;
		// 	break;
		default:
			std::cout << "Repeat key\n"
				    << std::endl;
			action = Rx90::NONE;
		}

		Rx90::printAction(action);
		rx90.move(action);
	}
}
