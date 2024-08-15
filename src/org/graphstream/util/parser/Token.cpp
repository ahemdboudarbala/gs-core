#include "Token.hpp"

// Constructors
Token::Token() : kind(0), beginLine(0), beginColumn(0), endLine(0), endColumn(0), next(nullptr), specialToken(nullptr) {}

Token::Token(int kind) : kind(kind), beginLine(0), beginColumn(0), endLine(0), endColumn(0), next(nullptr), specialToken(nullptr) {}

Token::Token(int kind, const std::string& image) : kind(kind), image(image), beginLine(0), beginColumn(0), endLine(0), endColumn(0), next(nullptr), specialToken(nullptr) {}

// Accessors
std::shared_ptr<Token> Token::getNext() const {
    return next;
}

void Token::setNext(std::shared_ptr<Token> nextToken) {
    next = nextToken;
}

std::shared_ptr<Token> Token::getSpecialToken() const {
    return specialToken;
}

void Token::setSpecialToken(std::shared_ptr<Token> specialToken) {
    this->specialToken = specialToken;
}

std::string Token::toString() const {
    return image;
}

// Method to return an optional attribute value (can be overridden in subclasses)
std::shared_ptr<void> Token::getValue() const {
    return nullptr;
}

// Static factory method
std::shared_ptr<Token> Token::newToken(int ofKind, const std::string& image) {
    return std::make_shared<Token>(ofKind, image);
}

std::shared_ptr<Token> Token::newToken(int ofKind) {
    return newToken(ofKind, "");
}
