#include "MissingDisplayException.hpp"

// Constructor definition
MissingDisplayException::MissingDisplayException(const std::string& message)
    : std::runtime_error(message) {
}
