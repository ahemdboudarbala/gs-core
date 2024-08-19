#ifndef PARSERFACTORY_HPP
#define PARSERFACTORY_HPP

#include <memory>
#include <istream>
#include "Parser.hpp"  // Include the header file for the Parser interface

class ParserFactory {
public:
    // Pure virtual method to create a new parser from an input stream
    virtual std::shared_ptr<Parser> newParser(std::istream& input) = 0;

    // Virtual destructor to allow proper cleanup in derived classes
    virtual ~ParserFactory() = default;
};

#endif // PARSERFACTORY_HPP
