#include "Style.hpp"
#include <sstream>
#include <algorithm>

Style::Style(std::shared_ptr<Rule> parent)
    : parent(parent) {}

std::shared_ptr<Rule> Style::getParent() const {
    return parent;
}

std::shared_ptr<void> Style::getValue(const std::string& property, const std::vector<std::string>& events) const {
    if (!events.empty()) {
        for (auto it = events.rbegin(); it != events.rend(); ++it) {
            auto value = getValueForEvent(property, *it);
            if (value) return value;
        }
    }

    auto it = values.find(property);
    if (it != values.end()) {
        return it->second;
    }

    if (parent) {
        return parent->getStyle()->getValue(property, events);
    }

    return nullptr;
}

std::shared_ptr<void> Style::getValueForEvent(const std::string& property, const std::string& event) const {
    if (alternates) {
        auto rule = alternates.find(event);
        if (rule != alternates.end()) {
            auto it = rule->second->getStyle()->values.find(property);
            if (it != rule->second->getStyle()->values.end()) {
                return it->second;
            }
        }
    } else if (parent) {
        return parent->getStyle()->getValueForEvent(property, event);
    }

    return nullptr;
}

bool Style::hasValue(const std::string& field, const std::vector<std::string>& events) const {
    if (!events.empty()) {
        for (const auto& event : events) {
            auto rule = alternates.find(event);
            if (rule != alternates.end() && rule->second->getStyle()->hasValue(field)) {
                return true;
            }
        }
    }

    return values.find(field) != values.end();
}

// Individual style properties implementations

StyleConstants::FillMode Style::getFillMode() const {
    return *std::static_pointer_cast<StyleConstants::FillMode>(getValue("fill-mode"));
}

std::shared_ptr<Colors> Style::getFillColors() const {
    return std::static_pointer_cast<Colors>(getValue("fill-color"));
}

int Style::getFillColorCount() const {
    auto colors = getFillColors();
    return colors ? colors->size() : 0;
}

std::shared_ptr<Color> Style::getFillColor(int i) const {
    auto colors = getFillColors();
    return colors ? colors->get(i) : nullptr;
}

// More getters follow similar patterns...

void Style::setDefaults() {
    auto fillColor = std::make_shared<Colors>();
    fillColor->add(std::make_shared<Color>(Color::BLACK));

    auto strokeColor = std::make_shared<Colors>();
    strokeColor->add(std::make_shared<Color>(Color::BLACK));

    auto shadowColor = std::make_shared<Colors>();
    shadowColor->add(std::make_shared<Color>(Color::GRAY));

    auto textColor = std::make_shared<Colors>();
    textColor->add(std::make_shared<Color>(Color::BLACK));

    auto canvasColor = std::make_shared<Colors>();
    canvasColor->add(std::make_shared<Color>(Color::WHITE));

    auto textBgColor = std::make_shared<Colors>();
    textBgColor->add(std::make_shared<Color>(Color::WHITE));

    setValue("z-index", std::make_shared<int>(0));

    setValue("fill-mode", std::make_shared<StyleConstants::FillMode>(StyleConstants::FillMode::PLAIN));
    setValue("fill-color", fillColor);

    setValue("stroke-mode", std::make_shared<StyleConstants::StrokeMode>(StyleConstants::StrokeMode::NONE));
    setValue("stroke-color", strokeColor);
    setValue("stroke-width", std::make_shared<Value>(StyleConstants::Units::PX, 1));

    setValue("shadow-mode", std::make_shared<StyleConstants::ShadowMode>(StyleConstants::ShadowMode::NONE));
    setValue("shadow-color", shadowColor);
    setValue("shadow-width", std::make_shared<Value>(StyleConstants::Units::PX, 3));
    setValue("shadow-offset", std::make_shared<Values>(StyleConstants::Units::PX, std::vector<float>{3, 3}));

    setValue("padding", std::make_shared<Values>(StyleConstants::Units::PX, std::vector<float>{0, 0, 0}));

    setValue("text-mode", std::make_shared<StyleConstants::TextMode>(StyleConstants::TextMode::NORMAL));
    setValue("text-visibility-mode", std::make_shared<StyleConstants::TextVisibilityMode>(StyleConstants::TextVisibilityMode::NORMAL));
    setValue("text-color", textColor);
    setValue("text-style", std::make_shared<StyleConstants::TextStyle>(StyleConstants::TextStyle::NORMAL));
    setValue("text-font", std::make_shared<std::string>("default"));
    setValue("text-size", std::make_shared<Value>(StyleConstants::Units::PX, 10));
    setValue("text-alignment", std::make_shared<StyleConstants::TextAlignment>(StyleConstants::TextAlignment::CENTER));
    setValue("text-background-mode", std::make_shared<StyleConstants::TextBackgroundMode>(StyleConstants::TextBackgroundMode::NONE));
    setValue("text-background-color", textBgColor);
    setValue("text-offset", std::make_shared<Values>(StyleConstants::Units::PX, std::vector<float>{0, 0}));
    setValue("text-padding", std::make_shared<Values>(StyleConstants::Units::PX, std::vector<float>{0, 0}));

    setValue("icon-mode", std::make_shared<StyleConstants::IconMode>(StyleConstants::IconMode::NONE));

    setValue("visibility-mode", std::make_shared<StyleConstants::VisibilityMode>(StyleConstants::VisibilityMode::NORMAL));

    setValue("size-mode", std::make_shared<StyleConstants::SizeMode>(StyleConstants::SizeMode::NORMAL));
    setValue("size", std::make_shared<Values>(StyleConstants::Units::PX, std::vector<float>{10, 10, 10}));

    setValue("shape", std::make_shared<StyleConstants::Shape>(StyleConstants::Shape::CIRCLE));

    setValue("sprite-orientation", std::make_shared<StyleConstants::SpriteOrientation>(StyleConstants::SpriteOrientation::NONE));

    setValue("arrow-shape", std::make_shared<StyleConstants::ArrowShape>(StyleConstants::ArrowShape::ARROW));
    setValue("arrow-size", std::make_shared<Values>(StyleConstants::Units::PX, std::vector<float>{8, 4}));
    setValue("canvas-color", canvasColor);
}

void Style::augment(std::shared_ptr<Style> other) {
    if (other == this) return;

    augmentField("z-index", other);
    augmentField("fill-mode", other);
    augmentField("fill-color", other);
    augmentField("fill-image", other);

    augmentField("stroke-mode", other);
    augmentField("stroke-color", other);
    augmentField("stroke-width", other);

    augmentField("shadow-mode", other);
    augmentField("shadow-color", other);
    augmentField("shadow-width", other);
    augmentField("shadow-offset", other);

    augmentField("padding", other);

    augmentField("text-mode", other);
    augmentField("text-visibility-mode", other);
    augmentField("text-visibility", other);
    augmentField("text-color", other);
    augmentField("text-style", other);
    augmentField("text-font", other);
    augmentField("text-size", other);
    augmentField("text-alignment", other);
    augmentField("text-background-mode", other);
    augmentField("text-background-color", other);
    augmentField("text-offset", other);
    augmentField("text-padding", other);

    augmentField("icon-mode", other);
    augmentField("icon", other);

    augmentField("visibility-mode", other);
    augmentField("visibility", other);

    augmentField("size-mode", other);
    augmentField("size", other);

    augmentField("shape", other);
    augmentField("shape-points", other);
    augmentField("jcomponent", other);

    augmentField("sprite-orientation", other);

    augmentField("arrow-shape", other);
    augmentField("arrow-size", other);
    augmentField("arrow-image", other);

    augmentField("canvas-color", other);
}

void Style::augmentField(const std::string& field, std::shared_ptr<Style> other) {
    auto value = other->values[field];
    if (value) {
        setValue(field, value);
    }
}

void Style::reparent(std::shared_ptr<Rule> parent) {
    this->parent = parent;
}

void Style::addAlternateStyle(const std::string& event, std::shared_ptr<Rule> alternateStyle) {
    if (!alternates) {
        alternates = std::make_shared<std::map<std::string, std::shared_ptr<Rule>>>();
    }
    alternates[event] = alternateStyle;
}

void Style::setValue(const std::string& field, std::shared_ptr<void> value) {
    values[field] = value;
}

std::string Style::toString(int level) const {
    std::ostringstream builder;
    std::string prefix(level > 0 ? std::string(level * 4, ' ') : "");

    builder << prefix << "Style:\n";

    if (parent) {
        auto p = parent;
        while (p) {
            builder << " -> " << p->getSelector().toString();
            p = p->getStyle()->getParent();
        }
    }

    builder << "\n";

    for (const auto& [key, value] : values) {
        if (auto vec = std::dynamic_pointer_cast<std::vector<std::shared_ptr<void>>>(value)) {
            if (!vec->empty()) {
                builder << prefix << "    " << key << ": ";
                for (const auto& v : *vec) {
                    builder << v->toString() << " ";
                }
                builder << "\n";
            } else {
                builder << prefix << "    " << key << ": <empty>\n";
            }
        } else {
            builder << prefix << "    " << key << ": " << (value ? value->toString() : "<null>") << "\n";
        }
    }

    if (alternates && !alternates->empty()) {
        for (const auto& [event, rule] : *alternates) {
            builder << rule->toString(level - 1);
        }
    }

    return builder.str();
}
