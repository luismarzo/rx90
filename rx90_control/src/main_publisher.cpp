#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "stdio.h"
#include <sstream>
#include <thread>
#include "gazebo_msgs/ApplyJointEffort.h"
#include "std_msgs/Duration.h"
#include "std_msgs/Time.h"


int main(int argc, char **argv)
{
  ros::init(argc, argv, "publisher");
  std_msgs::Float64 msg;
  msg.data = 0;
  int end=0;
  ros::NodeHandle n;


  ros::Publisher pub_shoulder = n.advertise<std_msgs::Float64>("/rx90/RX90_SHOULDER_JOINT_position_controller/command", 1000);
  ros::Publisher pub_arm = n.advertise<std_msgs::Float64>("/rx90/RX90_ARM_JOINT_position_controller/command", 1000);
  ros::Publisher pub_elbow = n.advertise<std_msgs::Float64>("/rx90/RX90_ELBOW_JOINT_position_controller/command", 1000);
  ros::Publisher pub_forearm = n.advertise<std_msgs::Float64>("/rx90/RX90_FOREARM_JOINT_position_controller/command", 1000);
  ros::Publisher pub_wrist = n.advertise<std_msgs::Float64>("/rx90/RX90_WRIST_JOINT_position_controller/command", 1000);
  ros::Publisher pub_flange = n.advertise<std_msgs::Float64>("/rx90/RX90_FLANGE_JOINT_position_controller/command", 1000);
  ros::Publisher pub_lgripper = n.advertise<std_msgs::Float64>("/rx90/RX90_LGRIPPER_JOINT_position_controller/command", 1000);
  ros::Publisher pub_rgripper = n.advertise<std_msgs::Float64>("/rx90/RX90_RGRIPPER_JOINT_position_controller/command", 1000);
  ros::Publisher pub_mgripper = n.advertise<std_msgs::Float64>("/rx90/RX90_MGRIPPER_JOINT_position_controller/command", 1000);
  ros::Rate loop_rate(10);
  std::thread garra;


 auto mgarra = [&end,&argc,&argv](){ 
  ros::init(argc, argv, "publisher_finger");
  ros::NodeHandle n;

  ros::ServiceClient client = n.serviceClient<gazebo_msgs::ApplyJointEffort>("/gazebo/apply_joint_effort");
  gazebo_msgs::ApplyJointEffort srv;
  srv.request.joint_name = "rx90::RX90_MGRIPPER_JOINT";
  srv.request.effort = -100;
  srv.request.start_time = ros::Time(0);
  srv.request.duration = ros::Duration(-1);
  client.call(srv);
  ROS_INFO("hilo");

  while(!end)
  {
    sleep(0.2);
    
  }

  srv.request.joint_name = "rx90::RX90_MGRIPPER_JOINT";
  srv.request.effort = 100;
  srv.request.start_time = ros::Time(0);
  srv.request.duration = ros::Duration(-1);
  client.call(srv);

  };


      float tiempo=4;
      ROS_INFO("Ready!");
      pub_arm.publish(msg);
      sleep(tiempo);
      msg.data=-0.1;
      ROS_INFO("moving arm [%f]", msg.data);
      pub_arm.publish(msg);
      sleep(tiempo);
      msg.data=0.6;
      ROS_INFO("moving elbow [%f]", msg.data);
      pub_elbow.publish(msg);
      sleep(tiempo);
      msg.data=-0.3;
      ROS_INFO("moving arm [%f]", msg.data);
      pub_arm.publish(msg);
      sleep(tiempo);
      garra = std::thread(mgarra);
      sleep(tiempo);
      msg.data=0.9;
      ROS_INFO("moving arm [%f]", msg.data);
      pub_arm.publish(msg);
      sleep(tiempo);
      msg.data=0.9;
      ROS_INFO("moving wrist [%f]", msg.data);
      pub_wrist.publish(msg);
      sleep(tiempo);
      msg.data=-1.6;
      ROS_INFO("moving shoulder [%f]", msg.data);
      pub_shoulder.publish(msg);
      sleep(tiempo);
      msg.data=0;
      ROS_INFO("moving wrist [%f]", msg.data);
      pub_wrist.publish(msg);
      sleep(tiempo);
      msg.data=-0.1;
      ROS_INFO("moving arm [%f]", msg.data);
      pub_arm.publish(msg);
      sleep(tiempo);
      end=1;

    

  //    ros::spinOnce();
  //    loop_rate.sleep();
  //   ROS_INFO("Ending");
     garra.join();
  //   break;
  // }

 

  return 0;
}
