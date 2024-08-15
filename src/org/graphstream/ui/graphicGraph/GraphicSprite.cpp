#include "GraphicSprite.hpp"
#include "GraphicGraph.hpp"
#include <stdexcept>
#include <cmath>

// Constructor
GraphicSprite::GraphicSprite(const std::string& id, std::shared_ptr<GraphicGraph> graph)
    : GraphicElement(id, graph), position(Style::Units::GU, 0, 0, 0) {

    // Get the position of a random node.
    if (graph->getNodeCount() > 0) {
        auto randomNode = graph->getNodes().begin();
        node = *randomNode;

        position.setValue(0, node->getX());
        position.setValue(1, node->getY());
        position.setValue(2, node->getZ());
    }

    std::string myPrefix = "ui.sprite." + id;
    if (!graph->hasAttribute(myPrefix)) {
        graph->setAttribute(myPrefix, position);
    }
}

// Access methods
std::shared_ptr<GraphicNode> GraphicSprite::getNodeAttachment() const {
    return node;
}

std::shared_ptr<GraphicEdge> GraphicSprite::getEdgeAttachment() const {
    return edge;
}

std::shared_ptr<GraphicElement> GraphicSprite::getAttachment() const {
    if (node) return node;
    if (edge) return edge;
    return nullptr;
}

bool GraphicSprite::isAttached() const {
    return (edge != nullptr || node != nullptr);
}

bool GraphicSprite::isAttachedToNode() const {
    return node != nullptr;
}

bool GraphicSprite::isAttachedToEdge() const {
    return edge != nullptr;
}

Selector::Type GraphicSprite::getSelectorType() const {
    return Selector::Type::SPRITE;
}

double GraphicSprite::getX() const {
    return position.get(0);
}

double GraphicSprite::getY() const {
    return position.get(1);
}

double GraphicSprite::getZ() const {
    return position.get(2);
}

Style::Units GraphicSprite::getUnits() const {
    return position.getUnits();
}

// Command methods
void GraphicSprite::move(double x, double y, double z) {
    if (isAttachedToNode()) {
        auto n = getNodeAttachment();
        x -= n->getX();
        y -= n->getY();
        z -= n->getZ();
        setPosition(x, y, z, Style::Units::GU);
    } else if (isAttachedToEdge()) {
        auto e = getEdgeAttachment();
        double len = e->getTo()->getX() - e->getFrom()->getX();
        double diff = x - e->getFrom()->getX();
        x = diff / len;
        setPosition(x);
    } else {
        setPosition(x, y, z, Style::Units::GU);
    }
}

void GraphicSprite::attachToNode(std::shared_ptr<GraphicNode> node) {
    this->edge = nullptr;
    this->node = node;

    std::string prefix = "ui.sprite." + getId();
    if (!this->node->hasAttribute(prefix)) {
        this->node->setAttribute(prefix);
    }

    graph->setGraphChanged(true);
}

void GraphicSprite::attachToEdge(std::shared_ptr<GraphicEdge> edge) {
    this->node = nullptr;
    this->edge = edge;

    std::string prefix = "ui.sprite." + getId();
    if (!this->edge->hasAttribute(prefix)) {
        this->edge->setAttribute(prefix);
    }

    graph->setGraphChanged(true);
}

void GraphicSprite::detach() {
    std::string prefix = "ui.sprite." + getId();

    if (node) {
        node->removeAttribute(prefix);
    } else if (edge) {
        edge->removeAttribute(prefix);
    }

    node = nullptr;
    edge = nullptr;
    graph->setGraphChanged(true);
}

void GraphicSprite::setPosition(double value) {
    setPosition(value, 0, 0, getUnits());
}

void GraphicSprite::setPosition(double x, double y, double z, Style::Units units) {
    if (edge) {
        if (x < 0) x = 0;
        else if (x > 1) x = 1;
    }

    bool changed = false;
    if (getX() != x) {
        changed = true;
        position.setValue(0, x);
    }
    if (getY() != y) {
        changed = true;
        position.setValue(1, y);
    }
    if (getZ() != z) {
        changed = true;
        position.setValue(2, z);
    }
    if (getUnits() != units) {
        changed = true;
        position.setUnits(units);
    }

    if (changed) {
        graph->setGraphChanged(true);
        graph->setBoundsChanged(true);

        std::string prefix = "ui.sprite." + getId();
        graph->setAttribute(prefix, position);
    }
}

void GraphicSprite::setPosition(const Values& values) {
    double x = 0, y = 0, z = 0;
    if (values.getValueCount() > 0) x = values.get(0);
    if (values.getValueCount() > 1) y = values.get(1);
    if (values.getValueCount() > 2) z = values.get(2);

    setPosition(x, y, z, values.getUnits());
}

double GraphicSprite::checkAngle(double angle) {
    if (angle > 2 * M_PI)
        angle = fmod(angle, 2 * M_PI);
    else if (angle < 0)
        angle = 2 * M_PI - fmod(-angle, 2 * M_PI);

    return angle;
}

void GraphicSprite::attributeChanged(const AttributeChangeEvent& event, const std::string& attribute,
                                     const std::any& oldValue, const std::any& newValue) {
    GraphicElement::attributeChanged(event, attribute, oldValue, newValue);

    std::string completeAttr = "ui.sprite." + getId() + "." + attribute;
    graph->getListeners().sendAttributeChangedEvent(graph->getId(), ElementType::GRAPH, completeAttr, event, oldValue, newValue);
}

void GraphicSprite::removed() {
    // Implement removal logic if necessary
}
