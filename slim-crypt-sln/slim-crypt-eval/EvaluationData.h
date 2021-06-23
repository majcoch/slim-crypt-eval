#pragma once
#include <string>
#include <filesystem>

#include "messages/data_transfers.h"

class EvaluationData {

public:
	EvaluationData(std::filesystem::directory_entry plain, std::filesystem::directory_entry encrypted);

	void load(std::string folder, std::string filename);

	data_transfer_m get_plain_data() const;
	data_transfer_m get_enc_data() const;

private:
	std::filesystem::directory_entry plain_path;
	std::filesystem::directory_entry encrypted_path;

	data_transfer_m plain_data;
	data_transfer_m enc_data;
};

