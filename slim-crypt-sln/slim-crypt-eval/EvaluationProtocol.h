#pragma once
#include "SerialPort.h"

#include "messages/message_id.h"
#include "messages/commands.h"
#include "messages/responses.h"
#include "messages/data_transfers.h"

class EvaluationProtocol {
private:
	SerialPort& port;
	uint8_t transmission_buffer[2096];
	uint16_t last_msg_size;

public:
	EvaluationProtocol(SerialPort &port) : port(port) {}

	message_id await_message(void);

	void send_message(void* msg, message_id msg_id, size_t len);

	void get_message(void* msg);
};
