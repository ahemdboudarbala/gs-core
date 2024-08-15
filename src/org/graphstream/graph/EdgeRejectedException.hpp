#ifndef EDGE_REJECTED_EXCEPTION_HPP
#define EDGE_REJECTED_EXCEPTION_HPP

#include <stdexcept>
#include <string>

class EdgeRejectedException : public std::runtime_error {
public:
    // Constructor with default message
    EdgeRejectedException()
        : std::runtime_error("Edge rejected") {}

    // Constructor with custom message
    EdgeRejectedException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif // EDGE_REJECTED_EXCEPTION_HPP
