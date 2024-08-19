#ifndef GRAPH_PARSE_EXCEPTION_HPP
#define GRAPH_PARSE_EXCEPTION_HPP

#include <exception>
#include <string>

/**
 * Raised when a parse error occurs while reading a graph file.
 */
class GraphParseException : public std::exception {
public:
    /**
     * Default constructor with a generic error message.
     */
    GraphParseException();

    /**
     * Constructor with a custom error message.
     * 
     * @param message Custom error message.
     */
    explicit GraphParseException(const std::string& message);

    /**
     * Returns the error message.
     * 
     * @return A C-string representing the error message.
     */
    const char* what() const noexcept override;

private:
    std::string errorMessage;
};

#endif // GRAPH_PARSE_EXCEPTION_HPP
