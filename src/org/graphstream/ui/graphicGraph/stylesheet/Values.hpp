#ifndef VALUES_HPP
#define VALUES_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iterator>

class Units {
public:
    enum class Type { GU, PX, PERCENTS, UNKNOWN };
};

class Values {
public:
    // Constructors
    Values(Units::Type units, const std::vector<double>& values = {});
    Values(const Values& other);
    Values(double value, Units::Type units);

    // Methods
    int size() const;
    int getValueCount() const;
    double get(int i) const;
    Units::Type getUnits() const;
    void copy(const Values& other);
    void addValues(const std::vector<double>& newValues);
    void insertValue(int i, double value);
    void setValue(int i, double value);
    void removeValue(int i);
    void setUnits(Units::Type newUnits);

    bool operator==(const Values& other) const;
    std::string toString() const;

    // Iterator
    std::vector<double>::iterator begin();
    std::vector<double>::iterator end();
    std::vector<double>::const_iterator begin() const;
    std::vector<double>::const_iterator end() const;

private:
    std::vector<double> values;
    Units::Type units;
};

#endif // VALUES_HPP
