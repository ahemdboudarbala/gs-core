#include "InvalidSpriteIDException.hpp"

InvalidSpriteIDException::InvalidSpriteIDException(const std::string& message)
    : std::runtime_error(message) {}

InvalidSpriteIDException::InvalidSpriteIDException()
    : std::runtime_error("Invalid sprite ID: identifier contains a dot, which is not allowed.") {}
