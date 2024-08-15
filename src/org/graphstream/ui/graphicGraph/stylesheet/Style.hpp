#ifndef STYLE_HPP
#define STYLE_HPP

#include <string>
#include <map>
#include <vector>
#include <memory>
#include "StyleConstants.hpp"
#include "Rule.hpp"
#include "Color.hpp"
#include "Value.hpp"
#include "Values.hpp"
#include "Colors.hpp"

class Style {
public:
    Style(std::shared_ptr<Rule> parent = nullptr);

    std::shared_ptr<Rule> getParent() const;

    std::shared_ptr<void> getValue(const std::string& property, const std::vector<std::string>& events = {}) const;

    bool hasValue(const std::string& field, const std::vector<std::string>& events = {}) const;

    // Individual style properties
    StyleConstants::FillMode getFillMode() const;
    std::shared_ptr<Colors> getFillColors() const;
    int getFillColorCount() const;
    std::shared_ptr<Color> getFillColor(int i) const;

    std::string getFillImage() const;

    StyleConstants::StrokeMode getStrokeMode() const;
    std::shared_ptr<Colors> getStrokeColor() const;
    int getStrokeColorCount() const;
    std::shared_ptr<Color> getStrokeColor(int i) const;

    std::shared_ptr<Value> getStrokeWidth() const;

    StyleConstants::ShadowMode getShadowMode() const;
    std::shared_ptr<Colors> getShadowColors() const;
    int getShadowColorCount() const;
    std::shared_ptr<Color> getShadowColor(int i) const;

    std::shared_ptr<Value> getShadowWidth() const;
    std::shared_ptr<Values> getShadowOffset() const;

    std::shared_ptr<Values> getPadding() const;

    StyleConstants::TextMode getTextMode() const;
    StyleConstants::TextVisibilityMode getTextVisibilityMode() const;
    std::shared_ptr<Values> getTextVisibility() const;

    std::shared_ptr<Colors> getTextColor() const;
    int getTextColorCount() const;
    std::shared_ptr<Color> getTextColor(int i) const;

    StyleConstants::TextStyle getTextStyle() const;
    std::string getTextFont() const;
    std::shared_ptr<Value> getTextSize() const;

    StyleConstants::IconMode getIconMode() const;
    std::string getIcon() const;

    StyleConstants::VisibilityMode getVisibilityMode() const;
    std::shared_ptr<Values> getVisibility() const;

    StyleConstants::SizeMode getSizeMode() const;
    std::shared_ptr<Values> getSize() const;

    std::shared_ptr<Values> getShapePoints() const;

    StyleConstants::TextAlignment getTextAlignment() const;
    StyleConstants::TextBackgroundMode getTextBackgroundMode() const;
    std::shared_ptr<Colors> getTextBackgroundColor() const;
    std::shared_ptr<Color> getTextBackgroundColor(int i) const;

    std::shared_ptr<Values> getTextOffset() const;
    std::shared_ptr<Values> getTextPadding() const;

    StyleConstants::Shape getShape() const;
    StyleConstants::JComponents getJComponent() const;

    StyleConstants::SpriteOrientation getSpriteOrientation() const;

    StyleConstants::ArrowShape getArrowShape() const;
    std::string getArrowImage() const;
    std::shared_ptr<Values> getArrowSize() const;

    std::shared_ptr<Colors> getCanvasColor() const;
    int getCanvasColorCount() const;
    std::shared_ptr<Color> getCanvasColor(int i) const;

    std::shared_ptr<int> getZIndex() const;

    // Commands
    void setDefaults();
    void augment(std::shared_ptr<Style> other);
    void reparent(std::shared_ptr<Rule> parent);
    void addAlternateStyle(const std::string& event, std::shared_ptr<Rule> alternateStyle);

    void setValue(const std::string& field, std::shared_ptr<void> value);

    std::string toString(int level = -1) const;

private:
    std::shared_ptr<void> getValueForEvent(const std::string& property, const std::string& event) const;
    void augmentField(const std::string& field, std::shared_ptr<Style> other);

    std::shared_ptr<Rule> parent;
    std::map<std::string, std::shared_ptr<void>> values;
    std::map<std::string, std::shared_ptr<Rule>> alternates;
};

#endif // STYLE_HPP
