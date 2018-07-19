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
