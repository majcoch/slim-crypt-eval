#pragma once
#include "EvaluationProtocol.h"

bool evaluate_aes_algorithm(EvaluationProtocol& eval, data_transfer_m data_msg, std::ofstream& out);

bool evaluate_des_algorithm(EvaluationProtocol& eval, data_transfer_m& data_msg, std::ofstream& out);

bool evaluate_tea_algorithm(EvaluationProtocol& eval, data_transfer_m& data_msg, std::ofstream& out);

bool evaluate_blowfish_algorithm(EvaluationProtocol& eval, data_transfer_m& data_msg, std::ofstream& out);

//void evaluate_sha1_algorithm(EvaluationProtocol& eval, data_transfer_m& data_msg, std::ofstream& out);
