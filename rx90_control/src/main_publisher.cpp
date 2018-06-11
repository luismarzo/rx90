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
  int alert=0;
  float tiempo=4;
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
  std::thread garra;
  

//THREAD GRIPPER
          auto mgarra = [&end,&alert,&argc,&argv](){ 
            ros::init(argc, argv, "publisher_finger");
            ros::NodeHandle n;

            ros::ServiceClient client = n.serviceClient<gazebo_msgs::ApplyJointEffort>("/gazebo/apply_joint_effort");
            gazebo_msgs::ApplyJointEffort gripper_1, gripper_2;

            gripper_1.request.joint_name = "rx90_1::RX90_MGRIPPER_JOINT";
            gripper_1.request.effort = -99;
            gripper_1.request.start_time = ros::Time(0);
            gripper_1.request.duration = ros::Duration(-1);

            gripper_2.request.joint_name = "rx90_2::RX90_MGRIPPER_JOINT";
            gripper_2.request.effort = 99;
            gripper_2.request.start_time = ros::Time(0);
            gripper_2.request.duration = ros::Duration(-1);
            client.call(gripper_2);
            client.call(gripper_1);
            ROS_INFO("hilo");

            while(!end)
            {
              sleep(0.2);
              
              if(alert==1){
              //gripper_1.request.joint_name = "rx90_1::RX90_MGRIPPER_JOINT";
              gripper_1.request.effort = 100;
              //gripper_1.request.start_time = ros::Time(0);
              //gripper_1.request.duration = ros::Duration(-1);
              client.call(gripper_1);


              gripper_2.request.effort = -150;
              client.call(gripper_2);
              alert=2;
              }
              else if(alert==2)
              {
                gripper_1.request.effort = 100;
                
                client.call(gripper_1);
                gripper_2.request.effort = -150;
                client.call(gripper_2);
                alert=3;
              }

              
            }

            //gripper_1.request.joint_name = "rx90_1::RX90_MGRIPPER_JOINT";
            gripper_1.request.effort = 100;
            gripper_2.request.effort = 200;
            //gripper_1.request.start_time = ros::Time(0);
            //gripper_1.request.duration = ros::Duration(-1);
            client.call(gripper_1);
            client.call(gripper_2);
            client.call(gripper_1);
            client.call(gripper_2);
            };



    

//FIRST ARM
      
      ROS_INFO("READY!");
      pub_arm_1.publish(msg);
      pub_arm_2.publish(msg);
      sleep(tiempo);
      msg.data=-0.06;
      
      ROS_INFO("moving arm [%f]", msg.data);
      pub_arm_1.publish(msg);
      pub_arm_2.publish(msg);
      sleep(tiempo);
      msg.data=0.7;

      ROS_INFO("moving elbow [%f]", msg.data);
      pub_elbow_1.publish(msg);
      pub_elbow_2.publish(msg);
      sleep(tiempo);
      msg.data=-0.3;

      ROS_INFO("moving arm [%f]", msg.data);
      pub_arm_1.publish(msg);
      pub_arm_2.publish(msg);
      sleep(tiempo);
      garra = std::thread(mgarra);
      sleep(tiempo);
      msg.data=0.9;

      ROS_INFO("moving arm [%f]", msg.data);
      pub_arm_1.publish(msg);
      pub_arm_2.publish(msg);
      sleep(tiempo);
      msg.data=0.85;

      ROS_INFO("moving wrist [%f]", msg.data);
      pub_wrist_1.publish(msg);
      pub_wrist_2.publish(msg);
      sleep(tiempo+3);
      msg.data=0.85;

      ROS_INFO("moving arm [%f]", msg.data);
      pub_arm_2.publish(msg);
      msg.data=1.20;
      sleep(tiempo);


      ROS_INFO("moving elbow [%f]", msg.data);
      pub_elbow_2.publish(msg);
      msg.data=0.3;
      sleep(tiempo);

      ROS_INFO("moving wrist [%f]", msg.data);
      pub_wrist_2.publish(msg);
      msg.data=0.7;
      sleep(tiempo+1);
      alert=1;
      sleep(tiempo-1);

      ROS_INFO("moving arm [%f]", msg.data);
      pub_arm_1.publish(msg);
      msg.data=-0.7;
      ROS_INFO("moving elbow [%f]", msg.data);
      pub_elbow_1.publish(msg);
      msg.data=1.6;
      sleep(tiempo);

      ROS_INFO("moving shoulder [%f]", msg.data);
      pub_shoulder_2.publish(msg);
      sleep(tiempo);
      msg.data=0;

      ROS_INFO("moving wrist [%f]", msg.data);
      pub_wrist_2.publish(msg);
      sleep(tiempo);
      msg.data=-0.1;

      ROS_INFO("moving arm [%f]", msg.data);
      pub_arm_2.publish(msg);
      sleep(tiempo);
      end=1;
  
      
      end=1;

     garra.join();
 

 

  return 0;
}
