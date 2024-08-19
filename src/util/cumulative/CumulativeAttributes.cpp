#include "CumulativeAttributes.hpp"

CumulativeAttributes::CumulativeAttributes(double date)
    : date(date), nullAttributesAreErrors(false) {}

void* CumulativeAttributes::get(const std::string& key) const {
    auto it = data.find(key);
    if (it != data.end()) {
        const CumulativeSpells::Spell* s = it->second.getCurrentSpell();
        return s ? s->getAttachedData() : nullptr;
    }
    return nullptr;
}

void* CumulativeAttributes::getAny(const std::string& key) const {
    auto it = data.find(key);
    if (it != data.end()) {
        const CumulativeSpells::Spell* s = it->second.getSpell(0);
        return s ? s->getAttachedData() : nullptr;
    }
    return nullptr;
}

std::vector<std::string> CumulativeAttributes::getAttributes() const {
    std::vector<std::string> keys;
    for (const auto& pair : data) {
        keys.push_back(pair.first);
    }
    return keys;
}

std::vector<CumulativeSpells::Spell> CumulativeAttributes::getAttributeSpells(const std::string& key) const {
    auto it = data.find(key);
    if (it != data.end()) {
        return it->second.getSpells();  // Assuming you add a getSpells() method to return all spells.
    }
    return std::vector<CumulativeSpells::Spell>();
}

int CumulativeAttributes::getAttributesCount() const {
    return data.size();
}

void CumulativeAttributes::set(const std::string& key, void* value) {
    auto& spells = data[key];
    CumulativeSpells::Spell* s = spells.closeSpell();
    if (s) {
        s->setEndOpen(true);
    }
    s = spells.startSpell(date);
    s->setAttachedData(value);
}

void CumulativeAttributes::remove(const std::string& key) {
    auto it = data.find(key);
    if (it != data.end()) {
        it->second.closeSpell();
    }
}

void CumulativeAttributes::remove() {
    for (auto& pair : data) {
        pair.second.closeSpell();
    }
}

void CumulativeAttributes::updateDate(double date) {
    this->date = date;
    for (auto& pair : data) {
        pair.second.updateCurrentSpell(date);
    }
}

std::string CumulativeAttributes::toString() const {
    std::ostringstream buffer;
    buffer << "(";
    for (const auto& pair : data) {
        buffer << pair.first << ":" << pair.second.toString();
    }
    buffer << ")";
    return buffer.str();
}
