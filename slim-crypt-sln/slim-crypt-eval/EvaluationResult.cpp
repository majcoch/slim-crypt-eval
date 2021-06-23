#include "EvaluationResult.h"

std::string operation_to_string(operation_e operation) {
    switch (operation) {
        case operation_e::ENCRYPTION: return "ENCRYPTION";
        case operation_e::DECRYPTION: return "DECRYPTION";
        default: return "UNKNOWN";
    }
}

std::string algorithm_to_string(algorithm_e algorithm) {
    switch (algorithm) {
        case algorithm_e::AES : return "AES";
        case algorithm_e::BLOWFISH : return "BLOWFISH";
        case algorithm_e::DES : return "DES";
        case algorithm_e::SHA_1 : return "SHA_1";
        case algorithm_e::TEA : return "TEA";
        default: return "UNKNOWN";
    }
}

EvaluationResult::EvaluationResult(algorithm_e algorithm, operation_e operation, uint32_t count, uint32_t data_size) {
    this->algorithm = algorithm;
    this->operation = operation;
    this->count = count;
    this->data_size = data_size;
}

std::string EvaluationResult::to_string() {
    return std::to_string(data_size) + ";" + operation_to_string(operation) + ";" + algorithm_to_string(algorithm) + ";" + std::to_string(count);
}
