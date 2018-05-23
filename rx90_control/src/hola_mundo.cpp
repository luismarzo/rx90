#include <gazebo/gazebo.hh>
#include <ros/ros.h>
#include "boost/bind.hpp"
#include "gazebo/gazebo.hh"

namespace gazebo
{
  class WorldPluginTutorial : public WorldPlugin
  {
    public: WorldPluginTutorial() : WorldPlugin()
            {
              printf("\n\nHola mundo!, DALE AL PLAY EN LA SIMULACIóN!!!!!!\n\n");
	
		 this->updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&WorldPluginTutorial::OnUpdate, this, _1));
            }

    public: void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
            {
		
            }
    public:
	void OnUpdate(const common::UpdateInfo &_info)
	{
		printf("\n\nHola mundo juas!\n\n");
	}
	private:
	event::ConnectionPtr updateConnection;

  };
  GZ_REGISTER_WORLD_PLUGIN(WorldPluginTutorial)
}
