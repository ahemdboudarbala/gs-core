#include "ParseException.hpp"
#include "Token.hpp"
#include <sstream>
#include <iomanip>

ParseException::ParseException() : std::exception() {}

ParseException::ParseException(const std::string& message) : std::exception(), currentToken(nullptr) {}

ParseException::ParseException(Token* currentTokenVal, const std::vector<std::vector<int>>& expectedTokenSequencesVal, const std::vector<std::string>& tokenImageVal)
    : currentToken(currentTokenVal), expectedTokenSequences(expectedTokenSequencesVal), tokenImage(tokenImageVal), std::exception(initialize(currentTokenVal, expectedTokenSequencesVal, tokenImageVal).c_str()) {}

const char* ParseException::what() const noexcept {
    return std::exception::what();
}

std::string ParseException::initialize(Token* currentToken, const std::vector<std::vector<int>>& expectedTokenSequences, const std::vector<std::string>& tokenImage) {
    std::string eol = "\n";
    std::ostringstream expected;
    int maxSize = 0;

    for (const auto& seq : expectedTokenSequences) {
        if (maxSize < seq.size()) {
            maxSize = seq.size();
        }
        for (int tok : seq) {
            expected << tokenImage[tok] << ' ';
        }
        if (seq.back() != 0) {
            expected << "...";
        }
        expected << eol << "    ";
    }

    std::ostringstream retval;
    retval << "Encountered \"";
    Token* tok = currentToken->next;
    for (int i = 0; i < maxSize; i++) {
        if (i != 0) retval << " ";
        if (tok->kind == 0) {
            retval << tokenImage[0];
            break;
        }
        retval << " " << tokenImage[tok->kind];
        retval << " \"";
        retval << addEscapes(tok->image);
        retval << " \"";
        tok = tok->next;
    }
    retval << "\" at line " << currentToken->next->beginLine << ", column " << currentToken->next->beginColumn;
    retval << "." << eol;
    if (expectedTokenSequences.size() == 1) {
        retval << "Was expecting:" << eol << "    ";
    } else {
        retval << "Was expecting one of:" << eol << "    ";
    }
    retval << expected.str();
    return retval.str();
}

std::string ParseException::addEscapes(const std::string& str) {
    std::ostringstream retval;
    for (char ch : str) {
        switch (ch) {
            case '\b': retval << "\\b"; break;
            case '\t': retval << "\\t"; break;
            case '\n': retval << "\\n"; break;
            case '\f': retval << "\\f"; break;
            case '\r': retval << "\\r"; break;
            case '\"': retval << "\\\""; break;
            case '\'': retval << "\\\'"; break;
            case '\\': retval << "\\\\"; break;
            default:
                if (ch < 0x20 || ch > 0x7e) {
                    retval << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)ch;
                } else {
                    retval << ch;
                }
        }
    }
    return retval.str();
}
