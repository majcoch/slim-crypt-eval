#include "SerialPort.h"
#include <atlstr.h>
#include <iostream>

SerialPort::SerialPort(std::string portName, Baudrate baudrate, Frame frameFormat) {
	hCOMPort = CreateFile(CString(portName.c_str()), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hCOMPort == INVALID_HANDLE_VALUE) throw PortUnavaliableException();

	DCB	COMPortConfiguration = { 0 };
	if (!GetCommState(hCOMPort, &COMPortConfiguration)) throw PortConfigurationException();
	COMPortConfiguration.BaudRate = (DWORD)baudrate;
	COMPortConfiguration.ByteSize = GET_BYTE_SIZE(frameFormat);
	COMPortConfiguration.StopBits = GET_STOP_BIT(frameFormat);
	COMPortConfiguration.Parity = GET_PARITY_BIT(frameFormat);

	COMPortConfiguration.fParity = TRUE;
	COMPortConfiguration.fAbortOnError = TRUE;

	if (!SetCommState(hCOMPort, &COMPortConfiguration)) throw PortConfigurationException();

	COMMTIMEOUTS COMPortTimeouts = { 0 };
	COMPortTimeouts.ReadIntervalTimeout = 1;
	COMPortTimeouts.ReadTotalTimeoutConstant = 1;
	COMPortTimeouts.ReadTotalTimeoutMultiplier = 1;
	COMPortTimeouts.WriteTotalTimeoutConstant = 0;
	COMPortTimeouts.WriteTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(hCOMPort, &COMPortTimeouts)) throw PortConfigurationException();
}

unsigned long SerialPort::Read(char* buffer, const size_t len) {
	unsigned long readBytes = 0;

	if (!ReadFile(hCOMPort, buffer, len, &readBytes, NULL)) {
		DWORD dwErrors;
		COMSTAT comStat;
		ClearCommError(hCOMPort, &dwErrors, &comStat);
		if (dwErrors & CE_RXPARITY) {
			throw CommunicationParityException();
		}
		else if (dwErrors & CE_FRAME) {
			throw CommunicationFrameException();
		}
	}

	return readBytes;
}

unsigned long SerialPort::Write(const char* buffer, const size_t len) {
	unsigned long writtenBytes = 0;

	if (!WriteFile(hCOMPort, buffer, len, &writtenBytes, NULL)) {
		DWORD dwErrors;
		COMSTAT comStat;
		ClearCommError(hCOMPort, &dwErrors, &comStat);
		if (dwErrors & CE_RXPARITY) {
			throw CommunicationParityException();
		}
		if (dwErrors & CE_FRAME) {
			throw CommunicationFrameException();
		}
	}

	return writtenBytes;
}

SerialPort::~SerialPort() {
	if (hCOMPort) {
		CloseHandle(hCOMPort);
		hCOMPort = NULL;
	}
}

std::vector<std::string> SerialPort::getAvaliablePorts() {
	std::vector<std::string> avaliablePortsList;

	CString serialPortName;
	COMMCONFIG comPortStructurePointer;
	unsigned long structureSize = sizeof(comPortStructurePointer);
	for (unsigned int i = 1; i < 10; i++) {
		serialPortName.Format(_T("COM%d"), i);
		if (GetDefaultCommConfig(serialPortName, &comPortStructurePointer, &structureSize)) {
			avaliablePortsList.push_back("COM" + std::to_string(i));
		}
	}

	return avaliablePortsList;
}