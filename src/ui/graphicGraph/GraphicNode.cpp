#include "GraphicNode.hpp"
#include "GraphicGraph.hpp"
#include <stdexcept>
#include <iostream>

// Constructor
GraphicNode::GraphicNode(std::shared_ptr<GraphicGraph> graph, const std::string& id, 
                         const std::map<std::string, std::any>& attributes)
    : GraphicElement(id, graph), x(0), y(0), z(0), positioned(false) {
    if (!attributes.empty()) {
        setAttributes(attributes);
    }
}

// Access methods
Selector::Type GraphicNode::getSelectorType() const {
    return Selector::Type::NODE;
}

double GraphicNode::getX() const {
    return x;
}

double GraphicNode::getY() const {
    return y;
}

double GraphicNode::getZ() const {
    return z;
}

std::shared_ptr<Point3> GraphicNode::getPosition() const {
    return std::make_shared<Point3>(x, y, z);
}

void GraphicNode::moveFromEvent(double newX, double newY, double newZ) {
    x = newX;
    y = newY;
    z = newZ;

    if (!positioned) {
        positioned = true;
    }

    graph->setGraphChanged(true);
    graph->setBoundsChanged(true);
}

void GraphicNode::move(double newX, double newY, double newZ) {
    moveFromEvent(newX, newY, newZ);

    if (graph->isFeedbackXYZEnabled()) {
        setAttribute("xyz", newX, newY, newZ);
    }
}

void GraphicNode::attributeChanged(const AttributeChangeEvent& event, const std::string& attribute, 
                                   const std::any& oldValue, const std::any& newValue) {
    GraphicElement::attributeChanged(event, attribute, oldValue, newValue);
    
    if (attribute.length() > 2 && attribute.substr(0, 2) == "ui" && attribute.find("ui.sprite.") == 0) {
        graph->spriteAttribute(event, shared_from_this(), attribute, newValue);
    } else if (event == AttributeChangeEvent::ADD || event == AttributeChangeEvent::CHANGE) {
        char c = attribute[0];
        if (attribute.length() == 1) {
            switch (c) {
                case 'x':
                    moveFromEvent(numberAttribute(newValue), y, z);
                    break;
                case 'y':
                    moveFromEvent(x, numberAttribute(newValue), z);
                    break;
                case 'z':
                    moveFromEvent(x, y, numberAttribute(newValue));
                    break;
                default:
                    break;
            }
        } else if (c == 'x' && attribute.length() > 1 && attribute[1] == 'y') {
            double pos[] = {x, y, z};
            moveFromEvent(pos[0], pos[1], pos[2]);
        }
    }

    graph->getListeners().sendAttributeChangedEvent(getId(), ElementType::NODE, attribute, event, oldValue, newValue);
}

double GraphicNode::numberAttribute(const std::any& value) const {
    if (value.type() == typeid(double)) {
        return std::any_cast<double>(value);
    } else if (value.type() == typeid(std::string)) {
        try {
            return std::stod(std::any_cast<std::string>(value));
        } catch (const std::invalid_argument&) {
        }
    }
    return 0;
}

void GraphicNode::removed() {
    // Implementation for removal logic
}

// Node interface methods
int GraphicNode::getDegree() const {
    auto it = graph->getConnectivity().find(shared_from_this());
    if (it != graph->getConnectivity().end()) {
        return it->second.size();
    }
    return 0;
}

std::shared_ptr<GraphicEdge> GraphicNode::getEdge(int i) const {
    auto it = graph->getConnectivity().find(shared_from_this());
    if (it != graph->getConnectivity().end() && i >= 0 && i < it->second.size()) {
        return it->second[i];
    }
    return nullptr;
}

std::shared_ptr<GraphicEdge> GraphicNode::getEdgeBetween(const std::string& id) const {
    if (hasEdgeToward(id)) {
        return getEdgeToward(id);
    } else {
        return getEdgeFrom(id);
    }
}

std::shared_ptr<GraphicEdge> GraphicNode::getEdgeFrom(const std::string& id) const {
    // Implementation needed based on the graph structure
    return nullptr;
}

std::shared_ptr<GraphicEdge> GraphicNode::getEdgeToward(const std::string& id) const {
    auto it = graph->getConnectivity().find(shared_from_this());
    if (it != graph->getConnectivity().end()) {
        for (const auto& edge : it->second) {
            if (edge->getOpposite(shared_from_this())->getId() == id) {
                return edge;
            }
        }
    }
    return nullptr;
}

std::shared_ptr<Graph> GraphicNode::getGraph() const {
    return graph;
}

int GraphicNode::getInDegree() const {
    return getDegree();
}

int GraphicNode::getOutDegree() const {
    return getDegree();
}

bool GraphicNode::hasEdgeBetween(const std::string& id) const {
    return hasEdgeToward(id) || hasEdgeFrom(id);
}

bool GraphicNode::hasEdgeFrom(const std::string& id) const {
    return false;
}

bool GraphicNode::hasEdgeToward(const std::string& id) const {
    return false;
}

// Stub methods for the new methods
std::shared_ptr<GraphicEdge> GraphicNode::getEdgeBetween(std::shared_ptr<GraphicNode> node) {
    return nullptr;
}

std::shared_ptr<GraphicEdge> GraphicNode::getEdgeBetween(int index) {
    return nullptr;
}

std::shared_ptr<GraphicEdge> GraphicNode::getEdgeFrom(std::shared_ptr<GraphicNode> node) {
    return nullptr;
}

std::shared_ptr<GraphicEdge> GraphicNode::getEdgeFrom(int index) {
    return nullptr;
}

std::shared_ptr<GraphicEdge> GraphicNode::getEdgeToward(std::shared_ptr<GraphicNode> node) {
    return nullptr;
}

std::shared_ptr<GraphicEdge> GraphicNode::getEdgeToward(int index) {
    return nullptr;
}

std::shared_ptr<GraphicEdge> GraphicNode::getEnteringEdge(int i) {
    return nullptr;
}

std::shared_ptr<GraphicEdge> GraphicNode::getLeavingEdge(int i) {
    return nullptr;
}
