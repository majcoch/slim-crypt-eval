#pragma once
#include "SerialPort.h"
#include "messages/message.h"

class EvaluationProtocol {
private:
	SerialPort& port;
	uint8_t transmission_buffer[512];

public:
	EvaluationProtocol(SerialPort &port) : port(port) {}

	message_id await_message(void);

	void send_message(void* msg, message_id msg_id);

	void get_message(void* msg, message_id msg_id);
};

