#pragma once

#include <stdexcept>
#include <string>

class MissingDisplayException : public std::runtime_error {
public:
    // Constructor accepting a message string
    explicit MissingDisplayException(const std::string& message);
};
