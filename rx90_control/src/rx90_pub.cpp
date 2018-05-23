#include <gazebo/gazebo.hh>
#include <ros/ros.h>
#include <sstream>

#include "std_msgs/Float64.h"
#include "boost/bind.hpp"

namespace gazebo
{
  class rx90_publisher : public WorldPlugin
  {
    public: rx90_publisher() : WorldPlugin()
            {
		if(ros::ok()){
		
		
                printf("\n\nRX90 PUBLISHER! ARE YOU READY? [y/n]\n\n");
		
		std::cin>>input;
		if(input=='y')
		{
		printf("\n\nPUBLISHING, CLICK RUN BUTTOM IN THE GAZEBO SIMULATION\n\n");
		ros::NodeHandle n;
		pub = n.advertise<std_msgs::Float64>("/rx90/RX90_ARM_JOINT_position_controller/command", 1000);
		this->updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&rx90_publisher::OnUpdate, this, _1));
		}
		else
		{
		printf("\n\nOK BYE!\n\n");
		}
		}

            }

    public: void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
            {
            }

    public: void OnUpdate(const common::UpdateInfo &_info)
	{
		msg.data=cnt;
		pub.publish(msg);
		cnt=cnt+1;
		printf("\ncnt=%f", cnt);
		printf("\tmsg.data=%f", msg.data);
		sleep(2);
	}

   


    private:
	ros::Publisher pub;
	event::ConnectionPtr updateConnection;
	std_msgs::Float64 msg;
	float cnt=0;
	char input;
	



  };
  GZ_REGISTER_WORLD_PLUGIN(rx90_publisher)
}
