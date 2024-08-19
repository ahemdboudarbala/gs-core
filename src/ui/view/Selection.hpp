#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <cmath>

class Selection {
public:
    double x1, y1, x2, y2;

    Selection() : x1(0), y1(0), x2(0), y2(0) {}
    Selection(double x1, double y1, double x2, double y2)
        : x1(x1), y1(y1), x2(x2), y2(y2) {}

    bool operator==(const Selection& other) const {
        return doubleEquals(x1, other.x1) &&
               doubleEquals(y1, other.y1) &&
               doubleEquals(x2, other.x2) &&
               doubleEquals(y2, other.y2);
    }

    bool operator!=(const Selection& other) const {
        return !(*this == other);
    }

    std::size_t hashCode() const {
        std::size_t h1 = std::hash<double>{}(x1);
        std::size_t h2 = std::hash<double>{}(y1);
        std::size_t h3 = std::hash<double>{}(x2);
        std::size_t h4 = std::hash<double>{}(y2);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }

private:
    static bool doubleEquals(double a, double b) {
        return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
    }
};

namespace std {
    template <>
    struct hash<Selection> {
        std::size_t operator()(const Selection& sel) const {
            return sel.hashCode();
        }
    };
}

#endif // SELECTION_HPP
