#include "SerialPort.h"
#include <atlstr.h>

unsigned long SerialPort::SerialRead(char* buffer, size_t num) {
	unsigned long readBytes = 0;

	OVERLAPPED overlapedSerialRead = { 0 };
	overlapedSerialRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (overlapedSerialRead.hEvent != NULL) {
		// Issue read operation
		if (!ReadFile(hCOMPort, buffer, num, NULL, &overlapedSerialRead)) {
			if (ERROR_IO_PENDING == GetLastError()) {
				bool fWaitingOnRead = true;
				while (fWaitingOnRead) {
					if (WAIT_OBJECT_0 == WaitForSingleObject(overlapedSerialRead.hEvent, 100)) {
						// Read completed successfully.
						// Reset flag so that another opertion can be issued.
						fWaitingOnRead = false;
						GetOverlappedResult(hCOMPort, &overlapedSerialRead, &readBytes, FALSE);
					}
				}
			}
		}
	}

	return readBytes;
}

unsigned long SerialPort::SerialWrite(const char* buffer, const size_t num) {
	unsigned long writtenBytes = 0;

	OVERLAPPED overlappedSerialWrite = { 0 };
	overlappedSerialWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (overlappedSerialWrite.hEvent != NULL) {
		// Issue write operation
		if (!WriteFile(hCOMPort, buffer, num, &writtenBytes, &overlappedSerialWrite)) {
			if (GetLastError() == ERROR_IO_PENDING) {
				if (WAIT_OBJECT_0 == WaitForSingleObject(overlappedSerialWrite.hEvent, INFINITE)) {
					GetOverlappedResult(hCOMPort, &overlappedSerialWrite, &writtenBytes, FALSE);
				}
			}
		}
	}

	return writtenBytes;
}

SerialPort::SerialPort(std::string portName, Baudrate baudrate, Frame frameFormat) {
	hCOMPort = CreateFile(CString(portName.c_str()), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
	if (hCOMPort == INVALID_HANDLE_VALUE) throw std::exception();

	DCB	COMPortConfiguration = { 0 };
	if (!GetCommState(hCOMPort, &COMPortConfiguration)) throw std::exception();
	COMPortConfiguration.BaudRate = (DWORD)baudrate;
	COMPortConfiguration.ByteSize = 8;
	COMPortConfiguration.StopBits = 0;
	COMPortConfiguration.Parity = 0;
	if (!SetCommState(hCOMPort, &COMPortConfiguration)) throw std::exception();

	COMMTIMEOUTS COMPortTimeouts = { 0 };
	COMPortTimeouts.ReadIntervalTimeout = 30;
	COMPortTimeouts.ReadTotalTimeoutConstant = 30;
	COMPortTimeouts.ReadTotalTimeoutMultiplier = 30;
	COMPortTimeouts.WriteTotalTimeoutConstant = 30;
	COMPortTimeouts.WriteTotalTimeoutMultiplier = 30;
	if (!SetCommTimeouts(hCOMPort, &COMPortTimeouts)) throw std::exception();
}

void SerialPort::Read(char* buffer, const size_t len) {
	while (0 == SerialRead(buffer, len));
}

void SerialPort::Write(const char* data, const size_t len) {
	SerialWrite(data, len);
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