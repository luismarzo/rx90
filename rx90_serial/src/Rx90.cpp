//
// C++ Implementation: Rx90
//
// Description:
//
//
// <franroyal@yahoo.es>
//
//

#include "Rx90.h"
#include <sstream>
#include <cmath>
#include <stdexcept>

#define DELTA_VH 25
#define DELTA_DIAG (0.707 * DELTA_VH)
#define END "\r\n"

using namespace LibSerial;

Rx90::Rx90(const std::string &serialPort, const std::string &originPoint)
{

	init(serialPort, originPoint);
	x = 0.0;
	y = 0.0;
}

Rx90::~Rx90() { close(); }

void Rx90::init(const std::string &serialPort, const std::string &originPoint)
{

	serial.Open(serialPort.c_str());
	serial.SetBaudRate(SerialStreamBuf::BAUD_9600);
	serial.SetParity(SerialStreamBuf::PARITY_NONE);
	serial.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
	serial.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
	serial.SetNumOfStopBits(1);
	serial.unsetf(std::ios_base::skipws);

	// Set origin precision point
	std::stringstream command_origin;
	command_origin << "DO SET #ORIGIN=#PPOINT(" << originPoint.c_str() << ")";
	std::cout<<originPoint.c_str()<<std::endl;
	sendCommand(command_origin.str());

	sendCommand("SPEED 20"); 
	sendCommand("DO ABOVE");
	sendCommand("DO MOVE #ORIGIN");
	sendCommand("HERE ORIGIN", true);
	sendCommand("DO OPENI");
	sendCommand("DO ENABLE CP");
}

void Rx90::close() { serial.Close(); }

void Rx90::sendCommand(const std::string &command, bool waitQuestionMark)
{

	if (serial.IsOpen())
	{

		serial << command << END;

		if (waitQuestionMark)
		{
			char qm;
			do
			{
				serial >> qm;
			} while (qm != '?');
			serial << END;
		}

		char r;
		do
		{
			serial >> r;
			std::cout << r;
		} while (r != '.');
	}
	else
	{
		std::cout << "Serial port not opened" << std::endl;
	}
}

void Rx90::panic() { sendCommand("PANIC"); }

void Rx90::move(const Action &action)
{

	std::string Point;

	switch (action)
	{
	case NONE:
		break;
	case UP:
		y += DELTA_VH;
		break;
	case DOWN:
		y -= DELTA_VH;
		break;
	case RIGHT:
		x += DELTA_VH;
		break;
	case LEFT:
		x -= DELTA_VH;
		break;
	case UP_RIGHT:
		x += DELTA_DIAG;
		y += DELTA_DIAG;
		break;
	case UP_LEFT:
		x -= DELTA_DIAG;
		y += DELTA_DIAG;
		break;
	case DOWN_LEFT:
		x -= DELTA_DIAG;
		y -= DELTA_DIAG;
		break;
	case DOWN_RIGHT:
		x += DELTA_DIAG;
		y -= DELTA_DIAG;
		break;
	case CATCH:
		catchIt();
		break;
	case POSITION:
		std::cout << "\n Give me a position Example:122,-77,-19,55,45,-43" << std::endl;
		std::cin >> Point;
		std::cout << Point << std::endl;
		move_position(Point);
		break;
	default:
		std::cout << "unexpected!";
	}

	// send the command
	if (action != POSITION)
	{
		std::stringstream sstr;
		sstr << "DO SET P" << "=SHIFT(ORIGIN BY " << (int)x << "," << (int)y << ",0)";
		std::string command = sstr.str();
		sendCommand(command);
		sendCommand("SPEED 20");
		sendCommand("DO MOVES P");
	}
	else
	{
	}
}

void Rx90::move_position(const std::string &PPoint)
{
    /*std::string Point;
	Point = "100,-77,-43,55,45,-43";*/
	std::stringstream command_pose;
	command_pose << "DO SET #POSE=#PPOINT(" << PPoint.c_str() << ")";
	sendCommand(command_pose.str());
	sendCommand("SPEED 20");
	sendCommand("DO ABOVE");
	sendCommand("DO MOVE #POSE");
}

void Rx90::catchIt() { sendCommand("DO CLOSEI"); }

void Rx90::printAction(const Action &action)
{

	std::cout << "Rx90::printAction: ";
	switch (action)
	{
	case NONE:
		std::cout << "none!";
		break;
	case UP:
		std::cout << "up!";
		break;
	case DOWN:
		std::cout << "down!";
		break;
	case RIGHT:
		std::cout << "right!";
		break;
	case LEFT:
		std::cout << "left!";
		break;
	case UP_RIGHT:
		std::cout << "up-right!";
		break;
	case UP_LEFT:
		std::cout << "up-left!";
		break;
	case DOWN_LEFT:
		std::cout << "down-left!";
		break;
	case DOWN_RIGHT:
		std::cout << "down-right!";
		break;
	case CATCH:
		std::cout << "catch!";
		break;
	case POSITION:
		std::cout << "moving to a position!";
		break;
	default:
		std::cout << "unexpected!";
	}
	std::cout << std::endl;
}
