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
// #include "/home/luis/catkin_ws/src/gazebo_ros_plugin/include/service_tutorial/Set_position.h"
// #include "/home/luis/catkin_ws/src/gazebo_ros_plugin/include/gazebo_ros_plugin/Set_activation.h"
// #include "service_tutorial/Set_position.h"
//#include "service_tutorial/Set_position.h"
//#include <LinkConfig.hh>

namespace gazebo
{
class gripper_plugin : public ModelPlugin
{
  public:
	void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
	{

		this->model = _parent;
		this->world = this->model->GetWorld();
		this->rx90 = this->world->GetModel("rx90_2");
		// this->number = this->world->GetModelCount();
		this->nombres = this->world->GetModels();
		// this->nombre_mundo = this->world->GetName();
		static_position = rx90->GetLink("RX90_MGRIPPER")->GetWorldPose();
		this->link_nombres = this->rx90->GetLinks();
		this->link_gripper = this->rx90->GetLink("RX90_MGRIPPER");
		// //   this->link_gripper->GetInertial()->SetMass(4); // Cambia la masa a 800000
		// //   this ->link_gripper ->UpdateMass();

		// archivo.open("Debugador_gripper.txt");
		// archivo << "\n"
		// 		<< this->rx90->GetName()
		// 		<< "\t" << model->GetName()
		// 		<< "\t" << this->number
		// 		<< "\t" << this->link_nombres[1]->GetName()
		// 		<< std::endl;


		// //Server
		// if (!ros::isInitialized())
		// {
		// 	ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
		// 					 << "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
		// 	return;
		// }

		// ROS_INFO("\n\nPluging activated!\n");
		// ros::NodeHandle n;
		// service = n.advertiseService("Set_position", &gripper_plugin::Model_pose, this);
		// autoservice = n.advertiseService("Set_autoposition", &gripper_plugin::Model_autopose, this);
		// activationservice = n.advertiseService("Set_activation", &gripper_plugin::activation, this);
		// ROS_INFO("Ready.");

		// this->updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&gripper_plugin::OnUpdate, this, _1));
	}

  public:
	void OnUpdate(const common::UpdateInfo &_info)
	{
		// if (set_activation == 1)
		// {	
		// 	//mover el dron entero
		// 	//math::Pose staticPose(posx,posy,posz,roll,pitch,yaw);
		// 	//this->rx90->SetWorldPose(staticPose);
		// 	//Setlinkstatic
		// 	math::Pose staticPose(static_position);
		// 	this->link_gripper->SetWorldPose(staticPose);
		// }
		// else{
		// 	static_position = rx90->GetLink("right/Positioner_Fija_BASE")->GetWorldPose();
		// }
	}

	// bool Model_pose(service_tutorial::Set_position::Request &req,
	// 				service_tutorial::Set_position::Response &res)
	// {
	// 	posx = res.resu1 = req.x;
	// 	posy = res.resu2 = req.y;
	// 	posz = res.resu3 = req.z;
	// 	roll = res.resu4 = req.roll;
	// 	pitch = res.resu5 = req.pitch;
	// 	yaw = res.resu6 = req.yaw;
	// 	//ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
	// 	ROS_INFO("sending back response: [%ld,%ld,%ld,%ld,%ld,%ld]", (long int)res.resu1,
	// 			 (long int)res.resu2,
	// 			 (long int)res.resu3,
	// 			 (long int)res.resu4,
	// 			 (long int)res.resu5,
	// 			 (long int)res.resu6);

	// 	return true;
	// }

	// bool Model_autopose(service_tutorial::Set_position::Request &req,
	// 					service_tutorial::Set_position::Response &res)
	// {

	// 	res.resu1 = 10;
	// 	res.resu2 = 20;
	// 	res.resu3 = 30;
	// 	res.resu4 = 40;
	// 	res.resu5 = 50;
	// 	res.resu6 = 60;
	// 	ROS_INFO("sending auto back response: [%ld,%ld,%ld,%ld,%ld,%ld]", (long int)res.resu1,
	// 			 (long int)res.resu2,
	// 			 (long int)res.resu3,
	// 			 (long int)res.resu4,
	// 			 (long int)res.resu5,
	// 			 (long int)res.resu6);
	// 	return true;
	// }

	// bool activation(gazebo_ros_plugin::Set_activation::Request &req,
	// 				gazebo_ros_plugin::Set_activation::Response &res)
	// {
	// 	set_activation = res.resu = req.activation;
	// 	ROS_INFO("request: activation=%ld, ", (long int)req.activation);
	// 	ROS_INFO("sending back response: [%ld]", (long int)res.resu);
	// 	return true;
	// }

  private:
	physics::ModelPtr model;
	physics::ModelPtr rx90;
	physics::WorldPtr world;
	event::ConnectionPtr updateConnection;
	std::ofstream archivo;
	physics::JointPtr jointR1_;
	physics::LinkPtr link_gripper;
	physics::InertialPtr mass;
	ignition::math::Pose3d Pos;
	ros::ServiceServer service;
	ros::ServiceServer autoservice;
	ros::ServiceServer activationservice;
	long int posx = 0, posy = 0, posz = 0, roll = 0, pitch = 0, yaw = 0;
	unsigned int number;
	physics::Model_V nombres;
	physics::Link_V link_nombres;
	std::string nombre_mundo;
	long int set_activation = 0;
	math::Pose static_position;
};

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(gripper_plugin)
} // namespace gazebo