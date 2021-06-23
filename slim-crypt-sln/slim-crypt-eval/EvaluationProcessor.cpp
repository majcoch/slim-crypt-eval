#include "EvaluationProcessor.h"

EvaluationProcessor::EvaluationProcessor(EvaluationProtocol& protocol) : protocol(protocol) {
}

bool EvaluationProcessor::evaluate_encryption(EvaluationData data, algorithm_e algorithm) {
	count_result_m	measure = { 0 };
	bool result = false;

	// Prepare - send data to be encrypted to the device
	send_data(data.get_plain_data());

	if (execute_algorithm(algorithm, operation_e::ENCRYPTION)) {
		result = validate_data(data.get_enc_data());
		if (result) {
			measure = get_measurement();
			EvaluationResult res(algorithm, operation_e::ENCRYPTION, measure.count, data.get_plain_data().data_len);
			evaluation_results.push_back(res);
		}
	}
	return result;
}

bool EvaluationProcessor::evaluate_decryption(EvaluationData data, algorithm_e algorithm) {
	count_result_m	measure = { 0 };
	bool result = false;

	// Prepare - send data to be encrypted to the device
	send_data(data.get_enc_data());

	if (execute_algorithm(algorithm, operation_e::DECRYPTION)) {
		result = validate_data(data.get_plain_data());
		if (result) {
			measure = get_measurement();
			EvaluationResult res(algorithm, operation_e::DECRYPTION, measure.count, data.get_enc_data().data_len);
			evaluation_results.push_back(res);
		}
	}
	return result;
}

std::vector<EvaluationResult> EvaluationProcessor::get_results() {
	return evaluation_results;
}

void EvaluationProcessor::send_data(data_transfer_m raw_data) {
	protocol.send_message(&raw_data, message_id::DATA_TRANSFER, DATA_TRANSFER_SIZE(raw_data.data_len));
}

bool EvaluationProcessor::execute_algorithm(algorithm_e algorithm, operation_e operation) {
	message_id id = message_id::INVALID;
	execute_algorithm_cmd_m exe_alg_cmd = { algorithm, operation };
	execution_status_m exe_status_msg = { 0 };

	// Request algorithm execution
	protocol.send_message(&exe_alg_cmd, message_id::EXEC_ALGO_CMD, EXEC_ALGO_CMD_SIZE);

	// Get execution completeness status
	id = protocol.await_message();
	protocol.get_message(&exe_status_msg);

	return exe_status_msg.status;
}

data_transfer_m EvaluationProcessor::get_data() {
	message_id id = message_id::INVALID;
	send_data_cmd_m send_data_cmd;
	data_transfer_m data_transfer_msg = { 0 };

	// Request data after algorithm executed
	protocol.send_message(&send_data_cmd, message_id::SEND_DATA_CMD, 0);

	id = protocol.await_message();
	protocol.get_message(&data_transfer_msg);

	return data_transfer_msg;
}

count_result_m EvaluationProcessor::get_measurement() {
	message_id id = message_id::INVALID;
	send_count_cmd_m send_count_cmd;
	count_result_m count_result_msg = { 0 };

	// Request execution measurement data
	protocol.send_message(&send_count_cmd, message_id::SEND_CNT_CMD, 0);

	id = protocol.await_message();
	protocol.get_message(&count_result_msg);

	return count_result_msg;
}

bool EvaluationProcessor::validate_data(data_transfer_m expected_data) {
	const int retry_max = 2;
	bool comparison_result = false;
	data_transfer_m device_data = { 0 };

	int retry_cnt = 0;
	while ((!comparison_result) && (retry_cnt++ < retry_max)) {
		device_data = get_data();
		comparison_result = !memcmp(expected_data.data_buff, device_data.data_buff, expected_data.data_len * sizeof(uint8_t));
	}

	return comparison_result;
}
