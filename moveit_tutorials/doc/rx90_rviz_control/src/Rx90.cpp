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
#include "std_msgs/Float64.h"
#include "gazebo_msgs/ApplyJointEffort.h"
#include "ros/ros.h"

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

Rx90::Rx90(const std::string& serialPort, const std::string& originPoint)
{
  init(serialPort, originPoint);
  x = 0.0;
  y = 0.0;
}

Rx90::~Rx90()
{
  close();
}

void Rx90::init(const std::string& serialPort, const std::string& originPoint)
{
  serial.Open(serialPort.c_str());
  serial.SetBaudRate(SerialStreamBuf::BAUD_9600);
  serial.SetParity(SerialStreamBuf::PARITY_NONE);
  serial.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
  serial.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
  serial.SetNumOfStopBits(1);
  serial.unsetf(std::ios_base::skipws);

  // Set origin precision point
  std::stringstream command_origin, command_pose;
  command_origin << "DO SET #ORIGIN=#PPOINT(" << originPoint.c_str() << ")";
  command_pose << "DO SET #POSE=#PPOINT(" << originPoint.c_str() << ")";
  std::cout << originPoint.c_str() << std::endl;
  sendCommand(command_origin.str());
  sendCommand(command_pose.str());
  sendCommand("SPEED 10");
  sendCommand("DO ABOVE");
  sendCommand("DO MOVE #ORIGIN");
  sendCommand("HERE ORIGIN", true);
  sendCommand("DO OPENI");
  sendCommand("DO ENABLE CP");
}

void Rx90::close()
{
  serial.Close();
}

void Rx90::sendCommand(const std::string& command, bool waitQuestionMark)
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

void Rx90::panic()
{
  sendCommand("PANIC");
}

void Rx90::move(const Action& action)
{
  std::string Point;
  char pos;

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
      std::cout<<" Do you want to introduce the position or the joints?[p/j]:"<<std::endl;
      std::cin>>pos;
      if(pos=='p'){
          rviz();
      }

      else if(pos=='j')
      {
          gazebo(0,-90,90,0,0,0,'n');
      }
      
      else
      {
        std::cout<<"Wrong character"<<std::endl;
      }

      break;
    default:
      std::cout << "unexpected!";
  }

  // send the command
  if (action != POSITION)
  {
    std::stringstream sstr;
    sstr << "DO SET P"
         << "=SHIFT(POSE BY " << (int)x << "," << (int)y << ",0)";
    std::string command = sstr.str();
    sendCommand(command);
    sendCommand("SPEED 10");
    sendCommand("DO MOVES P");
  }
  else
  {
  }
}

void Rx90::move_position(const std::string& PPoint)
{
  /*std::string Point;
Point = "100,-77,-43,55,45,-43";*/
  std::stringstream command_pose;
  command_pose << "DO SET #POSE=#PPOINT(" << PPoint.c_str() << ")";
  sendCommand(command_pose.str());
  sendCommand("SPEED 10");
  sendCommand("DO ABOVE");
  sendCommand("DO MOVE #POSE");
}

void Rx90::catchIt()
{
  sendCommand("DO CLOSEI");
}

void Rx90::printAction(const Action& action)
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

void Rx90::rviz()
{

   //ros::NodeHandle node_handle;
   //ros::AsyncSpinner spinner(1);
   //spinner.start();


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
  char send_position = 'y';
  // std::string stg_x, stg_y, stg_z, stg_r, stg_p, stg_w;

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


  // cambio de coordenadas para el real

  // y maxima 970, en simulacion 1080
  // z max 970, minimo es -284. en simulacon la minima es 100 y maxima 1500. punto intermedio 500
  // x max 970

  // y=180 mira hacia abajo, 0 hacia arriba y 90 recto.

  // posiciones que se le van a enviiar -89.57,-393.16,580.48,-167.73,73.76,-103.63
  //-406.54,-273.28,847.70,-147.53,35.53,-147.83
  // calibrar el robot es en putty:
  //.cal
  //.do ready

  // 65.67,-355.19,k617.37,-145.87,53.40,-117.86
  y = y * 1080 / 970;
  x = x * 1080 / 970;
  z = z + 284;
  z = z * 1350 / 1254;
  z = z + 100;

  aux = x;
  x = y;
  y = aux;
  x = (-1) * x;

  x = x / 1000;
  y = y / 1000;
  z = z / 1000;

  rotz = rotz + 90;
  roty = roty - 90;

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

    
    printf("\nSend position to Gazebo?:[y/n]");
    std::cin >> send_position;

    if (send_position == 'y')
    {
      std::cout << "Vamos a leer las posiciones de las articulaciones " << std::endl;
      moveit::core::RobotStatePtr current_state = move_group.getCurrentState();
      std::vector<double> joint_group_positions;
      current_state->copyJointGroupPositions(joint_model_group, joint_group_positions);


      float j1,j2,j3,j4,j5,j6;
      // Cambio de coordenadas de gazebo a robot real para enviarselas al rx90
      j1=joint_group_positions[0]*360/(2*pi)*(-1);
      j2=joint_group_positions[1]*360/(2*pi);
      j2=j2-90;
      j3=joint_group_positions[2]*360/(2*pi);
      j3=j3+90;
      j4=joint_group_positions[3]*360/(2*pi);
      j5=joint_group_positions[4]*360/(2*pi);
      j6=joint_group_positions[5]*360/(2*pi);

      gazebo(j1,j2,j3,j4,j5,j6,'y');
    }
  }
}


void Rx90::gazebo(float _j1, float _j2, float _j3, float _j4, float _j5, float _j6, char send){

  std_msgs::Float64 msg;
	msg.data = 0;
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

  if(send=='n')  //Las posiciones de las joints no vienen de la función rviz
  {
      
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
  }
  
  else
  {
    // printf("INTRODUCE COORDENADAS ALEATORIAS, PREGUNTAR A PABLO ESTE ISSUE"); //Coordenadas del sistema de referencia del mando del Rx90
	  // printf("\nj1:");
	  // std::cin>>j1;
	  // printf("\nj2:");
	  // std::cin>>j2;
	  // printf("\nj3:");
	  // std::cin>>j3;
	  // printf("\nj4:");
	  // std::cin>>j4;
	  // printf("\nj5:");
	  // std::cin>>j5;
	  // printf("\nj6:");
	  // std::cin>>j6;
    j1=_j1;
    j2=_j2;
    j3=_j3;
    j4=_j4;
    j5=_j5;
    j6=_j6;
  }
  
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
   std::cout<<"pub_msgdataaa:"<<msg.data<<std::endl;
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




   ////////////////////////////////////////////Meter restricciones de los joints para dejar enviar o no
  
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