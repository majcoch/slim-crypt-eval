#pragma once
#include "SerialPort.h"
#include "messages/data_message.h"
#include "messages/request_message.h"
#include "messages/result_message.h"

typedef enum {
	INVALID = 0x00,
	DATA_MSG = 0x01,
	REQU_MSG = 0x02,
	RESP_MSG = 0x03
} message_id;

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

