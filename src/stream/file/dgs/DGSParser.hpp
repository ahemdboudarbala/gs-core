#ifndef DGS_PARSER_HPP
#define DGS_PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <memory>
#include <sstream>
#include "FileSourceDGS.hpp"
#include "ParseException.hpp"

enum class Token {
    AN, CN, DN, AE, CE, DE, CG, ST, CL, TF, EOF
};

class DGSParser {
public:
    DGSParser(std::shared_ptr<FileSourceDGS> dgs, std::istream& reader);
    void close();
    void open();
    void all();
    bool next();
    bool nextStep();

private:
    static constexpr int BUFFER_SIZE = 4096;
    static constexpr char ARRAY_OPEN = '{';
    static constexpr char ARRAY_CLOSE = '}';
    static constexpr char MAP_OPEN = '[';
    static constexpr char MAP_CLOSE = ']';

    std::istream& reader;
    int line, column;
    char buffer[BUFFER_SIZE];
    int pushback[10];
    int pushbackOffset;
    std::shared_ptr<FileSourceDGS> dgs;
    std::string sourceId;
    Token lastDirective;

    int nextChar();
    void pushbackChar(int c);
    void skipLine();
    void skipWhitespaces();
    void header();
    void attributes(ElementType type, const std::string& id);
    void attribute(ElementType type, const std::string& elementId);
    Token directive();
    std::string string();
    std::string id();
    std::shared_ptr<void> value(bool array);
    std::shared_ptr<void> map();
    std::shared_ptr<void> array();
    std::string color();
    ParseException parseException(const std::string& message);
};

#endif // DGS_PARSER_HPP
