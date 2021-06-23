#pragma once
#include <string>
#include <messages/commands.h>
#include <messages/responses.h>

class EvaluationResult {
public:
	EvaluationResult(algorithm_e algorithm, operation_e operation, uint32_t count, uint32_t data_size);

	std::string to_string();

private:
	algorithm_e algorithm;
	operation_e operation;
	uint32_t count;
	uint32_t data_size;
};

