#include "ros/ros.h"
#include "std_msgs/Float64.h"

#include <sstream>


int main(int argc, char **argv)
{
  ros::init(argc, argv, "publisher");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<std_msgs::Float64>("/rx90/RX90_ARM_JOINT_position_controller/command", 1000);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    std_msgs::Float64 msg;

    msg.data = 20;

    ROS_INFO("%f", msg.data);

    pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}
