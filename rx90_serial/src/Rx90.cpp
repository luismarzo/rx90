//
// C++ Implementation: Rx90
//
// Description:
//
//
// <franroyal@yahoo.es>
//
//

#include "Rx90.h"
#include <sstream>
#include <cmath>
#include <stdexcept>
#include "gazebo_msgs/ApplyJointEffort.h"
#include "ros/ros.h"
#include "std_msgs/Float64.h"


#define pi 3.14159265359

#define DELTA_VH 25
#define DELTA_DIAG (0.707 * DELTA_VH)
#define END "\r\n"

using namespace LibSerial;

Rx90::Rx90(const std::string &serialPort, const std::string &originPoint)
{

	init(serialPort, originPoint);
	x = 0.0;
	y = 0.0;
}

Rx90::~Rx90() { close(); }

void Rx90::init(const std::string &serialPort, const std::string &originPoint)
{

	serial.Open(serialPort.c_str());
	serial.SetBaudRate(SerialStreamBuf::BAUD_9600);
	serial.SetParity(SerialStreamBuf::PARITY_NONE);
	serial.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
	serial.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
	serial.SetNumOfStopBits(1);
	serial.unsetf(std::ios_base::skipws);

	// Set origin precision point
	std::stringstream command_pose,command_origin;
	command_origin << "DO SET #ORIGIN=#PPOINT(" << originPoint.c_str() << ")";
	//command_pose << "DO SET #POSE=#PPOINT(" << originPoint.c_str() << ")";
	std::cout<<originPoint.c_str()<<std::endl;
	sendCommand(command_origin.str());
	//sendCommand(command_pose.str());
	sendCommand("SPEED 10"); 
	sendCommand("DO ABOVE");
	sendCommand("DO MOVE #ORIGIN");
	sendCommand("HERE ORIGIN", true);
	sendCommand("DO OPENI");
	sendCommand("DO ENABLE CP");
}

void Rx90::close() { serial.Close(); }

void Rx90::sendCommand(const std::string &command, bool waitQuestionMark)
{

	if (serial.IsOpen())
	{

		serial << command << END;

		if (waitQuestionMark)
		{
			char qm;
			do
			{
				serial >> qm;
			} while (qm != '?');
			serial << END;
		}

		char r;
		do
		{
			serial >> r;
			std::cout << r;
		} while (r != '.');
	}
	else
	{
		std::cout << "Serial port not opened" << std::endl;
	}
}

void Rx90::panic() { sendCommand("PANIC"); }

void Rx90::move(const Action &action)
{

	std::string Point;

	switch (action)
	{
	case NONE:
		break;
	case UP:
		y += DELTA_VH;
		break;
	case DOWN:
		y -= DELTA_VH;
		break;
	case RIGHT:
		x += DELTA_VH;
		break;
	case LEFT:
		x -= DELTA_VH;
		break;
	case UP_RIGHT:
		x += DELTA_DIAG;
		y += DELTA_DIAG;
		break;
	case UP_LEFT:
		x -= DELTA_DIAG;
		y += DELTA_DIAG;
		break;
	case DOWN_LEFT:
		x -= DELTA_DIAG;
		y -= DELTA_DIAG;
		break;
	case DOWN_RIGHT:
		x += DELTA_DIAG;
		y -= DELTA_DIAG;
		break;
	case CATCH:
		catchIt();
		break;
	case POSITION:
		gazebo();
		// std::cout << "\n Give me the joints Example:122,-77,-19,55,45,-43" << std::endl;
		// std::cin >> Point;
		// std::cout << Point << std::endl;
		// move_position(Point);
		break;
	default:
		std::cout << "unexpected!";
	}

	// send the command
	if (action != POSITION)
	{
		std::stringstream sstr;
		sstr << "DO SET P" << "=SHIFT(POSE BY " << (int)x << "," << (int)y << ",0)";
		std::string command = sstr.str();
		sendCommand(command);
		sendCommand("SPEED 10");
		sendCommand("DO MOVES P");
	}
	else
	{
	}
}

void Rx90::move_position(const std::string &PPoint)
{
    /*std::string Point;
	Point = "100,-77,-43,55,45,-43";*/
	printf("\nMoving joints\n");
	std::stringstream command_pose;
	command_pose << "DO SET #POSE=#PPOINT(" << PPoint.c_str() << ")";
	sendCommand(command_pose.str());
	sendCommand("SPEED 10");
	sendCommand("DO ABOVE");
	sendCommand("DO MOVE #POSE");
	printf("\nEnding...\n");
}

void Rx90::gazebo(){
	

	std_msgs::Float64 msg;
	msg.data = 0;
	int end = 0;
	int alert = 0;
	float tiempo = 4;
	ros::NodeHandle n;
	char send_position='y';


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

      float j1,j2,j3,j4,j5,j6;
      printf("READY! give methe joints in degrees (j1,j2,j3,j4,j5,j6)"); //Coordenadas del sistema de referencia del mando del Rx90
	  printf("\nj1:");
	  std::cin>>j1;
	  printf("\nj2:");
	  std::cin>>j2;
	  printf("\nj3:");
	  std::cin>>j3;
	  printf("\nj4:");
	  std::cin>>j4;
	  printf("\nj5:");
	  std::cin>>j5;
	  printf("\nj6:");
	  std::cin>>j6;

		//Se hace antes para no pillar las transformaciones a Gazebo
	  	std::string Joints;
		std::string stg_j1,stg_j2,stg_j3,stg_j4,stg_j5,stg_j6;
		stg_j1 =static_cast<std::ostringstream*>(&(std::ostringstream() << j1))->str();
		stg_j2 =static_cast<std::ostringstream*>(&(std::ostringstream() << j2))->str();
		stg_j3 =static_cast<std::ostringstream*>(&(std::ostringstream() << j3))->str();
		stg_j4 =static_cast<std::ostringstream*>(&(std::ostringstream() << j4))->str();
		stg_j5 =static_cast<std::ostringstream*>(&(std::ostringstream() << j5))->str();
		stg_j6 =static_cast<std::ostringstream*>(&(std::ostringstream() << j6))->str();
	 
	  //Changing to radians and changing the sign  Lo cambio aquí porque es peor cambiar el urdf
	  //Este cambio solo sirve para la simulación de gazebo
	  j1=j1*2*pi/360;
	  j1=j1* (-1);
	  j2=j2+90;
	  j2=j2*2*pi/360;
	  j3=j3-90;
	  j3=j3*2*pi/360;
	  j4=j4*2*pi/360;
	  j5=j5*2*pi/360;
	  j6=j6*2*pi/360;


	//Publicación en gazebo
	 msg.data=j1;
	 pub_shoulder_2.publish(msg);
	 msg.data=j2;
	 pub_arm_2.publish(msg);
	 msg.data=j3;
	 pub_elbow_2.publish(msg);
	 msg.data=j4;
	 pub_forearm_2.publish(msg);
	 msg.data=j5;
	 pub_wrist_2.publish(msg);
	 msg.data=j6;
	 pub_flange_2.publish(msg);

	printf("\nSend position to real Rx90?:[y/n]");
	std::cin>>send_position;

	if(send_position=='y')
	  {
		//Se manda sin los cambios de coordenadas realizados a Gazebo
		Joints=stg_j1+","+stg_j2+","+stg_j3+","+stg_j4+","+stg_j5+","+stg_j6;
		std::cout<< "\n" <<  Joints <<std::endl;
		move_position(Joints);  
	  }


	
}

void Rx90::catchIt() { sendCommand("DO CLOSEI"); }

void Rx90::printAction(const Action &action)
{

	std::cout << "Rx90::printAction: ";
	switch (action)
	{
	case NONE:
		std::cout << "none!";
		break;
	case UP:
		std::cout << "up!";
		break;
	case DOWN:
		std::cout << "down!";
		break;
	case RIGHT:
		std::cout << "right!";
		break;
	case LEFT:
		std::cout << "left!";
		break;
	case UP_RIGHT:
		std::cout << "up-right!";
		break;
	case UP_LEFT:
		std::cout << "up-left!";
		break;
	case DOWN_LEFT:
		std::cout << "down-left!";
		break;
	case DOWN_RIGHT:
		std::cout << "down-right!";
		break;
	case CATCH:
		std::cout << "catch!";
		break;
	case POSITION:
		std::cout << "moving to a position!";
		break;
	default:
		std::cout << "unexpected!";
	}
	std::cout << std::endl;
}
