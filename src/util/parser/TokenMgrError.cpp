#include "TokenMgrError.hpp"
#include <sstream>
#include <iomanip>

TokenMgrError::TokenMgrError() : std::runtime_error(""), errorCode(0) {}

TokenMgrError::TokenMgrError(const std::string& message, int reason) : std::runtime_error(message), errorCode(reason) {}

TokenMgrError::TokenMgrError(bool EOFSeen, int lexState, int errorLine, int errorColumn, const std::string& errorAfter, char curChar, int reason)
    : std::runtime_error(LexicalError(EOFSeen, lexState, errorLine, errorColumn, errorAfter, curChar)), errorCode(reason) {}

const char* TokenMgrError::what() const noexcept {
    return std::runtime_error::what();
}

std::string TokenMgrError::addEscapes(const std::string& str) {
    std::ostringstream retval;
    for (char ch : str) {
        switch (ch) {
        case '\b':
            retval << "\\b";
            break;
        case '\t':
            retval << "\\t";
            break;
        case '\n':
            retval << "\\n";
            break;
        case '\f':
            retval << "\\f";
            break;
        case '\r':
            retval << "\\r";
            break;
        case '\"':
            retval << "\\\"";
            break;
        case '\'':
            retval << "\\\'";
            break;
        case '\\':
            retval << "\\\\";
            break;
        default:
            if (ch < 0x20 || ch > 0x7e) {
                retval << "\\u" << std::setw(4) << std::setfill('0') << std::hex << static_cast<int>(ch);
            } else {
                retval << ch;
            }
        }
    }
    return retval.str();
}

std::string TokenMgrError::LexicalError(bool EOFSeen, int lexState, int errorLine, int errorColumn, const std::string& errorAfter, char curChar) {
    std::ostringstream oss;
    oss << "Lexical error at line " << errorLine << ", column " << errorColumn << ".  Encountered: "
        << (EOFSeen ? "<EOF> " : ("\"" + addEscapes(std::string(1, curChar)) + "\"") + " (" + std::to_string(static_cast<int>(curChar)) + "), ")
        << "after : \"" << addEscapes(errorAfter) << "\"";
    return oss.str();
}
