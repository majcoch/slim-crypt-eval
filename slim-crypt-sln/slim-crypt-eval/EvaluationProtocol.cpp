#include "EvaluationProtocol.h"
#include <cstring>

message_id EvaluationProtocol::await_message(void) {
	// ID LAYER
	uint8_t msg_id = 0;
	while( 0 == msg_id ) port.Read((char*)&msg_id, 1);

	// SIZE LAYER - read 2 bytes
	while( 0 == port.Read((char*)transmission_buffer, sizeof(uint16_t)));
	memcpy(&last_msg_size, transmission_buffer, sizeof(uint16_t));

	// PAYLOAD LAYER
	while (0 == port.Read((char*)transmission_buffer, last_msg_size * sizeof(uint8_t)));
	// At this point whole message is inside transmission_buffer
	// and can be easily serialized and deserialized

	// Send back response that message has been received
	uint8_t resp = 0xAA;
	port.Write((char*)&resp, 1);

	return (message_id)msg_id;
}

void EvaluationProtocol::send_message(void* msg, message_id msg_id, size_t len) {
	// ID LAYER
	transmission_buffer[0] = (uint8_t)msg_id;

	// SIZE LAYER
	memcpy(&transmission_buffer[1], &len, sizeof(uint16_t));

	// PAYLOAD LAYER
	memcpy(&transmission_buffer[3], msg, len);

	port.Write((char*)transmission_buffer, 3 + len);

	// Wait for ACK
	uint8_t resp = 0;
	while (resp != 0xAA) port.Read((char*)&resp, 1);
}

void EvaluationProtocol::get_message(void* msg) {
	memcpy(msg, transmission_buffer, last_msg_size * sizeof(uint8_t));
}
