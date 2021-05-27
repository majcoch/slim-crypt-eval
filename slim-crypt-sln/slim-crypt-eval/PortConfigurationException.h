#pragma once

#include <exception>

class PortConfigurationException : public std::exception {
public:
    const char* what() const throw () {
        return "Unable to configure port with selected options";
    }
};

