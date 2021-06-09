#include "evaluation.h"

#include <fstream>
#include "config/enc_config.h"

void device_send_data(EvaluationProtocol& eval, data_transfer_m& data_msg) {
	eval.send_message(&data_msg, message_id::DATA_TRANSFER, DATA_TRANSFER_SIZE(data_msg.data_len));
}

bool device_execute_algorithm(EvaluationProtocol& eval, algorithm_e alg, operation_e oper) {
	message_id id = message_id::INVALID;
	execute_algorithm_cmd_m exe_alg_cmd = { alg, oper };
	execution_status_m exe_status_msg = { 0 };

	// Request algorithm execution
	eval.send_message(&exe_alg_cmd, message_id::EXEC_ALGO_CMD, EXEC_ALGO_CMD_SIZE);

	// Get execution completeness status
	id = eval.await_message();
	eval.get_message(&exe_status_msg);

	return exe_status_msg.status;
}

data_transfer_m device_get_data(EvaluationProtocol& eval) {
	message_id id = message_id::INVALID;
	send_data_cmd_m send_data_cmd;
	data_transfer_m data_transfer_msg = { 0 };

	// Request data after algorithm executed
	eval.send_message(&send_data_cmd, message_id::SEND_DATA_CMD, 0);

	id = eval.await_message();
	eval.get_message(&data_transfer_msg);

	return data_transfer_msg;
}

count_result_m device_get_measurement(EvaluationProtocol& eval) {
	message_id id = message_id::INVALID;
	send_count_cmd_m send_count_cmd;
	count_result_m count_result_msg = { 0 };

	// Request execution measurement data
	eval.send_message(&send_count_cmd, message_id::SEND_CNT_CMD, 0);

	id = eval.await_message();
	eval.get_message(&count_result_msg);

	return count_result_msg;
}

bool validate_data(EvaluationProtocol& eval, data_transfer_m& expected_data) {
	const int retry_max = 2;
	bool comparison_result = false;
	data_transfer_m device_data = { 0 };

	int retry_cnt = 0;
	while ( (!comparison_result) && (retry_cnt++ < retry_max)) {
		device_data = device_get_data(eval);
		comparison_result = !memcmp(expected_data.data_buff, device_data.data_buff, expected_data.data_len * sizeof(uint8_t));
	}

	return comparison_result;
}

bool evaluate_aes_algorithm(EvaluationProtocol& eval, data_transfer_m data_msg, std::ofstream& out) {
	count_result_m	enc_measure = { 0 };
	count_result_m	dec_measure = { 0 };
	bool enc_result = false;
	bool dec_result = false;

	aes_128_init(&aes);

	// Prepare - send data to be encrypted to the device
	device_send_data(eval, data_msg);

	// Evaluate encryption
	if (device_execute_algorithm(eval, algorithm_e::AES, operation_e::ENCRYPTION)) {
		aes_128_encrypt(&aes, data_msg.data_buff, data_msg.data_len);
		enc_result = validate_data(eval, data_msg);
		if (enc_result) {
			enc_measure = device_get_measurement(eval);
		}
	}
	
	// Evaluate decryption
	if (device_execute_algorithm(eval, algorithm_e::AES, operation_e::DECRYPTION)) {
		aes_128_decrypt(&aes, data_msg.data_buff, data_msg.data_len);
		dec_result = validate_data(eval, data_msg);
		if (dec_result) {
			dec_measure = device_get_measurement(eval);
		}
	}
	
	//Save results to file
	out << "AES;"
		<< (enc_result ? std::to_string(enc_measure.count) : "-") << ";"
		<< (dec_result ? std::to_string(dec_measure.count) : "-") << std::endl;

	return enc_result && dec_result;
}

bool evaluate_des_algorithm(EvaluationProtocol& eval, data_transfer_m& data_msg, std::ofstream& out) {
	count_result_m	enc_measure = { 0 };
	count_result_m	dec_measure = { 0 };
	bool enc_result = false;
	bool dec_result = false;

	des_init(&des);

	// Prepare - send data to be encrypted to the device
	device_send_data(eval, data_msg);

	// Evaluate encryption
	if (device_execute_algorithm(eval, algorithm_e::DES, operation_e::ENCRYPTION)) {
		des_encrypt(&des, data_msg.data_buff, data_msg.data_len);
		enc_result = validate_data(eval, data_msg);
		if (enc_result) {
			enc_measure = device_get_measurement(eval);
		}
	}

	// Evaluate decryption
	if (device_execute_algorithm(eval, algorithm_e::DES, operation_e::DECRYPTION)) {
		des_decrypt(&des, data_msg.data_buff, data_msg.data_len);
		dec_result = validate_data(eval, data_msg);
		if (dec_result) {
			dec_measure = device_get_measurement(eval);
		}
	}

	//Save results to file
	out << "DES;"
		<< (enc_result ? std::to_string(enc_measure.count) : "-") << ";"
		<< (dec_result ? std::to_string(dec_measure.count) : "-") << std::endl;

	return enc_result && dec_result;
}

bool evaluate_tea_algorithm(EvaluationProtocol& eval, data_transfer_m& data_msg, std::ofstream& out) {
	count_result_m	enc_measure = { 0 };
	count_result_m	dec_measure = { 0 };
	bool enc_result = false;
	bool dec_result = false;

	// Prepare - send data to be encrypted to the device
	device_send_data(eval, data_msg);

	// Evaluate encryption
	if (device_execute_algorithm(eval, algorithm_e::TEA, operation_e::ENCRYPTION)) {
		tea_encrypt(&tea, data_msg.data_buff, data_msg.data_len);
		enc_result = validate_data(eval, data_msg);
		if (enc_result) {
			enc_measure = device_get_measurement(eval);
		}
	}

	// Evaluate decryption
	if (device_execute_algorithm(eval, algorithm_e::TEA, operation_e::DECRYPTION)) {
		tea_decrypt(&tea, data_msg.data_buff, data_msg.data_len);
		dec_result = validate_data(eval, data_msg);
		if (dec_result) {
			dec_measure = device_get_measurement(eval);
		}
	}

	//Save results to file
	out << "TEA;"
		<< (enc_result ? std::to_string(enc_measure.count) : "-") << ";"
		<< (dec_result ? std::to_string(dec_measure.count) : "-") << std::endl;

	return enc_result && dec_result;
}

bool evaluate_blowfish_algorithm(EvaluationProtocol& eval, data_transfer_m& data_msg, std::ofstream& out) {
	count_result_m	enc_measure = { 0 };
	count_result_m	dec_measure = { 0 };
	bool enc_result = false;
	bool dec_result = false;

	//blowfish_init(key, 2);

	// Prepare - send data to be encrypted to the device
	device_send_data(eval, data_msg);

	// Evaluate encryption
	if (device_execute_algorithm(eval, algorithm_e::BLOWFISH, operation_e::ENCRYPTION)) {
		blowfish_encrypt(data_msg.data_buff, data_msg.data_len);
		enc_result = validate_data(eval, data_msg);
		if (enc_result) {
			enc_measure = device_get_measurement(eval);
		}
	}

	// Evaluate decryption
	if (device_execute_algorithm(eval, algorithm_e::BLOWFISH, operation_e::DECRYPTION)) {
		blowfish_decrypt(data_msg.data_buff, data_msg.data_len);
		dec_result = validate_data(eval, data_msg);
		if (dec_result) {
			dec_measure = device_get_measurement(eval);
		}
	}

	//Save results to file
	out << "BLOWFISH;"
		<< (enc_result ? std::to_string(enc_measure.count) : "-") << ";"
		<< (dec_result ? std::to_string(dec_measure.count) : "-") << std::endl;

	return enc_result && dec_result;
}

bool evaluate_sha1_algorithm(EvaluationProtocol& eval, data_transfer_m& data_msg, std::ofstream& out) {
	count_result_m	hash_measure = { 0 };
	bool hash_result = false;

	// Prepare - send data to be hashed to the device
	device_send_data(eval, data_msg);

	// Evaluate hashing
	if (device_execute_algorithm(eval, algorithm_e::SHA_1, operation_e::ENCRYPTION)) {

		uint32_t hash[5] = { 0 };
		sha1_hash(data_msg.data_buff, data_msg.data_len, hash);

		data_transfer_m exp = { 0 };
		memcpy(exp.data_buff, hash, 5 * sizeof(uint32_t));
		exp.data_len = 5 * sizeof(uint32_t);

		hash_result = validate_data(eval, exp);
		if (hash_result) {
			hash_measure = device_get_measurement(eval);
		}
	}

	out << "SHA-1;"
		<< (hash_result ? std::to_string(hash_measure.count) : "-") << ";"
		<< "N/A" << std::endl;

	return hash_result;
}
