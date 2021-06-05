#include <iostream>
#include <fstream>

#include "SerialPort.h"
#include "evaluation.h"


int main(int argc, char* argv[]) {

	std::cout << "Starting encryption algorithms evaluation..." << std::endl;

	try {
		// Open serial port
		SerialPort port("COM4", Baudrate::SP_19200, Frame::SP_8N1);

		// Create software protocol for messages exchange
		EvaluationProtocol eval(port);

		// Prepare - send data to be encrypted to the device
		data_message_t data_msg = { 32, "Hello World! hell do some work" };
		eval.send_message(&data_msg, message_id::MSG_1);

		std::ofstream result_file("results.csv");
		if (result_file.is_open()) {

			// Print file header
			result_file << "Algorithm;Encryption [cycles];Decryption [cycles]" << std::endl;

			evaluate_aes_algorithm(eval, data_msg, result_file);

			evaluate_des_algorithm(eval, data_msg, result_file);

			evaluate_tea_algorithm(eval, data_msg, result_file);

			evaluate_blowfish_algorithm(eval, data_msg, result_file);

			result_file.close();
		}
	}
	catch (const std::exception&) {

	}

	return 0;
}

