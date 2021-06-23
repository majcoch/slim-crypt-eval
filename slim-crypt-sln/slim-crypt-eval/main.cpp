#include <fstream>
#include <iostream>
#include <filesystem>

#include "SerialPort.h"
#include "EvaluationProtocol.h"
#include "EvaluationProcessor.h"
#include "EvaluationData.h"

int main(int argc, char* argv[]) {
	if (4 == argc) {
		std::string com_port(argv[1]);
		std::filesystem::directory_entry plain_data_path(argv[2]);
		std::filesystem::directory_entry enc_data_path(argv[3]);

		if (!plain_data_path.exists() || !plain_data_path.is_directory()) {
			std::cout << "Plain data folder does not exist. Please make sure path is correct and try again!" << std::endl;
			return 0;
		}

		if (!enc_data_path.exists() || !enc_data_path.is_directory()) {
			std::cout << "Encrypted data folder does not exist. Please make sure path is correct and try again!" << std::endl;
			return 0;
		}
		
		// Evaluation results will be stored here
		std::vector<EvaluationResult> eval_results;

		try {
			// Open serial port
			SerialPort port(com_port, Baudrate::SP_9600, Frame::SP_8E1);

			// Create software protocol for messages exchange
			EvaluationProtocol protocol(port);

			// Create evaluation processor
			EvaluationProcessor processor(protocol);

			// Get input files
			EvaluationData eval_data(plain_data_path, enc_data_path);

			// Scan plain data directory
			for (const auto& entry : std::filesystem::directory_iterator(plain_data_path)) {
				// Extract file name only
				std::string file_name = entry.path().filename().string();
				file_name.erase(file_name.rfind('.'));

				try {
					std::cout << "Loading AES evaluation data...";
					eval_data.load("aes", file_name);
					std::cout << "Loading completed!" << std::endl << "Starting AES encyption evaluation...";
					processor.evaluate_encryption(eval_data, algorithm_e::AES);
					std::cout << "Evaluation completed!" << std::endl << "Starting AES decyption evaluation...";
					processor.evaluate_decryption(eval_data, algorithm_e::AES);
					std::cout << "Evaluation completed!" << std::endl;

					std::cout << "Loading DES evaluation data...";
					eval_data.load("des", file_name);
					std::cout << "Loading completed!" << std::endl << "Starting DES encyption evaluation...";
					processor.evaluate_encryption(eval_data, algorithm_e::DES);
					std::cout << "Evaluation completed!" << std::endl << "Starting DES decyption evaluation...";
					processor.evaluate_decryption(eval_data, algorithm_e::DES);
					std::cout << "Evaluation completed!" << std::endl;

					std::cout << "Loading TEA evaluation data...";
					eval_data.load("tea", file_name);
					std::cout << "Loading completed!" << std::endl << "Starting TEA encyption evaluation...";
					processor.evaluate_encryption(eval_data, algorithm_e::TEA);
					std::cout << "Evaluation completed!" << std::endl << "Starting TEA decyption evaluation...";
					processor.evaluate_decryption(eval_data, algorithm_e::TEA);
					std::cout << "Evaluation completed!" << std::endl;

					std::cout << "Loading BLOWFISH evaluation data...";
					eval_data.load("blowfish", file_name);
					std::cout << "Loading completed!" << std::endl << "Starting BLOWFISH encyption evaluation...";
					processor.evaluate_encryption(eval_data, algorithm_e::BLOWFISH);
					std::cout << "Evaluation completed!" << std::endl << "Starting BLOWFISH decyption evaluation...";
					processor.evaluate_decryption(eval_data, algorithm_e::BLOWFISH);
					std::cout << "Evaluation completed!" << std::endl;

					std::cout << "Loading SHA-1 evaluation data...";
					eval_data.load("sha-1", file_name);
					std::cout << "Loading completed!" << std::endl << "Starting SHA-1 hashing evaluation...";
					processor.evaluate_encryption(eval_data, algorithm_e::SHA_1);
					std::cout << "Evaluation completed!" << std::endl;
				}
				catch (const CommunicationParityException& e) {
					std::cout << "Communication error has occured. Data integrity has been compromised. Aborting execution now. Results will not be generated" << std::endl;
					return 0;
				}
				catch (const CommunicationFrameException& e) {
					std::cout << "Communication error has occured. Data integrity has been compromised. Aborting execution now. Results will not be generated" << std::endl;
					return 0;
				}
			}
			// Get collected results
			eval_results = processor.get_results();
		}
		catch (const PortUnavaliableException& e) {
			std::cout << "Selected serial port is unavailable. Please select existing port and try again!" << std::endl;
			return 0;
		}
		catch (const PortConfigurationException& e) {
			std::cout << "Error starting serial communication connection. " << e.what();
			return 0;
		}

		// Dump to result file
		std::ofstream result_file("results.csv");
		if (result_file.good()) {
			std::cout << "Result file created. Writing results now...";
			result_file << "Data size;Operation;Algorithm;Cycle count" << std::endl;
			for (auto& result : eval_results) {
				result_file << result.to_string() << std::endl;
			}
			result_file.close();
			std::cout << "Write completed! Exiting now!" << std::endl;
			return 0;
		}
		else {
			std::cout << "Unable to create result file. Results will not be saved. Exiting now!" << std::endl;
			return 0;
		}

	}
	else {
		std::cout << "Invalid number of pramateres. Please try again" << std::endl;
		return 0;
	}
}
