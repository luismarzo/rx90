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
#include <iostream>
#include <vector>
#include <stdint.h>
#define DELTA_VH 50
#define END "\r\n"
using namespace std;
class Rx90 {

public:
Rx90(const std::string& serialPort, const std::string& originPoint);
~Rx90();

enum Action { NONE, UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT,DOWN_LEFT, DOWN_RIGHT, BACKWARD, FORWARD, CATCH, DROP, AUTOMOVE };
vector<int> leePos;
static void printAction(const Action& action);
void move(const Action& action);
void automove(vector<int> leePos);
void panic();

private:
void init(const std::string& serialPort, const std::string&
originPoint);
void close();
void catchIt();
void dropIt();
void send(double x, double y, double z);
void sendCommand(const std::string& command, bool waitQuestionMark = false);
int uintToInt(uint16_t value);
LibSerial::SerialStream serial;
double x, y, z;
uint16_t value;
};
#endif
// RX90_H
