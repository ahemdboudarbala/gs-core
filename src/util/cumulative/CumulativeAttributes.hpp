#ifndef CUMULATIVE_ATTRIBUTES_HPP
#define CUMULATIVE_ATTRIBUTES_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include "CumulativeSpells.hpp"

class CumulativeAttributes {
public:
    CumulativeAttributes(double date);

    void* get(const std::string& key) const;
    void* getAny(const std::string& key) const;

    std::vector<std::string> getAttributes() const;
    std::vector<CumulativeSpells::Spell> getAttributeSpells(const std::string& key) const;

    int getAttributesCount() const;

    void set(const std::string& key, void* value);
    void remove(const std::string& key);
    void remove();
    void updateDate(double date);

    std::string toString() const;

private:
    bool nullAttributesAreErrors;
    std::unordered_map<std::string, CumulativeSpells> data;
    double date;
};

#endif // CUMULATIVE_ATTRIBUTES_HPP
