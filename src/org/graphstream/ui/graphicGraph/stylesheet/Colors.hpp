#ifndef COLORS_HPP
#define COLORS_HPP

#include <vector>
#include "Color.hpp"

class Colors : public std::vector<Color> {
public:
    // Constructors
    Colors() = default;
    Colors(const Colors& others);

    // Utility
    Colors& operator=(const Colors& others);
};

#endif // COLORS_HPP
