#include "evaluation.h"

std::tuple<data_message_t, result_message_t>
evaluate_algorithm(EvaluationProtocol& eval, encryption_algorithm alg, encryption_operation oper) {
	message_id id = (message_id)0; // Set msg id to invalid value

	// Evalueate encryption
	request_message_t enc_req = { alg, oper };
	eval.send_message(&enc_req, message_id::MSG_2);

	// Get encryption results
	id = eval.await_message();
	data_message_t enc_data = { 0 };
	eval.get_message(&enc_data, message_id::MSG_1);

	// Get measurement
	id = eval.await_message();
	result_message_t enc_measure = { 0 };
	eval.get_message(&enc_measure, message_id::MSG_3);

	return std::make_tuple(enc_data, enc_measure);
}

void evaluate_aes_algorithm(EvaluationProtocol& eval, data_message_t& msg, std::ofstream& out) {
	aes_128_context_t aes = { { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, { 0 } };
	data_message_t tmp_msg = msg;
	aes_128_init(&aes);

	// Evaluate encryption
	std::tuple<data_message_t, result_message_t> enc_res = evaluate_algorithm(eval,
		encryption_algorithm::AES, encryption_operation::ENCRYPT);
	// Validate results
	aes_128_encrypt(&aes, tmp_msg.data_buff, tmp_msg.data_len);
	bool enc_result = !memcmp(tmp_msg.data_buff, std::get<0>(enc_res).data_buff, tmp_msg.data_len * sizeof(uint8_t));

	// Evaluate decryption
	std::tuple<data_message_t, result_message_t> dec_res = evaluate_algorithm(eval,
		encryption_algorithm::AES, encryption_operation::DECRYPT);
	// Validate results
	aes_128_decrypt(&aes, tmp_msg.data_buff, tmp_msg.data_len);
	bool dec_result = !memcmp(tmp_msg.data_buff, std::get<0>(dec_res).data_buff, tmp_msg.data_len * sizeof(uint8_t));

	// Save results to file
	out << "AES;"
		<< (enc_result ? std::to_string(std::get<1>(enc_res).cycle_count) : "-") << ";"
		<< (dec_result ? std::to_string(std::get<1>(dec_res).cycle_count) : "-") << std::endl;
}

void evaluate_des_algorithm(EvaluationProtocol& eval, data_message_t& msg, std::ofstream& out) {
	des_context_t des = { 0xAABB09182736CCDD, {0} };
	data_message_t tmp_msg = msg;
	des_init(&des);

	// Evaluate encryption
	std::tuple<data_message_t, result_message_t> enc_res = evaluate_algorithm(eval,
		encryption_algorithm::DES, encryption_operation::ENCRYPT);
	// Validate results
	des_encrypt(&des, tmp_msg.data_buff, tmp_msg.data_len);
	bool enc_result = !memcmp(tmp_msg.data_buff, std::get<0>(enc_res).data_buff, tmp_msg.data_len * sizeof(uint8_t));

	// Evaluate decryption
	std::tuple<data_message_t, result_message_t> dec_res = evaluate_algorithm(eval,
		encryption_algorithm::DES, encryption_operation::DECRYPT);
	// Validate results
	des_decrypt(&des, tmp_msg.data_buff, tmp_msg.data_len);
	bool dec_result = !memcmp(tmp_msg.data_buff, std::get<0>(dec_res).data_buff, tmp_msg.data_len * sizeof(uint8_t));

	// Save results to file
	out << "DES;"
		<< (enc_result ? std::to_string(std::get<1>(enc_res).cycle_count) : "-") << ";"
		<< (dec_result ? std::to_string(std::get<1>(dec_res).cycle_count) : "-") << std::endl;
}

void evaluate_tea_algorithm(EvaluationProtocol& eval, data_message_t& msg, std::ofstream& out) {
	tea_context_t tea = { {0x12, 0x65, 0x22, 0x55} };
	data_message_t tmp_msg = msg;
	// Evaluate encryption
	std::tuple<data_message_t, result_message_t> enc_res = evaluate_algorithm(eval,
		encryption_algorithm::TEA, encryption_operation::ENCRYPT);
	// Validate results
	tea_encrypt(&tea, tmp_msg.data_buff, tmp_msg.data_len);
	bool enc_result = !memcmp(tmp_msg.data_buff, std::get<0>(enc_res).data_buff, tmp_msg.data_len * sizeof(uint8_t));

	// Evaluate decryption
	std::tuple<data_message_t, result_message_t> dec_res = evaluate_algorithm(eval,
		encryption_algorithm::TEA, encryption_operation::DECRYPT);

	// Validate results
	tea_decrypt(&tea, tmp_msg.data_buff, tmp_msg.data_len);
	bool dec_result = !memcmp(tmp_msg.data_buff, std::get<0>(dec_res).data_buff, tmp_msg.data_len * sizeof(uint8_t));

	// Save results to file
	out << "TEA;"
		<< (enc_result ? std::to_string(std::get<1>(enc_res).cycle_count) : "-") << ";"
		<< (dec_result ? std::to_string(std::get<1>(dec_res).cycle_count) : "-") << std::endl;
}

void evaluate_blowfish_algorithm(EvaluationProtocol& eval, data_message_t& msg, std::ofstream& out) {
	//uint32_t key[] = { 0x01, 0x02 };
	data_message_t tmp_msg = msg;
	//blowfish_init(key, 2);

	// Evaluate encryption
	std::tuple<data_message_t, result_message_t> enc_res = evaluate_algorithm(eval,
		encryption_algorithm::BLOWFISH, encryption_operation::ENCRYPT);
	// Validate results
	blowfish_encrypt(tmp_msg.data_buff, tmp_msg.data_len);
	bool enc_result = !memcmp(tmp_msg.data_buff, std::get<0>(enc_res).data_buff, tmp_msg.data_len * sizeof(uint8_t));

	// Evaluate decryption
	std::tuple<data_message_t, result_message_t> dec_res = evaluate_algorithm(eval,
		encryption_algorithm::BLOWFISH, encryption_operation::DECRYPT);
	// Validate results
	blowfish_decrypt(tmp_msg.data_buff, tmp_msg.data_len);
	bool dec_result = !memcmp(tmp_msg.data_buff, std::get<0>(dec_res).data_buff, tmp_msg.data_len * sizeof(uint8_t));

	// Save results to file
	out << "BLOWFISH;"
		<< (enc_result ? std::to_string(std::get<1>(enc_res).cycle_count) : "-") << ";"
		<< (dec_result ? std::to_string(std::get<1>(dec_res).cycle_count) : "-") << std::endl;
}
