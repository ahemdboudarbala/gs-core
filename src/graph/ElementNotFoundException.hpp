#ifndef ELEMENT_NOT_FOUND_EXCEPTION_HPP
#define ELEMENT_NOT_FOUND_EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <cstdarg>
#include <cstdio>
#include <vector>

class ElementNotFoundException : public std::runtime_error {
public:
    // Throws the message "not found".
    ElementNotFoundException()
        : std::runtime_error("not found") {}

    // Throws a formatted message.
    ElementNotFoundException(const std::string& message, ...) 
        : std::runtime_error(formatMessage(message)) {}

private:
    static std::string formatMessage(const std::string& format, ...) {
        va_list args;
        va_start(args, format);

        // Estimate the required buffer size.
        std::vector<char> buffer(256);
        int size = vsnprintf(buffer.data(), buffer.size(), format.c_str(), args);

        // Handle buffer resizing if necessary.
        if (size >= static_cast<int>(buffer.size())) {
            buffer.resize(size + 1);
            vsnprintf(buffer.data(), buffer.size(), format.c_str(), args);
        }

        va_end(args);

        return std::string(buffer.data());
    }
};

#endif // ELEMENT_NOT_FOUND_EXCEPTION_HPP
