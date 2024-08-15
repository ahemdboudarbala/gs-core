#ifndef RULE_HPP
#define RULE_HPP

#include <string>
#include <unordered_set>
#include <memory>
#include <sstream>
#include "Selector.hpp"
#include "Style.hpp"

class Rule {
public:
    // Constructors
    Rule();
    Rule(const Selector& selector);
    Rule(const Selector& selector, std::shared_ptr<Rule> parent);

    // Accessors
    std::shared_ptr<Style> getStyle() const;
    std::unordered_set<std::string> getGroups() const;

    // Mutators
    void setStyle(const std::shared_ptr<Style>& style);
    void addGroup(const std::string& groupId);
    void removeGroup(const std::string& groupId);

    // Match function
    bool matchId(const std::string& identifier) const;

    // Utility
    std::string toString() const;
    std::string toString(int level) const;

private:
    Selector selector;
    std::shared_ptr<Style> style;
    std::unordered_set<std::string> groups;
};

#endif // RULE_HPP
