/*

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>

#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>
#include <tf/transform_datatypes.h>
#include <moveit_visual_tools/moveit_visual_tools.h>

#define pi 3.14159265359

int main(int argc, char** argv)
{

    sleep(16);
	  printf("\n");
    printf("//////////////////////////////////////////////////\n");
    printf("\n");
    printf("            RX 90 SIMULATION AND CONTROL          \n");
    printf("\n");
    printf("\n");
    printf("                       GRVC                       \n");
    printf("//////////////////////////////////////////////////\n\n");





  ros::init(argc, argv, "rx90_rviz_control");
  ros::NodeHandle node_handle;
  ros::AsyncSpinner spinner(1);
  spinner.start();

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
  char continuee = 'y';
  geometry_msgs::Pose target_pose1;
  moveit::planning_interface::MoveGroupInterface::Plan my_plan;
  bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
  tf::Quaternion q;

  while (continuee == 'y')
  {
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
    }
    printf("\033[01;33m");
    printf("\nContinue?[y/n]:");
    printf("\033[01;33m");
    std::cin >> continuee;
  }

  return 0;
}

*/
