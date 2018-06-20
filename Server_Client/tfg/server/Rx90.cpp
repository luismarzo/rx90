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

using namespace LibSerial;

Rx90::Rx90(const std::string& serialPort, const std::string& originPoint) {

	init(serialPort, originPoint);
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Rx90::~Rx90() { close(); }


void Rx90::init(const std::string& serialPort, const std::string& originPoint) {

	serial.Open( serialPort.c_str() );
	serial.SetBaudRate( SerialStreamBuf::BAUD_9600 );
	serial.SetParity( SerialStreamBuf::PARITY_NONE );
	serial.SetCharSize( SerialStreamBuf::CHAR_SIZE_8  );
	serial.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE );
	serial.SetNumOfStopBits( 1 );
	serial.unsetf( std::ios_base::skipws ) ;

  // Set origin precision point
	std::stringstream command_origin;
	command_origin << "DO SET #ORIGIN=#PPOINT(" << originPoint.c_str() << ")";
	sendCommand(command_origin.str());

	sendCommand("SPEED 30");
	sendCommand("DO ABOVE");
	sendCommand("DO MOVE #ORIGIN");
	sendCommand("HERE ORIGIN", true);
	sendCommand("DO OPENI");
	sendCommand( "DO ENABLE CP" );
}

void Rx90::close() { serial.Close(); }

void Rx90::sendCommand(const std::string& command, bool waitQuestionMark) {

	if( serial.IsOpen() ){

		serial << command << END;

		if(waitQuestionMark) {
			char qm;
			do { serial >> qm; } while(qm != '?');
			serial << END;
		}

		char r;
		do { serial >> r; std::cout << r; } while (r != '.');

	} else { std::cout << "Serial port not opened" << std::endl; }
}

void Rx90::move(const Action& action) {

	switch(action) {
		case NONE:
			break;
		case UP:
			z += DELTA_VH;
			break;
		case DOWN:
			z -= DELTA_VH;
			break;
		case RIGHT:
			x -= DELTA_VH;
			break;
		case LEFT:
			x += DELTA_VH;
			break;
		case BACKWARD:
			y += DELTA_VH;
			break;
		case FORWARD:
			y -= DELTA_VH;
			break;
		case DROP:
			dropIt();
			break;
		case CATCH:
			catchIt();
			break;
		default:
			std::cout << "unexpected!";
	}

	// send the command
	send(x,y,z);
}

void Rx90::send(double x, double y, double z){
	std::stringstream position;
	position << "DO SET P" << "=SHIFT(ORIGIN BY " << (int)x << "," << (int)y << "," << (int)z << ")";
	std::string command = position.str();
	sendCommand(command);
	sendCommand("DO MOVE P");
}

void Rx90::catchIt() { sendCommand("DO CLOSEI"); }

void Rx90::dropIt() { sendCommand("DO OPENI"); }

void Rx90::automove(vector<int> leePos) {
	ostringstream pos;
	pos << '"'<<uintToInt(leePos[0])<<','<<uintToInt(leePos[1])<<','<< uintToInt(leePos[2])<<','<<uintToInt(leePos[3])<<','<<uintToInt(leePos[4])<<','<<uintToInt(leePos[5])<<'"';
	printf("RX90 MOVES TO ");
	cout << pos.str();
	std::cout << '\n';
	//char cadenaPosicion[]="160,-150,10,-90,-90,0";
	std::stringstream posicion;
	posicion << "DO SET P=#PPOINT(" << pos.str() << ")";
	//??????????????????
	//sendCommand(posicion.str());
	sendCommand("DO MOVE P");
}

void Rx90::printAction(const Action& action) {
std::cout << "Rx90::printAction: ";
switch(action) {
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
	/*case UP_RIGHT:
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
	break;*/
	case BACKWARD:
		std::cout << "backward!";
		break;
	case FORWARD:
		std::cout << "forward!";
		break;
	case CATCH:
		std::cout << "catch!";
		break;
	case DROP:
		std::cout << "drop!";
		break;
	case AUTOMOVE:
		std::cout << "automove!";
		break;
	default:
		std::cout << "unexpected!";
	}
	std::cout << std::endl;
}

int Rx90::uintToInt(uint16_t value) {
int entero;
if (value>65176)
entero=-(65536-value);
else
entero=value;
return entero;
}


