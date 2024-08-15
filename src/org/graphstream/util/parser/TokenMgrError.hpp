#ifndef TOKENMGRError_HPP
#define TOKENMGRError_HPP

#include <stdexcept>
#include <string>

class TokenMgrError : public std::runtime_error {
public:
    // Ordinals for various reasons why an Error of this type can be thrown
    static const int LEXICAL_ERROR = 0;
    static const int STATIC_LEXER_ERROR = 1;
    static const int INVALID_LEXICAL_STATE = 2;
    static const int LOOP_DETECTED = 3;

    // Constructors
    TokenMgrError();
    TokenMgrError(const std::string& message, int reason);
    TokenMgrError(bool EOFSeen, int lexState, int errorLine, int errorColumn, const std::string& errorAfter, char curChar, int reason);

    // Accessors
    const char* what() const noexcept override;

protected:
    static std::string addEscapes(const std::string& str);
    static std::string LexicalError(bool EOFSeen, int lexState, int errorLine, int errorColumn, const std::string& errorAfter, char curChar);

private:
    int errorCode;
};

#endif // TOKENMGRError_HPP
