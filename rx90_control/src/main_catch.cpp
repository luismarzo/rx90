#include "ros/ros.h"
#include "std_msgs/Float64.h"

#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "catch");
  std_msgs::Float64 lmsg, rmsg;
  int sem=0;

  ros::NodeHandle m;


  ros::Publisher pub_lgripper = m.advertise<std_msgs::Float64>("/rx90/RX90_LGRIPPER_JOINT_position_controller/command", 1000);
  ros::Publisher pub_rgripper = m.advertise<std_msgs::Float64>("/rx90/RX90_RGRIPPER_JOINT_position_controller/command", 1000);
  ros::Publisher pub_mgripper = m.advertise<std_msgs::Float64>("/rx90/RX90_MGRIPPER_JOINT_position_controller/command", 1000);
  ros::Rate loop_rate(0.3);

  while (ros::ok())
  
    {
    if(sem==0)
    {
    lmsg.data = 20;
    rmsg.data = -20;
    ROS_INFO("%f", lmsg.data);
    sem=1;
    }   
    /*else if(sem==1)
    {
    lmsg.data = 0;
    rmsg.data = 0;
    ROS_INFO("%f", lmsg.data);
    sem=2;
    }*/
    pub_lgripper.publish(lmsg);
    pub_rgripper.publish(rmsg);

    ros::spinOnce();

    
    loop_rate.sleep();
  }


  return 0;
}
