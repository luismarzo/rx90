#include <hecatonquiros/Arm4DoF.h>
#define HAS_OPENRAVE
#include <hecatonquiros/model_solvers/ModelSolverOpenRave.h>
#include <Eigen/QR>
#include <iostream>
#include <thread>
#include <chrono>
#include <ros/ros.h>
#include <serial/serial.h>

int main(int _argc, char **_argv)
{
	if (_argc < 2)
	{
		printf("You need more input arguments\n");
		return 0;
	}
	else
	{

		ros::init(_argc, _argv, "rx90_ik");

		hecatonquiros::Backend::Config backendConfig1;
		hecatonquiros::Backend::Config backendConfig2;

		backendConfig1.type = hecatonquiros::Backend::Config::eType::Dummy;
		backendConfig2.type = hecatonquiros::Backend::Config::eType::Dummy;

		srand(time(NULL));
		hecatonquiros::ModelSolver::Config modelSolverConfig1;
		modelSolverConfig1.type = hecatonquiros::ModelSolver::Config::eType::OpenRave;
		// modelSolverConfig1.type = hecatonquiros::ModelSolver::Config::eType::Simple4DoF;
		modelSolverConfig1.robotName = "rx90";
		modelSolverConfig1.manipulatorName = "manipulator";
		modelSolverConfig1.robotFile = _argv[1];
		//modelSolverConfig1.environment = _argv[1];
		modelSolverConfig1.offset = {0.0, 0.0, 0.0};
		Eigen::Matrix3f m;
		m = Eigen::AngleAxisf(0 * M_PI, Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(1 * M_PI, Eigen::Vector3f::UnitY());
		Eigen::Quaternionf q(m);
		modelSolverConfig1.rotation = {q.w(), q.x(), q.y(), q.z()};
		modelSolverConfig1.visualizer = true;

		printf("debug4\n");
		hecatonquiros::Arm4DoF leftArm(modelSolverConfig1, backendConfig1);
		printf("debug5\n");
		leftArm.home();
		printf("debug6\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		hecatonquiros::Arm4DoF *armInUse = &leftArm;

		bool usingRight = false;
		std::cout << "USING Left ARM" << &armInUse << std::endl;

		float x, y, z;
		bool forceOri;
		std::cout << "position" << std::endl;
		std::cout << "x: " << std::endl;
		std::cin >> x;
		std::cout << "y: " << std::endl;
		std::cin >> y;
		std::cout << "z: " << std::endl;
		std::cin >> z;
		std::cout << "Force ori: " << std::endl;
		std::cin >> forceOri;
		Eigen::Matrix4f pose = Eigen::Matrix4f::Identity();
		pose(0, 3) = x;
		pose(1, 3) = y;
		pose(2, 3) = z;

		hecatonquiros::ModelSolver::IK_TYPE type;
		if (forceOri)
		{
			float dx, dy, dz;
			// std::cout << "dx: " << std::endl;
			// std::cin >> dx;
			// std::cout << "dy: " << std::endl;
			// std::cin >> dy;
			// std::cout << "dz: " << std::endl;
			// std::cin >> dz;
			// Eigen::Vector3f xAxis = {dx, dy, dz};
			// xAxis /=xAxis.norm();
			std::cout << "dx: " << std::endl;
			std::cin >> dx;
			std::cout << "dy: " << std::endl;
			std::cin >> dy;
			std::cout << "dz: " << std::endl;
			std::cin >> dz;
			Eigen::Vector3f zAxis = {dx, dy, dz};
			zAxis /= zAxis.norm();
			// zAxis -= zAxis.dot(xAxis)*xAxis;

			// Eigen::Vector3f yAxis = zAxis.cross(xAxis);
			// pose.block<3,1>(0,0) = xAxis;
			// pose.block<3,1>(0,1) = yAxis;
			pose.block<3, 1>(0, 2) = zAxis;
			type = hecatonquiros::ModelSolver::IK_TYPE::IK_6D;
		}
		else
		{
			type = hecatonquiros::ModelSolver::IK_TYPE::IK_3D;
		}
		std::vector<float> joints;
		auto start = std::chrono::high_resolution_clock::now();
		if (armInUse->checkIk(pose, joints, type))
		{
			armInUse->joints(joints, true);
		}
		else
		{
			std::cout << "Not found IK" << std::endl;
		}
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << duration.count() << std::endl;
	}
}