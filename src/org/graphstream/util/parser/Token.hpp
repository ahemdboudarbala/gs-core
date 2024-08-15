#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <memory>

class Token {
public:
    // Constructors
    Token();
    Token(int kind);
    Token(int kind, const std::string& image);

    // Accessors
    std::shared_ptr<Token> getNext() const;
    void setNext(std::shared_ptr<Token> nextToken);

    std::shared_ptr<Token> getSpecialToken() const;
    void setSpecialToken(std::shared_ptr<Token> specialToken);

    std::string toString() const;

    // Method to return an optional attribute value (can be overridden in subclasses)
    virtual std::shared_ptr<void> getValue() const;

    // Static factory method
    static std::shared_ptr<Token> newToken(int ofKind, const std::string& image);
    static std::shared_ptr<Token> newToken(int ofKind);

    // Public members
    int kind;
    int beginLine;
    int beginColumn;
    int endLine;
    int endColumn;
    std::string image;

private:
    std::shared_ptr<Token> next;
    std::shared_ptr<Token> specialToken;
};

#endif // TOKEN_HPP
