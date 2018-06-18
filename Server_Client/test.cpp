#include "Rx90.h"

int main( int argc, char** argv ) {

	// Rx90
	Rx90 rx90("/dev/ttyUSB0", "122,-77,-43,55,45,-43");
  char key;

  bool  play = true;
	while(play) {

    Rx90::Action action = Rx90::NONE;
		std::cin >> key;
    switch(key) {
      case 'w':
        action = Rx90::UP;
        break;
      case 'a':
        action = Rx90::LEFT;
        break;
      case 's':
        action = Rx90::DOWN;
        break;
      case 'd':
        action = Rx90::RIGHT;
        break;
      case 'e':
        action = Rx90::CATCH;
        break;
      case 'q':
        play = false;
        break;
      default:
        std::cout << "Usage> w:up, s:down, a:left, d:right, e:catch, q:quit" << std::endl;
    }

    Rx90::printAction(action);
		rx90.move(action);
	}

}

