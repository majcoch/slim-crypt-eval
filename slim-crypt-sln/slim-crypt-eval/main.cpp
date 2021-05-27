#include <iostream>
#include "SerialPort.h"

int main(int argc, char* argv[]) {

	std::vector<std::string> avaliablePorts = SerialPort::getAvaliablePorts();

	try {
		SerialPort port("COM5", Baudrate::SP_19200, Frame::SP_8E2);
	}
	catch (const std::exception& ex) {
		std::cout << ex.what();
	}
	
	return 0;
}
