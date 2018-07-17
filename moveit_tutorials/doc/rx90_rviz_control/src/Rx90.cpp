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


#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>

#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>
#include <tf/transform_datatypes.h>
#include <moveit_visual_tools/moveit_visual_tools.h>

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
	std::stringstream command_origin;
	command_origin << "DO SET #ORIGIN=#PPOINT(" << originPoint.c_str() << ")";
	command_origin << "DO SET #POSE=#PPOINT(" << originPoint.c_str() << ")";
	std::cout<<originPoint.c_str()<<std::endl;
	sendCommand(command_origin.str());

	sendCommand("SPEED 20"); 
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
	    rviz();
		//std::cout << "\n Give me a position Example:122,-77,-19,55,45,-43" << std::endl;
		//std::cin >> Point;
		//std::cout << Point << std::endl;
		//move_position(Point);
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
		sendCommand("SPEED 20");
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
	std::stringstream command_pose;
	command_pose << "DO SET #POSE=#PPOINT(" << PPoint.c_str() << ")";
	sendCommand(command_pose.str());
	sendCommand("SPEED 20");
	sendCommand("DO ABOVE");
	sendCommand("DO MOVE #POSE");
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

void Rx90::rviz(){

//   ros::init(argc, argv, "rx90_rviz_control");
//   ros::NodeHandle node_handle;
//   ros::AsyncSpinner spinner(1);
//   spinner.start();

  static const std::string PLANNING_GROUP = "rx90_arm";

  moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);

  moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

  const robot_state::JointModelGroup* joint_model_group =
      move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);

  namespace rvt = rviz_visual_tools;
  moveit_visual_tools::MoveItVisualTools visual_tools("world");  // puede ser world
  visual_tools.deleteAllMarkers();

  visual_tools.loadRemoteControl();

  Eigen::Affine3d text_pose = Eigen::Affine3d::Identity();
  text_pose.translation().z() = 1.75;
  visual_tools.publishText(text_pose, "Rx90 rviz control", rvt::WHITE, rvt::XLARGE);

  visual_tools.trigger();

  ROS_INFO_NAMED("tutorial", "Reference frame: %s", move_group.getPlanningFrame().c_str());

  ROS_INFO_NAMED("tutorial", "End effector link: %s", move_group.getEndEffectorLink().c_str());


  // z minimo real=270mm-370
  // eje x e y cambiados
  // al y le pones signo contrario

  float x, y, z, rotx, roty, rotz, aux;
  char send_position='y';
  std::string stg_x,stg_y,stg_z,stg_r,stg_p,stg_w;

  std::string Point;
  geometry_msgs::Pose target_pose1;
  moveit::planning_interface::MoveGroupInterface::Plan my_plan;
  bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
  tf::Quaternion q;

 
    printf("\033[01;33m");
    printf("\nMoving to a pose. Enter the desired pose (milimeters and degrees)\n");
    printf("\nx:");
    std::cin >> x;
    printf("\ny:");
    std::cin >> y;
    printf("\nz:");
    std::cin >> z;
    printf("\nrotx:");
    std::cin >> rotx;
    printf("\nroty:");
    std::cin >> roty;
    printf("\nrotz:");
    std::cin >> rotz;
    printf("\033[01;33m");

	//antes del cambio de coordenadas guardamos variables por si hay que enviarlas
	stg_x =static_cast<std::ostringstream*>(&(std::ostringstream() << x))->str();
	stg_y =static_cast<std::ostringstream*>(&(std::ostringstream() << y))->str();
	stg_z =static_cast<std::ostringstream*>(&(std::ostringstream() << z))->str();
	stg_r =static_cast<std::ostringstream*>(&(std::ostringstream() << rotx))->str();
	stg_p =static_cast<std::ostringstream*>(&(std::ostringstream() << roty))->str();
	stg_w =static_cast<std::ostringstream*>(&(std::ostringstream() << rotz))->str();

    // cambio de coordenadas para el real
    z = z + 370;
    aux = x;
    x = y;
    y = aux;
    x = (-1) * x;

    x = x / 1000;
    y = y / 1000;
    z = z / 1000;

    // changing into radians
    rotx = rotx * 2 * pi / 360;
    roty = roty * 2 * pi / 360;
    rotz = rotz * 2 * pi / 360;

    q.setEulerZYX(rotz, roty, rotx);

    if (z < 0.1)
    {
      printf("\033[1;31m");
      printf("\n[ERROR]: Variable 'z' must be mayor than 100");
      printf("\033[1;31m");
    }
    else
    {
      target_pose1.orientation.w = q.w();
      target_pose1.position.x = x;
      target_pose1.position.y = y;
      target_pose1.position.z = z;
      target_pose1.orientation.x = q.x();
      target_pose1.orientation.y = q.y();
      target_pose1.orientation.z = q.z();
      move_group.setPoseTarget(target_pose1);
      move_group.move();
      // move_group.setRPYTarget(xx,yy,zz);
      success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
      ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
      ROS_INFO_NAMED("tutorial", "Visualizing plan 1 as trajectory line");
      visual_tools.publishAxisLabeled(target_pose1, "pose1");
      visual_tools.publishText(text_pose, "Pose Goal", rvt::WHITE, rvt::XLARGE);
      visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
      visual_tools.trigger();

	  printf("\033[01;33m");
      printf("\nSend position ro real Rx90?:[y/n]");
      printf("\033[01;33m");
	  std::cin>>send_position;

	  if(send_position=='y')
	  {
		Point=stg_x+","+stg_y+","+stg_z+","+stg_r+","+stg_p+","+stg_w;
		std::cout<< Point<<std::endl;
		//move_position(Point);   Antes de enviar nada debes de comprobar el roll pitch yaw reales
	  }


    }





}
