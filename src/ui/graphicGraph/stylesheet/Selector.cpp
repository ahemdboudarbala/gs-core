#include "Selector.hpp"

// Constructors
Selector::Selector(Type type)
    : type(type) {}

Selector::Selector(Type type, const std::string& identifier, const std::string& clazz)
    : type(type), id(identifier), clazz(clazz) {}

Selector::Selector(const std::string& typeStr) {
    if (typeStr == "node")
        type = Type::NODE;
    else if (typeStr == "edge")
        type = Type::EDGE;
    else if (typeStr == "graph")
        type = Type::GRAPH;
    else if (typeStr == "sprite")
        type = Type::SPRITE;
    else
        throw std::runtime_error("Invalid matcher type '" + typeStr + "'");
}

Selector::Selector(const Selector& other)
    : type(other.type), id(other.id), clazz(other.clazz), pseudoClass(other.pseudoClass) {}

// Setters
void Selector::setId(const std::string& id) {
    this->id = id;
}

void Selector::setClass(const std::string& clazz) {
    this->clazz = clazz;
}

void Selector::setPseudoClass(const std::string& pseudoClass) {
    this->pseudoClass = pseudoClass;
}

// Getters
Selector::Type Selector::getType() const {
    return type;
}

std::string Selector::getId() const {
    return id;
}

std::string Selector::getClazz() const {
    return clazz;
}

std::string Selector::getPseudoClass() const {
    return pseudoClass;
}

// Utility
std::string Selector::toString() const {
    std::ostringstream oss;
    oss << typeToString(type)
        << (id.empty() ? "" : "#" + id)
        << (clazz.empty() ? "" : "." + clazz)
        << (pseudoClass.empty() ? "" : ":" + pseudoClass);
    return oss.str();
}

// Helper function to convert Type to string
std::string Selector::typeToString(Type type) const {
    switch (type) {
        case Type::ANY: return "ANY";
        case Type::GRAPH: return "GRAPH";
        case Type::NODE: return "NODE";
        case Type::EDGE: return "EDGE";
        case Type::SPRITE: return "SPRITE";
        default: throw std::runtime_error("Invalid type");
    }
}
