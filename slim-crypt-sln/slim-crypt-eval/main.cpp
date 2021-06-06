#include <iostream>
#include <fstream>

#include "SerialPort.h"
#include "evaluation.h"

void generate_data(uint8_t* buff, const size_t len) {
	srand(time(NULL));
	for (size_t i = 0; i < len; i++) {
		int r = rand() % 26;
		buff[i] = ('a' + r);
	}
}

int main(int argc, char* argv[]) {

	std::cout << "Starting encryption algorithms evaluation..." << std::endl;

	try {
		// Open serial port
		SerialPort port("COM4", Baudrate::SP_9600, Frame::SP_8E1);

		// Create software protocol for messages exchange
		EvaluationProtocol eval(port);

		for (size_t data_size = 32; data_size <= 512; data_size += 32) {
			std::cout << "Evaluation with block size [" << data_size << "]" << std::endl;

			data_message_t data_msg = { 0 };
			data_msg.data_len = data_size;
			generate_data(data_msg.data_buff, data_size);

			std::ofstream result_file("results/results_" + std::to_string(data_size) + "_bytes.csv");
			if (result_file.is_open()) {
				// Print file header
				result_file << "Algorithm;Encryption [cycles];Decryption [cycles]" << std::endl;

				std::cout << "AES evaluation started... ";
				evaluate_aes_algorithm(eval, data_msg, result_file);
				std::cout << "evaluation completed!" << std::endl;

				std::cout << "DES evaluation started... ";
				evaluate_des_algorithm(eval, data_msg, result_file);
				std::cout << "evaluation completed!" << std::endl;

				std::cout << "TEA evaluation started... ";
				evaluate_tea_algorithm(eval, data_msg, result_file);
				std::cout << "evaluation completed!" << std::endl;

				std::cout << "BLOWFISH evaluation started... ";
				evaluate_blowfish_algorithm(eval, data_msg, result_file);
				std::cout << "evaluation completed!" << std::endl;

				result_file.close();
			}
		}
	}
	catch (const std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}

	system("pause");
	return 0;
}

