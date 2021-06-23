#include "EvaluationData.h"

#include <fstream>

void load_data_file(std::filesystem::directory_entry entry, data_transfer_m& data) {
	std::ifstream data_file(entry.path(), std::ios::binary);
	if (data_file.good()) {

		data.data_len = entry.file_size();
		data_file.read((char*)data.data_buff, entry.file_size());

		data_file.close();
	}
}

EvaluationData::EvaluationData(std::filesystem::directory_entry plain, std::filesystem::directory_entry encrypted) {
	plain_data = { 0 };
	enc_data = { 0 };

	plain_path = plain;
	encrypted_path = encrypted;
}

void EvaluationData::load(std::string folder, std::string filename) {
	std::filesystem::directory_entry data(plain_path.path().string() + "/" + filename + ".txt");
	load_data_file(data, plain_data);

	std::filesystem::directory_entry encr(encrypted_path.path().string() + "/" + folder + "/" + filename + ".dat");
	load_data_file(encr, enc_data);
}

data_transfer_m EvaluationData::get_plain_data() const {
	return plain_data;
}

data_transfer_m EvaluationData::get_enc_data() const {
	return enc_data;
}
