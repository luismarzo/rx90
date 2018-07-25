#include "boost/bind.hpp"
#include "gazebo/gazebo.hh"
#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"
#include "stdio.h"
#include <fstream>

#include "gazebo/physics/PhysicsIface.hh"
#include "gazebo/physics/Model.hh"
#include "gazebo/physics/World.hh"
#include "gazebo/physics/ContactManager.hh"
#include "gazebo/physics/PhysicsEngine.hh"
#include "gazebo/physics/Collision.hh"
#include "gazebo/physics/Link.hh"
#include <gazebo/physics/Inertial.hh>

#include <gazebo/common/Plugin.hh>
#include <ros/ros.h>
//#include "service_tutorial/Set_position.h"
//#include "service_tutorial/Set_position.h"
//#include <LinkConfig.hh>

namespace gazebo
{
class contact_plugin : public ModelPlugin
{
  public:
	void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
	{
		
		this->model = _parent;
		this->world = this->model->GetWorld();
		this->number = this->world->GetModelCount();
		this->nombres = this->world->GetModels();
		this->nombre_mundo = this->world->GetName();
		
		std::cout<<"modelo:"<<model->GetName()<<std::endl;
		
		if (!ros::isInitialized())
		{
			ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
							 << "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
			return;
		}
		this->link_nombres = this->model->GetLinks();
		std::cout<<link_nombres[0]->GetName()<<std::endl;
		this->link = this->model->GetLink("RX90_BASE");
		int a;
		a = this->model->GetChildCount();
		std::cout<<a<<std::endl;
		chil=this->model->GetChild("RX90_BASE");
		std::cout<<this->model->GetChild(2)->GetName()<<std::endl;
		//col2 = this->model->GetChildCollision("RX90_SHOULDER");
		// const gazebo::math::Pose& pse = col2->GetState().GetPose();
		// std::cout <<pse << std::endl;
		ROS_INFO("\n\nContact Plugin activated!\n");
		ros::NodeHandle n;
		ROS_INFO("Ready.");
		this->contactManager = this->model->GetWorld()->GetPhysicsEngine()->GetContactManager();



		this->updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&contact_plugin::OnUpdate, this, _1));
	}

  public:
	void OnUpdate(const common::UpdateInfo &_info)
	{		
		col1 = this->model->GetChildCollision("RX90_BASE");
		col2 = this->model->GetChildCollision("RX90_SHOULDER");
		col3 = this->model->GetChildCollision("RX90_ARM");
		col4 = this->model->GetChildCollision("RX90_WRIST");
		col5 = this->model->GetChildCollision("RX90_FLANGE");
		col6 = this->model->GetChildCollision("RX90_FOREARM");
		col7 = this->model->GetChildCollision("RX90_LGRIPPER");
		col8 = this->model->GetChildCollision("RX90_MGRIPPER");
		col9 = this->model->GetChildCollision("RX90_RGRIPPER");
		col10 = this->model->GetChildCollision("RX90_BASE");

		const std::vector<gazebo::physics::Contact*>& contacts = this->contactManager->GetContacts();
		unsigned int howManyContacts = contacts.size();
		if (contador==220 ) {
		//shows four, which are assumed to be between the gripper plates and the floor
		printf("This many contacts: %d\n", howManyContacts);
		contador=0;
}
		contador++;
		if(col1!=NULL || col2!=NULL || col3!=NULL || col4!=NULL || col5!=NULL || col6!=NULL || col7!=NULL || col8!=NULL || col9!=NULL || col10!=NULL) 
		{std::cout<<"Colision"<<std::endl;}
	}

  private:
	physics::ModelPtr model;
	physics::WorldPtr world;
	physics::CollisionPtr col1,col2,col3,col4,col5,col6,col7,col8,col9,col10;
	physics::LinkPtr link_col;
	physics::ContactManager cnt;
	physics::Collision_V vec_col;
	physics::LinkPtr link;
	physics::Link_V link_nombres;
	event::ConnectionPtr updateConnection;
	physics::LinkPtr link_positioner;
	physics::InertialPtr mass;
	ignition::math::Pose3d Pos;
	unsigned int number;
	physics::BasePtr chil;
	physics::ContactManager *contactManager;
	int contador=0;
	physics::Model_V nombres;
	std::string nombre_mundo;
	math::Pose static_position;
};

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(contact_plugin)
} // namespace gazebo
