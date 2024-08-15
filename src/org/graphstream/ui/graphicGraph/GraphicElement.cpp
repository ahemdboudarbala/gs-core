#include "GraphicElement.hpp"
#include <iostream>

std::regex GraphicElement::acceptedAttribute("(ui[.].*)|(layout[.].*)|x|y|z|xy|xyz|label|stylesheet");

GraphicElement::GraphicElement(const std::string& id, std::shared_ptr<GraphicGraph> graph)
    : AbstractElement(id), myGraph(graph), style(nullptr) {}

std::shared_ptr<GraphicGraph> GraphicElement::getGraph() const {
    return myGraph;
}

std::shared_ptr<StyleGroup> GraphicElement::getStyle() const {
    return style;
}

std::string GraphicElement::getLabel() const {
    return label;
}

void* GraphicElement::getComponent() const {
    return component;
}

void GraphicElement::setComponent(void* comp) {
    component = comp;
}

void GraphicElement::attributeChanged(AttributeChangeEvent event, const std::string& attribute,
                                      const std::any& oldValue, const std::any& newValue) {
    if (event == AttributeChangeEvent::ADD || event == AttributeChangeEvent::CHANGE) {
        if (attribute.find("ui.") == 0) {
            if (attribute == "ui.class") {
                myGraph->getStyleGroups()->checkElementStyleGroup(shared_from_this());
                myGraph->graphChanged = true;
            } else if (attribute == "ui.label") {
                label = std::any_cast<std::string>(newValue);
                myGraph->graphChanged = true;
            } else if (attribute == "ui.style") {
                if (newValue.type() == typeid(std::string)) {
                    try {
                        myGraph->getStyleSheet()->parseStyleFromString(
                            Selector(getSelectorType(), getId(), std::nullopt),
                            std::any_cast<std::string>(newValue));
                    } catch (const std::exception& e) {
                        std::cerr << "Error parsing style for " << getSelectorType() << " '"
                                  << getId() << "': " << e.what() << std::endl;
                    }
                    myGraph->graphChanged = true;
                } else {
                    std::cerr << "Unknown value for style [" << std::any_cast<std::string>(newValue) << "]." << std::endl;
                }
            } else if (attribute == "ui.hide") {
                hidden = true;
                myGraph->graphChanged = true;
            }
        }
    } else {
        // Handle REMOVE case similarly...
    }
}

void GraphicElement::setAttribute(const std::string& attribute, const std::any& value) {
    if (std::regex_match(attribute, acceptedAttribute)) {
        AbstractElement::setAttribute(attribute, value);
    }
}
