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




int main(int argc, char **argv)
{
   ros::init(argc, argv, "rx90_rviz_control");
   ros::NodeHandle node_handle;
   ros::AsyncSpinner spinner(1);
   spinner.start();

    sleep(16);
	printf("\n");
    printf("//////////////////////////////////////////////////\n");
    printf("\n");
    printf("            RX 90 SIMULATION AND CONTROL          \n");
    printf("\n");
    printf("\n");
    printf("                       GRVC                       \n");
    printf("//////////////////////////////////////////////////\n\n");



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
