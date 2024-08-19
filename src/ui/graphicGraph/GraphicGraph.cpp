#include "GraphicGraph.hpp"

// Constructor
GraphicGraph::GraphicGraph(const std::string& id)
    : AbstractElement(id), 
      styleSheet(std::make_shared<StyleSheet>()),
      styleGroups(std::make_shared<StyleGroupSet>(styleSheet)),
      step(0), 
      graphChanged(false), 
      boundsChanged(true), 
      hi(std::make_shared<Point3>()), 
      lo(std::make_shared<Point3>()),
      listeners(std::make_shared<GraphListeners>(this)),
      feedbackXYZEnabled(false) {

    styleGroups->addListener(shared_from_this());
    styleGroups->addElement(shared_from_this());
    style = styleGroups->getStyleFor(shared_from_this());
}

// Accessors
bool GraphicGraph::graphChangedFlag() const {
    return graphChanged;
}

void GraphicGraph::resetGraphChangedFlag() {
    graphChanged = false;
}

std::shared_ptr<StyleSheet> GraphicGraph::getStyleSheet() const {
    return styleSheet;
}

std::shared_ptr<StyleGroup> GraphicGraph::getStyle() const {
    return style;
}

std::shared_ptr<StyleGroupSet> GraphicGraph::getStyleGroups() const {
    return styleGroups;
}

double GraphicGraph::getStep() const {
    return step;
}

std::shared_ptr<Point3> GraphicGraph::getMaxPos() const {
    return hi;
}

std::shared_ptr<Point3> GraphicGraph::getMinPos() const {
    return lo;
}

bool GraphicGraph::feedbackXYZ() const {
    return feedbackXYZEnabled;
}

// Commands
void GraphicGraph::feedbackXYZ(bool on) {
    feedbackXYZEnabled = on;
}

void GraphicGraph::computeBounds() {
    if (boundsChanged) {
        lo->x = lo->y = lo->z = std::numeric_limits<double>::max();
        hi->x = hi->y = hi->z = -std::numeric_limits<double>::max();

        for (auto& node : styleGroups->nodes()) {
            auto graphicNode = std::dynamic_pointer_cast<GraphicNode>(node);
            if (!graphicNode->isHidden() && graphicNode->isPositioned()) {
                lo->x = std::min(lo->x, graphicNode->getX());
                lo->y = std::min(lo->y, graphicNode->getY());
                lo->z = std::min(lo->z, graphicNode->getZ());
                hi->x = std::max(hi->x, graphicNode->getX());
                hi->y = std::max(hi->y, graphicNode->getY());
                hi->z = std::max(hi->z, graphicNode->getZ());
            }
        }

        for (auto& sprite : styleGroups->sprites()) {
            if (!sprite->isAttached() && sprite->getUnits() == StyleConstants::Units::GU) {
                lo->x = std::min(lo->x, sprite->getX());
                lo->y = std::min(lo->y, sprite->getY());
                lo->z = std::min(lo->z, sprite->getZ());
                hi->x = std::max(hi->x, sprite->getX());
                hi->y = std::max(hi->y, sprite->getY());
                hi->z = std::max(hi->z, sprite->getZ());
            }
        }

        boundsChanged = false;
    }
}

void GraphicGraph::moveNode(const std::string& id, double x, double y, double z) {
    auto node = std::dynamic_pointer_cast<GraphicNode>(styleGroups->getNode(id));
    if (node) {
        node->move(x, y, z);
        graphChanged = true;
    }
}

std::shared_ptr<GraphicNode> GraphicGraph::getNode(const std::string& id) const {
    return std::dynamic_pointer_cast<GraphicNode>(styleGroups->getNode(id));
}

std::shared_ptr<GraphicEdge> GraphicGraph::getEdge(const std::string& id) const {
    return std::dynamic_pointer_cast<GraphicEdge>(styleGroups->getEdge(id));
}

std::shared_ptr<GraphicSprite> GraphicGraph::getSprite(const std::string& id) const {
    return std::dynamic_pointer_cast<GraphicSprite>(styleGroups->getSprite(id));
}

// Sink management
void GraphicGraph::addSink(std::shared_ptr<Sink> listener) {
    listeners->addSink(listener);
}

void GraphicGraph::removeSink(std::shared_ptr<Sink> listener) {
    listeners->removeSink(listener);
}

void GraphicGraph::addAttributeSink(std::shared_ptr<AttributeSink> listener) {
    listeners->addAttributeSink(listener);
}

void GraphicGraph::removeAttributeSink(std::shared_ptr<AttributeSink> listener) {
    listeners->removeAttributeSink(listener);
}

void GraphicGraph::addElementSink(std::shared_ptr<ElementSink> listener) {
    listeners->addElementSink(listener);
}

void GraphicGraph::removeElementSink(std::shared_ptr<ElementSink> listener) {
    listeners->removeElementSink(listener);
}

void GraphicGraph::clearAttributeSinks() {
    listeners->clearAttributeSinks();
}

void GraphicGraph::clearElementSinks() {
    listeners->clearElementSinks();
}

void GraphicGraph::clearSinks() {
    listeners->clearSinks();
}

// Sprite management
std::shared_ptr<GraphicSprite> GraphicGraph::addSprite(const std::string& id) {
    auto sprite = addSprite_(id);
    styleGroups->addElement(sprite);
    return sprite;
}

void GraphicGraph::removeSprite(const std::string& id) {
    auto sprite = removeSprite_(id);
    if (sprite) {
        styleGroups->removeElement(sprite);
    }
}

// Utility methods
std::shared_ptr<GraphicSprite> GraphicGraph::addSprite_(const std::string& id) {
    auto sprite = std::make_shared<GraphicSprite>(id, shared_from_this());
    graphChanged = true;
    return sprite;
}

std::shared_ptr<GraphicSprite> GraphicGraph::removeSprite_(const std::string& id) {
    auto sprite = std::dynamic_pointer_cast<GraphicSprite>(styleGroups->getSprite(id));
    if (sprite) {
        sprite->detach();
        graphChanged = true;
    }
    return sprite;
}

void GraphicGraph::elementStyleChanged(std::shared_ptr<Element> element, 
                                       std::shared_ptr<StyleGroup> oldStyle, 
                                       std::shared_ptr<StyleGroup> newStyle) {
    auto ge = std::dynamic_pointer_cast<GraphicElement>(element);
    if (ge) {
        ge->setStyle(newStyle);
        graphChanged = true;
    }
}

void GraphicGraph::styleChanged(std::shared_ptr<StyleGroup> style) {
    // Implement any necessary updates when a style changes.
}

// Graph interface
int GraphicGraph::getEdgeCount() const {
    return styleGroups->getEdgeCount();
}

int GraphicGraph::getNodeCount() const {
    return styleGroups->getNodeCount();
}

int GraphicGraph::getSpriteCount() const {
    return styleGroups->getSpriteCount();
}

void GraphicGraph::replay() {
    for (auto& key : attributeKeys()) {
        listeners->sendGraphAttributeAdded(id, key, getAttribute(key));
    }

    for (auto& node : styleGroups->nodes()) {
        listeners->sendNodeAdded(id, node->getId());
        for (auto& key : node->attributeKeys()) {
            listeners->sendNodeAttributeAdded(id, node->getId(), key, node->getAttribute(key));
        }
    }

    for (auto& edge : styleGroups->edges()) {
        listeners->sendEdgeAdded(id, edge->getId(), edge->getSourceNode()->getId(), edge->getTargetNode()->getId(), edge->isDirected());
        for (auto& key : edge->attributeKeys()) {
            listeners->sendEdgeAttributeAdded(id, edge->getId(), key, edge->getAttribute(key));
        }
    }
}

// Override from AbstractElement
void GraphicGraph::attributeChanged(const AttributeChangeEvent& event, const std::string& attribute, 
                                    const std::any& oldValue, const std::any& newValue) {
    // Handle attribute changes
    if (attribute == "ui.repaint") {
        graphChanged = true;
    } else if (attribute == "ui.stylesheet" || attribute == "stylesheet") {
        if (event == AttributeChangeEvent::ADD || event == AttributeChangeEvent::CHANGE) {
            try {
                styleSheet->load(std::any_cast<std::string>(newValue));
                graphChanged = true;
            } catch (const std::exception& e) {
                std::cerr << "Error parsing style sheet: " << e.what() << std::endl;
            }
        } else {
            styleSheet->clear();
            graphChanged = true;
        }
    } else if (attribute.rfind("ui.sprite.", 0) == 0) {
        spriteAttribute(event, nullptr, attribute, newValue);
        graphChanged = true;
    }

    listeners->sendAttributeChangedEvent(getId(), ElementType::GRAPH, attribute, event, oldValue, newValue);
}

// Additional sprite management
void GraphicGraph::spriteAttribute(const AttributeChangeEvent& event, std::shared_ptr<Element> element, 
                                   const std::string& attribute, const std::any& value) {
    std::string spriteId = attribute.substr(10); // Remove "ui.sprite."
    std::string attr = "";

    size_t pos = spriteId.find('.');
    if (pos != std::string::npos) {
        attr = spriteId.substr(pos + 1);
        spriteId = spriteId.substr(0, pos);
    }

    if (attr.empty()) {
        addOrChangeSprite(event, element, spriteId, value);
    } else {
        auto sprite = styleGroups->getSprite(spriteId);
        if (!sprite) {
            addOrChangeSprite(AttributeChangeEvent::ADD, element, spriteId, nullptr);
            sprite = styleGroups->getSprite(spriteId);
        }
        if (sprite) {
            if (event == AttributeChangeEvent::ADD) {
                sprite->setAttribute(attr, value);
            } else if (event == AttributeChangeEvent::REMOVE) {
                sprite->removeAttribute(attr);
            }
        }
    }
}

void GraphicGraph::addOrChangeSprite(const AttributeChangeEvent& event, std::shared_ptr<Element> element, 
                                     const std::string& spriteId, const std::any& value) {
    if (event == AttributeChangeEvent::ADD || event == AttributeChangeEvent::CHANGE) {
        auto sprite = styleGroups->getSprite(spriteId);
        if (!sprite) {
            sprite = addSprite_(spriteId);
        }

        if (element) {
            auto graphicNode = std::dynamic_pointer_cast<GraphicNode>(element);
            auto graphicEdge = std::dynamic_pointer_cast<GraphicEdge>(element);
            if (graphicNode) {
                sprite->attachToNode(graphicNode);
            } else if (graphicEdge) {
                sprite->attachToEdge(graphicEdge);
            }
        }

        if (value.has_value()) {
            positionSprite(sprite, value);
        }
    } else if (event == AttributeChangeEvent::REMOVE) {
        if (!element) {
            removeSprite_(spriteId);
        } else {
            auto sprite = styleGroups->getSprite(spriteId);
            if (sprite) {
                sprite->detach();
            }
        }
    }
}

void GraphicGraph::positionSprite(std::shared_ptr<GraphicSprite> sprite, const std::any& value) {
    try {
        if (value.type() == typeid(std::array<double, 4>)) {
            auto values = std::any_cast<std::array<double, 4>>(value);
            sprite->setPosition(values[0], values[1], values[2], StyleConstants::Units::GU);
        } else if (value.type() == typeid(double)) {
            sprite->setPosition(std::any_cast<double>(value));
        } else if (value.type() == typeid(Value)) {
            sprite->setPosition(std::any_cast<Value>(value).value);
        } else if (value.type() == typeid(Values)) {
            sprite->setPosition(std::any_cast<Values>(value));
        } else {
            throw std::runtime_error("Invalid type for sprite position");
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to position sprite: " << e.what() << std::endl;
    }
}
