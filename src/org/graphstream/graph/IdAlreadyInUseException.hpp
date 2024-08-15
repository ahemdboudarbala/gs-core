#ifndef IDALREADYINUSEEXCEPTION_HPP
#define IDALREADYINUSEEXCEPTION_HPP

#include <stdexcept>
#include <string>

class IdAlreadyInUseException : public std::runtime_error {
public:
    // Default constructor with a predefined message
    IdAlreadyInUseException()
        : std::runtime_error("singleton exception") {}

    // Constructor with a custom message
    explicit IdAlreadyInUseException(const std::string& message)
        : std::runtime_error("singleton exception: " + message) {}
};

#endif // IDALREADYINUSEEXCEPTION_HPP
