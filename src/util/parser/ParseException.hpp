#ifndef PARSEEXCEPTION_HPP
#define PARSEEXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <vector>

class Token; // Forward declaration of the Token class

class ParseException : public std::exception {
public:
    Token* currentToken;
    std::vector<std::vector<int>> expectedTokenSequences;
    std::vector<std::string> tokenImage;

    ParseException();
    ParseException(const std::string& message);
    ParseException(Token* currentTokenVal, const std::vector<std::vector<int>>& expectedTokenSequencesVal, const std::vector<std::string>& tokenImageVal);

    const char* what() const noexcept override;

private:
    static std::string initialize(Token* currentToken, const std::vector<std::vector<int>>& expectedTokenSequences, const std::vector<std::string>& tokenImage);
    static std::string addEscapes(const std::string& str);
};

#endif // PARSEEXCEPTION_HPP
