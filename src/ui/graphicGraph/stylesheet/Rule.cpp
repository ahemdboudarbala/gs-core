#include "Rule.hpp"

// Constructors
Rule::Rule() : style(std::make_shared<Style>()) {}

Rule::Rule(const Selector& selector)
    : selector(selector), style(std::make_shared<Style>()) {}

Rule::Rule(const Selector& selector, std::shared_ptr<Rule> parent)
    : selector(selector), style(std::make_shared<Style>(parent->getStyle())) {}

// Accessors
std::shared_ptr<Style> Rule::getStyle() const {
    return style;
}

std::unordered_set<std::string> Rule::getGroups() const {
    return groups;
}

// Mutators
void Rule::setStyle(const std::shared_ptr<Style>& style) {
    this->style = style;
}

void Rule::addGroup(const std::string& groupId) {
    groups.insert(groupId);
}

void Rule::removeGroup(const std::string& groupId) {
    groups.erase(groupId);
}

// Match function
bool Rule::matchId(const std::string& identifier) const {
    std::string ident = selector.getId();
    return !ident.empty() && ident == identifier;
}

// Utility
std::string Rule::toString() const {
    return toString(-1);
}

std::string Rule::toString(int level) const {
    std::ostringstream oss;
    std::string prefix = "";

    if (level > 0) {
        for (int i = 0; i < level; ++i)
            prefix += "    ";
    }

    oss << prefix << selector.toString() << style->toString(level + 1);
    return oss.str();
}
