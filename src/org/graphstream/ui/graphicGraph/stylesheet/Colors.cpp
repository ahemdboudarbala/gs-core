#include "Colors.hpp"

// Copy constructor
Colors::Colors(const Colors& others) : std::vector<Color>(others) {
}

// Assignment operator
Colors& Colors::operator=(const Colors& others) {
    if (this != &others) {
        std::vector<Color>::operator=(others);
    }
    return *this;
}
