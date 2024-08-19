#ifndef SELECTOR_HPP
#define SELECTOR_HPP

#include <string>
#include <stdexcept>
#include <sstream>

class Selector {
public:
    // Types of elements.
    enum class Type {
        ANY, GRAPH, NODE, EDGE, SPRITE
    };

    // Constructors
    Selector(Type type);
    Selector(Type type, const std::string& identifier, const std::string& clazz);
    Selector(const std::string& type);
    Selector(const Selector& other);

    // Setters
    void setId(const std::string& id);
    void setClass(const std::string& clazz);
    void setPseudoClass(const std::string& pseudoClass);

    // Getters
    Type getType() const;
    std::string getId() const;
    std::string getClazz() const;
    std::string getPseudoClass() const;

    // Utility
    std::string toString() const;

private:
    Type type;
    std::string id;
    std::string clazz;
    std::string pseudoClass;
};

#endif // SELECTOR_HPP
