#pragma once

#include <Windows.h>
#include <string>
#include <vector>


#define BYTE_MASK	0b11110000	// 4 - 8
#define PARI_MASK	0b00001100	// 0 -> None,  1-> Odd,  2 -> Even
#define SBIT_MASK	0b00000011	// 0 -> 1,  1 -> 1.5,  2 -> 2

#define GET_BYTE_SIZE(x)	(((uint8_t)x & BYTE_MASK )) >> 4
#define GET_PARITY_BIT(x)	(((uint8_t)x & PARI_MASK )) >> 2
#define GET_STOP_BIT(x)		(((uint8_t)x & SBIT_MASK )) >> 0

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
	SP_8N1 = 0b10000000,
	SP_8N2 = 0b10000010,
	SP_8O1 = 0b10000100,
	SP_8O2 = 0b10000110,
	SP_8E1 = 0b10001000,
	SP_8E2 = 0b10001010
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

