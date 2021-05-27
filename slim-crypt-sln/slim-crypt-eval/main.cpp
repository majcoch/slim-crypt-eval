#include <iostream>
#include "SerialPort.h"

int main(int argc, char* argv[]) {

	std::vector<std::string> avaliablePorts = SerialPort::getAvaliablePorts();
	if (avaliablePorts.size() < 2) return 0;

	try {
		SerialPort port(avaliablePorts[0], Baudrate::SP_19200, Frame::SP_8E2);
	}
	catch (const std::exception&) {
		std::cout << "Unable to open COM port";
	}
	
	return 0;
}
