#ifndef STYLECONSTANTS_HPP
#define STYLECONSTANTS_HPP

#include <string>
#include <map>
#include <regex>
#include <memory>
#include <stdexcept>
#include "Color.hpp"
#include "Value.hpp"

// StyleConstants class declaration
class StyleConstants {
public:
    // Enums for various styling modes and properties
    enum class Units { PX, GU, PERCENTS };
    enum class FillMode { NONE, PLAIN, DYN_PLAIN, GRADIENT_RADIAL, GRADIENT_HORIZONTAL, GRADIENT_VERTICAL, GRADIENT_DIAGONAL1, GRADIENT_DIAGONAL2, IMAGE_TILED, IMAGE_SCALED, IMAGE_SCALED_RATIO_MAX, IMAGE_SCALED_RATIO_MIN };
    enum class StrokeMode { NONE, PLAIN, DASHES, DOTS, DOUBLE };
    enum class ShadowMode { NONE, PLAIN, GRADIENT_RADIAL, GRADIENT_HORIZONTAL, GRADIENT_VERTICAL, GRADIENT_DIAGONAL1, GRADIENT_DIAGONAL2 };
    enum class VisibilityMode { NORMAL, HIDDEN, AT_ZOOM, UNDER_ZOOM, OVER_ZOOM, ZOOM_RANGE, ZOOMS };
    enum class TextMode { NORMAL, TRUNCATED, HIDDEN };
    enum class TextVisibilityMode { NORMAL, HIDDEN, AT_ZOOM, UNDER_ZOOM, OVER_ZOOM, ZOOM_RANGE, ZOOMS };
    enum class TextStyle { NORMAL, ITALIC, BOLD, BOLD_ITALIC };
    enum class IconMode { NONE, AT_LEFT, AT_RIGHT, UNDER, ABOVE };
    enum class SizeMode { NORMAL, FIT, DYN_SIZE };
    enum class TextAlignment { CENTER, LEFT, RIGHT, AT_LEFT, AT_RIGHT, UNDER, ABOVE, JUSTIFY, ALONG };
    enum class TextBackgroundMode { NONE, PLAIN, ROUNDEDBOX };
    enum class ShapeKind { ELLIPSOID, RECTANGULAR, LINEAR, CURVE };
    enum class Shape { CIRCLE, BOX, ROUNDED_BOX, DIAMOND, POLYGON, TRIANGLE, CROSS, FREEPLANE, TEXT_BOX, TEXT_ROUNDED_BOX, TEXT_PARAGRAPH, TEXT_CIRCLE, TEXT_DIAMOND, JCOMPONENT, PIE_CHART, FLOW, ARROW, IMAGES, LINE, ANGLE, CUBIC_CURVE, POLYLINE, POLYLINE_SCALED, SQUARELINE, LSQUARELINE, HSQUARELINE, VSQUARELINE, BLOB };
    enum class SpriteOrientation { NONE, FROM, NODE0, TO, NODE1, PROJECTION };
    enum class ArrowShape { NONE, ARROW, CIRCLE, DIAMOND, IMAGE };
    enum class JComponents { BUTTON, TEXT_FIELD, PANEL };

    // Static methods for converting and manipulating styles
    static std::shared_ptr<Color> convertColor(const std::string& value);
    static std::string convertLabel(const std::string& value);
    static float convertWidth(const std::string& value);
    static Value convertValue(const std::string& value);
    static Units convertUnit(const std::string& unit);

private:
    // Static members for color mapping and regex patterns
    static std::map<std::string, std::shared_ptr<Color>> colorMap;
    static std::regex sharpColor1, sharpColor2, cssColor, cssColorA, awtColor, hexaColor, numberUnit, number;

    // Private static method to initialize color mapping
    static void initializeColorMap();
    static void loadColors();
};

// Initialize static members
std::map<std::string, std::shared_ptr<Color>> StyleConstants::colorMap;
std::regex StyleConstants::sharpColor1;
std::regex StyleConstants::sharpColor2;
std::regex StyleConstants::cssColor;
std::regex StyleConstants::cssColorA;
std::regex StyleConstants::awtColor;
std::regex StyleConstants::hexaColor;
std::regex StyleConstants::numberUnit;
std::regex StyleConstants::number;

#endif // STYLECONSTANTS_HPP
