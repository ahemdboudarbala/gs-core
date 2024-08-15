#include "Values.hpp"

// Constructors
Values::Values(Units::Type units, const std::vector<double>& values)
    : units(units), values(values) {}

Values::Values(const Values& other)
    : units(other.units), values(other.values) {}

Values::Values(double value, Units::Type units)
    : units(units) {
    values.push_back(value);
}

// Methods
int Values::size() const {
    return values.size();
}

int Values::getValueCount() const {
    return values.size();
}

double Values::get(int i) const {
    if (i < 0) return values.front();
    else if (i >= values.size()) return values.back();
    else return values[i];
}

Units::Type Values::getUnits() const {
    return units;
}

void Values::copy(const Values& other) {
    units = other.units;
    values = other.values;
}

void Values::addValues(const std::vector<double>& newValues) {
    values.insert(values.end(), newValues.begin(), newValues.end());
}

void Values::insertValue(int i, double value) {
    values.insert(values.begin() + i, value);
}

void Values::setValue(int i, double value) {
    if (i >= 0 && i < values.size()) {
        values[i] = value;
    }
}

void Values::removeValue(int i) {
    if (i >= 0 && i < values.size()) {
        values.erase(values.begin() + i);
    }
}

void Values::setUnits(Units::Type newUnits) {
    units = newUnits;
}

bool Values::operator==(const Values& other) const {
    return (units == other.units) && (values == other.values);
}

std::string Values::toString() const {
    std::ostringstream oss;
    oss << '(';
    for (const auto& value : values) {
        oss << ' ' << value;
    }
    oss << " )";

    switch (units) {
        case Units::Type::GU: oss << "gu"; break;
        case Units::Type::PX: oss << "px"; break;
        case Units::Type::PERCENTS: oss << "%"; break;
        default: oss << "wtf (what's the fuck?)"; break;
    }

    return oss.str();
}

// Iterator
std::vector<double>::iterator Values::begin() {
    return values.begin();
}

std::vector<double>::iterator Values::end() {
    return values.end();
}

std::vector<double>::const_iterator Values::begin() const {
    return values.begin();
}

std::vector<double>::const_iterator Values::end() const {
    return values.end();
}
