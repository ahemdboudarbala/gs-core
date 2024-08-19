#include "GraphParseException.hpp"

GraphParseException::GraphParseException()
    : errorMessage("graph parse error") {}

GraphParseException::GraphParseException(const std::string& message)
    : errorMessage(message) {}

const char* GraphParseException::what() const noexcept {
    return errorMessage.c_str();
}
