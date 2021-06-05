#pragma once
#include <tuple>
#include <fstream>

#include "EvaluationProtocol.h"
#include "config/enc_config.h"
#include "messages/message.h"

void evaluate_aes_algorithm(EvaluationProtocol& eval, data_message_t& msg, std::ofstream& out);

void evaluate_des_algorithm(EvaluationProtocol& eval, data_message_t& msg, std::ofstream& out);

void evaluate_tea_algorithm(EvaluationProtocol& eval, data_message_t& msg, std::ofstream& out);

void evaluate_blowfish_algorithm(EvaluationProtocol& eval, data_message_t& msg, std::ofstream& out);

void evaluate_sha1_algorithm(EvaluationProtocol& eval, data_message_t& msg, std::ofstream& out);