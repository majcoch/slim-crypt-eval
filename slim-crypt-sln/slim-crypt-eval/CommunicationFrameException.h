#pragma once

#include <exception>

class CommunicationFrameException : public std::exception {
public:
    const char* what() const throw () {
        return "Invalid frame received";
    }
};

