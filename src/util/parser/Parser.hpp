#ifndef PARSER_HPP
#define PARSER_HPP

#include <stdexcept>
#include <istream>

class ParseException : public std::runtime_error {
public:
    explicit ParseException(const std::string& message)
        : std::runtime_error(message) {}
};

class Parser {
public:
    virtual ~Parser() = default;

    // Parse the whole stream
    virtual void all() = 0;

    // Start parsing the stream
    virtual void open() = 0;

    // Parse the next element
    virtual bool next() = 0;

    // Close the stream
    virtual void close() = 0;
};

#endif // PARSER_HPP
