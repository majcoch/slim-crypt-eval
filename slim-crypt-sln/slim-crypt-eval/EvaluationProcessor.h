#pragma once
#include <vector>
#include "EvaluationProtocol.h"
#include "EvaluationData.h"
#include "EvaluationResult.h"

class EvaluationProcessor {

public:
	EvaluationProcessor(EvaluationProtocol& protocol);

	bool evaluate_encryption(EvaluationData data, algorithm_e algorithm);
	bool evaluate_decryption(EvaluationData data, algorithm_e algorithm);

	std::vector<EvaluationResult> get_results();

private:
	EvaluationProtocol& protocol;

	std::vector<EvaluationResult> evaluation_results;

	bool execute_algorithm(algorithm_e algorithm, operation_e operation);
	bool validate_data(data_transfer_m expected_data);
	void send_data(data_transfer_m raw_data);
	count_result_m get_measurement();
	data_transfer_m get_data();
};
