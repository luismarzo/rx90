//
// C++ Interface: Rx90
//
// Description: 
//
//
// <franroyal@yahoo.es>
//
//

#ifndef RX90_H
#define RX90_H

#include <SerialStream.h>
#include <gazebo/physics/physics.hh>
#include <iostream>

class Rx90 {
public:
	Rx90(const std::string& serialPort, const std::string& originPoint);
	~Rx90();
	
	

  enum Action { NONE, UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, CATCH ,POSITION, OPEN, KNOW};
	static void printAction(const Action& action);
	void move(const Action& action);
	void panic();
	

private:
	void init(const std::string& serialPort, const std::string& originPoint);
	void close();
	void catchIt();
	void openIt();
	void rviz();
	void know_position();
	void rviz_check_collision(); //not necesary in our case
	void gazebo(float _j1, float _j2, float _j3, float _j4, float _j5, float _j6, char send);
	void move_position(const std::string &PPoint);
	void sendCommand(const std::string& command, bool waitQuestionMark = false);
	static void callb(ConstContactsPtr &_msg);
	LibSerial::SerialStream serial;
	double x, y;
	
	
};

#endif  // RX90_H

