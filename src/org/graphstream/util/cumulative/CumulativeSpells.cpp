#include "CumulativeSpells.hpp"

CumulativeSpells::Spell::Spell(double start, bool startOpen, double end, bool endOpen)
    : start(start), end(end), startOpen(startOpen), endOpen(endOpen), closed(false), data(nullptr) {}

CumulativeSpells::Spell::Spell(double start, double end)
    : Spell(start, false, end, false) {}

CumulativeSpells::Spell::Spell(double start)
    : Spell(start, false, start, false) {}

double CumulativeSpells::Spell::getStartDate() const {
    return start;
}

double CumulativeSpells::Spell::getEndDate() const {
    return end;
}

bool CumulativeSpells::Spell::isStartOpen() const {
    return startOpen;
}

bool CumulativeSpells::Spell::isEndOpen() const {
    return endOpen;
}

bool CumulativeSpells::Spell::isStarted() const {
    return !std::isnan(start);
}

bool CumulativeSpells::Spell::isEnded() const {
    return closed;
}

void CumulativeSpells::Spell::setStartOpen(bool open) {
    startOpen = open;
}

void CumulativeSpells::Spell::setEndOpen(bool open) {
    endOpen = open;
}

void* CumulativeSpells::Spell::getAttachedData() const {
    return data;
}

void CumulativeSpells::Spell::setAttachedData(void* data) {
    this->data = data;
}

std::string CumulativeSpells::Spell::toString() const {
    std::ostringstream str;
    if (isStarted()) {
        str << (isStartOpen() ? "]" : "[") << start << "; ";
    } else {
        str << "[...; ";
    }
    if (isEnded()) {
        str << end << (isEndOpen() ? "[" : "]");
    } else {
        str << "...]";
    }
    return str.str();
}

CumulativeSpells::CumulativeSpells() : currentDate(std::numeric_limits<double>::quiet_NaN()) {}

CumulativeSpells::Spell* CumulativeSpells::startSpell(double date) {
    spells.emplace_back(date);
    return &spells.back();
}

void CumulativeSpells::updateCurrentSpell(double date) {
    if (!spells.empty() && !std::isnan(currentDate)) {
        Spell& s = spells.back();
        if (!s.isEnded()) {
            s.end = currentDate;
        }
    }
    currentDate = date;
}

CumulativeSpells::Spell* CumulativeSpells::closeSpell() {
    if (!spells.empty()) {
        Spell& s = spells.back();
        if (!s.isEnded()) {
            s.closed = true;
            return &s;
        }
    }
    return nullptr;
}

CumulativeSpells::Spell* CumulativeSpells::getCurrentSpell() const {
    if (spells.empty()) {
        return nullptr;
    }
    const Spell& s = spells.back();
    return s.isEnded() ? nullptr : const_cast<Spell*>(&s);
}

CumulativeSpells::Spell* CumulativeSpells::getSpell(int i) const {
    if (i < 0 || i >= spells.size()) {
        return nullptr;
    }
    return const_cast<Spell*>(&spells[i]);
}

int CumulativeSpells::getSpellCount() const {
    return spells.size();
}

CumulativeSpells::Spell* CumulativeSpells::getOrCreateSpell(double date) {
    Spell* s = getCurrentSpell();
    if (s == nullptr) {
        s = startSpell(date);
    }
    return s;
}

bool CumulativeSpells::isEternal() const {
    return spells.size() == 1 && !spells[0].isStarted() && !spells[0].isEnded();
}

std::string CumulativeSpells::toString() const {
    std::ostringstream buffer;
    buffer << "{";
    for (size_t i = 0; i < spells.size(); ++i) {
        if (i > 0) {
            buffer << ", ";
        }
        buffer << spells[i].toString();
    }
    buffer << "}";
    return buffer.str();
}
