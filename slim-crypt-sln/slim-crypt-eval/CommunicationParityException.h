#pragma once

#include <exception>

class CommunicationParityException : public std::exception {
public:
    const char* what() const throw () {
        return "Parity error detected";
    }
};

