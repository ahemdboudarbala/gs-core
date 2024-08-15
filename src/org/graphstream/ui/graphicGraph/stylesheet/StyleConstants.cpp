#include "StyleConstants.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

// Static methods implementation

void StyleConstants::initializeColorMap() {
    sharpColor1 = std::regex("#(\\p{XDigit}{2})(\\p{XDigit}{2})(\\p{XDigit}{2})(\\p{XDigit}{2})?");
    sharpColor2 = std::regex("#(\\p{XDigit})(\\p{XDigit})(\\p{XDigit})(\\p{XDigit})?");
    cssColor = std::regex("rgb\\s*\\(\\s*([0-9]+)\\s*,\\s*([0-9]+)\\s*,\\s*([0-9]+)\\s*\\)");
    cssColorA = std::regex("rgba\\s*\\(\\s*([0-9]+)\\s*,\\s*([0-9]+)\\s*,\\s*([0-9]+)\\s*,\\s*([0-9]+)\\s*\\)");
    awtColor = std::regex("java.awt.Color\\[r=([0-9]+),g=([0-9]+),b=([0-9]+)\\]");
    hexaColor = std::regex("0[xX](\\p{XDigit}{2})(\\p{XDigit}{2})(\\p{XDigit}{2})(\\p{XDigit}{2})?");
    numberUnit = std::regex("\\s*(\\d+(?:\\.\\d+)?)\\s*(gu|px|%)\\s*");
    number = std::regex("\\s*(\\d+(\\.\\d+)?)\\s*");

    loadColors();
}

void StyleConstants::loadColors() {
    std::ifstream file("rgb.properties");
    if (!file.is_open()) {
        throw std::runtime_error("Could not open rgb.properties file");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            colorMap[key] = std::make_shared<Color>(Color::decode(value));
        }
    }
}

std::shared_ptr<Color> StyleConstants::convertColor(const std::string& value) {
    if (value.empty()) return nullptr;

    if (value[0] == '#') {
        std::smatch match;
        if (std::regex_match(value, match, sharpColor1)) {
            if (value.length() == 7) {
                return Color::decode(value);
            } else if (value.length() == 9) {
                int r = std::stoi(match[1], nullptr, 16);
                int g = std::stoi(match[2], nullptr, 16);
                int b = std::stoi(match[3], nullptr, 16);
                int a = std::stoi(match[4], nullptr, 16);
                return std::make_shared<Color>(r, g, b, a);
            }
        } else if (std::regex_match(value, match, sharpColor2)) {
            if (value.length() >= 4) {
                int r = std::stoi(match[1], nullptr, 16) * 16;
                int g = std::stoi(match[2], nullptr, 16) * 16;
                int b = std::stoi(match[3], nullptr, 16) * 16;
                int a = (value.length() == 5) ? std::stoi(match[4], nullptr, 16) * 16 : 255;
                return std::make_shared<Color>(r, g, b, a);
            }
        }
    } else if (value.rfind("rgb", 0) == 0) {
        std::smatch match;
        if (std::regex_match(value, match, cssColorA)) {
            int r = std::stoi(match[1]);
            int g = std::stoi(match[2]);
            int b = std::stoi(match[3]);
            int a = std::stoi(match[4]);
            return std::make_shared<Color>(r, g, b, a);
        } else if (std::regex_match(value, match, cssColor)) {
            int r = std::stoi(match[1]);
            int g = std::stoi(match[2]);
            int b = std::stoi(match[3]);
            return std::make_shared<Color>(r, g, b);
        }
    } else if (value.rfind("0x", 0) == 0 || value.rfind("0X", 0) == 0) {
        std::smatch match;
        if (std::regex_match(value, match, hexaColor)) {
            if (value.length() == 8) {
                return Color::decode(value);
            } else if (value.length() == 10) {
                int r = std::stoi(match[1], nullptr, 16);
                int g = std::stoi(match[2], nullptr, 16);
                int b = std::stoi(match[3], nullptr, 16);
                int a = std::stoi(match[4], nullptr, 16);
                return std::make_shared<Color>(r, g, b, a);
            }
        }
    } else if (value.rfind("java.awt.Color[", 0) == 0) {
        std::smatch match;
        if (std::regex_match(value, match, awtColor)) {
            int r = std::stoi(match[1]);
            int g = std::stoi(match[2]);
            int b = std::stoi(match[3]);
            return std::make_shared<Color>(r, g, b);
        }
    }

    auto it = colorMap.find(value);
    if (it != colorMap.end()) {
        return it->second;
    }

    return nullptr;
}

std::string StyleConstants::convertLabel(const std::string& value) {
    if (value.empty()) return "";
    if (value.length() > 128) {
        return value.substr(0, 128) + "...";
    }
    return value;
}

float StyleConstants::convertWidth(const std::string& value) {
    try {
        return std::stof(value);
    } catch (std::exception& e) {
        return -1.0f;
    }
}

Value StyleConstants::convertValue(const std::string& value) {
    std::smatch match;
    if (std::regex_match(value, match, numberUnit)) {
        return Value(convertUnit(match[2]), std::stof(match[1]));
    } else if (std::regex_match(value, match, number)) {
        return Value(Units::PX, std::stof(match[1]));
    }
    throw std::runtime_error("Invalid value: " + value);
}

StyleConstants::Units StyleConstants::convertUnit(const std::string& unit) {
    if (unit == "gu") return Units::GU;
    if (unit == "px") return Units::PX;
    if (unit == "%") return Units::PERCENTS;
    return Units::PX;
}
