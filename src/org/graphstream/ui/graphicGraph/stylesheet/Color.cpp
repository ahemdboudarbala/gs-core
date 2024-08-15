#include "Color.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>

// Predefined colors
const Color Color::white(255, 255, 255);
const Color Color::lightGray(192, 192, 192);
const Color Color::gray(128, 128, 128);
const Color Color::darkGray(64, 64, 64);
const Color Color::black(0, 0, 0);
const Color Color::red(255, 0, 0);
const Color Color::pink(255, 175, 175);
const Color Color::orange(255, 200, 0);
const Color Color::yellow(255, 255, 0);
const Color Color::green(0, 255, 0);
const Color Color::magenta(255, 0, 255);
const Color Color::cyan(0, 255, 255);
const Color Color::blue(0, 0, 255);

// Constructor
Color::Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}

// Getters
int Color::getRed() const { return r; }
int Color::getGreen() const { return g; }
int Color::getBlue() const { return b; }
int Color::getAlpha() const { return a; }

// Setters
void Color::setRed(int r) { this->r = r; }
void Color::setGreen(int g) { this->g = g; }
void Color::setBlue(int b) { this->b = b; }
void Color::setAlpha(int a) { this->a = a; }

// Decode method
Color Color::decode(const std::string& nm) {
    if (nm[0] != '#' || (nm.size() != 7 && nm.size() != 9)) {
        throw std::invalid_argument("Invalid color format");
    }

    int r = std::stoi(nm.substr(1, 2), nullptr, 16);
    int g = std::stoi(nm.substr(3, 2), nullptr, 16);
    int b = std::stoi(nm.substr(5, 2), nullptr, 16);
    int a = (nm.size() == 9) ? std::stoi(nm.substr(7, 2), nullptr, 16) : 255;

    return Color(r, g, b, a);
}

// Equality operator
bool Color::operator==(const Color& other) const {
    return r == other.r && g == other.g && b == other.b && a == other.a;
}
