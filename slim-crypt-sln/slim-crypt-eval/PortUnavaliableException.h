#pragma once

#include <exception>

class PortUnavaliableException : public std::exception {
public:
    const char* what() const throw () {
        return "Selected port is unavaliable";
    }
};

