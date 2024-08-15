#ifndef COLOR_HPP
#define COLOR_HPP

#include <string>

class Color {
public:
    // Constructors
    Color(int r, int g, int b, int a = 255);

    // Getters
    int getRed() const;
    int getGreen() const;
    int getBlue() const;
    int getAlpha() const;

    // Setters
    void setRed(int r);
    void setGreen(int g);
    void setBlue(int b);
    void setAlpha(int a);

    // Static predefined colors
    static const Color white;
    static const Color lightGray;
    static const Color gray;
    static const Color darkGray;
    static const Color black;
    static const Color red;
    static const Color pink;
    static const Color orange;
    static const Color yellow;
    static const Color green;
    static const Color magenta;
    static const Color cyan;
    static const Color blue;

    // Static method to decode a color from a hex string
    static Color decode(const std::string& nm);

    // Overload the equality operator
    bool operator==(const Color& other) const;

private:
    int r, g, b, a;
};

#endif // COLOR_HPP
