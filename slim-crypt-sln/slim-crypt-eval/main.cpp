#include <iostream>
#include "SerialPort.h"

int main(int argc, char* argv[]) {

	std::vector<std::string> avaliablePorts = SerialPort::getAvaliablePorts();
	if (avaliablePorts.size() < 2) return 0;

	try {
		SerialPort port(avaliablePorts[0], Baudrate::SP_9600, Frame::SP_8N1);

		while (true) {
			char inBuffer[100] = { 0 };
			std::cin >> inBuffer;
			port.Write(inBuffer, strlen(inBuffer));

			char outBuffer[100] = { 0 };
			port.Read(outBuffer, 100);
			std::cout << outBuffer << std::endl;
		}

	}
	catch (const std::exception&) {
		std::cout << "Unable to open COM port";
	}
	
	return 0;
}
