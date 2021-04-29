#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <functional>
#include <thread>

enum class Baudrate {
	SP_110 = CBR_110,
	SP_300 = CBR_300,
	SP_600 = CBR_600,
	SP_1200 = CBR_1200,
	SP_2400 = CBR_2400,
	SP_4800 = CBR_4800,
	SP_9600 = CBR_9600,
	SP_14400 = CBR_14400,
	SP_19200 = CBR_19200,
	SP_38400 = CBR_38400,
	SP_56000 = CBR_56000,
	SP_57600 = CBR_57600,
	SP_115200 = CBR_115200,
	SP_128000 = CBR_128000,
	SP_256000 = CBR_256000
};

enum class Frame {
	SP_8N1,
	SP_8N2,
	SP_8E1,
	SP_8E2,
	SP_8O1,
	SP_8O2,
};

class SerialPort {
private:
	HANDLE hCOMPort;

public:
	SerialPort(std::string portName, Baudrate baudrate, Frame frameFormat);

	/* Raw bytes operations - blocking! */
	unsigned long Read(char* buffer, const size_t len);
	unsigned long Write(const char* buffer, const size_t len);

	~SerialPort();
	static std::vector<std::string> getAvaliablePorts();
};

