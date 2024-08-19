#include "GraphicEdge.hpp"
#include "GraphicGraph.hpp"

GraphicEdge::GraphicEdge(const std::string& id, std::shared_ptr<GraphicNode> from, 
                         std::shared_ptr<GraphicNode> to, bool directed, 
                         const std::unordered_map<std::string, std::any>& attributes)
    : GraphicElement(id, from->getGraph()), from(from), to(to), directed(directed), multi(0) {
    if (!attributes.empty()) {
        setAttributes(attributes);
    }
}

Selector::Type GraphicEdge::getSelectorType() const {
    return Selector::Type::EDGE;
}

std::shared_ptr<GraphicNode> GraphicEdge::otherNode(const std::shared_ptr<GraphicNode>& node) const {
    return (node == from) ? to : from;
}

double GraphicEdge::getX() const {
    return from->getX() + ((to->getX() - from->getX()) / 2);
}

double GraphicEdge::getY() const {
    return from->getY() + ((to->getY() - from->getY()) / 2);
}

double GraphicEdge::getZ() const {
    return from->getZ() + ((to->getZ() - from->getZ()) / 2);
}

const std::vector<double>& GraphicEdge::getControlPoints() const {
    return ctrl;
}

void GraphicEdge::setControlPoints(const std::vector<double>& points) {
    ctrl = points;
}

bool GraphicEdge::isCurve() const {
    return !ctrl.empty();
}

int GraphicEdge::getMultiIndex() const {
    return multi;
}

void GraphicEdge::move(double x, double y, double z) {
    // Edges do not move directly
}

void GraphicEdge::removed() {
    if (group) {
        group->decrement(shared_from_this());
        if (group->getCount() == 1) {
            group.reset();
        }
    }
}

std::shared_ptr<GraphicNode> GraphicEdge::getNode0() const {
    return from;
}

std::shared_ptr<GraphicNode> GraphicEdge::getNode1() const {
    return to;
}

std::shared_ptr<GraphicEdge::EdgeGroup> GraphicEdge::getGroup() const {
    return group;
}

std::shared_ptr<GraphicNode> GraphicEdge::getOpposite(const std::shared_ptr<Node>& node) const {
    return (node == from) ? to : from;
}

std::shared_ptr<GraphicNode> GraphicEdge::getSourceNode() const {
    return from;
}

std::shared_ptr<GraphicNode> GraphicEdge::getTargetNode() const {
    return to;
}

bool GraphicEdge::isDirected() const {
    return directed;
}

bool GraphicEdge::isLoop() const {
    return from == to;
}

void GraphicEdge::setDirected(bool on) {
    directed = on;
}

void GraphicEdge::switchDirection() {
    std::swap(from, to);
}

void GraphicEdge::attributeChanged(AttributeChangeEvent event, const std::string& attribute, 
                                   const std::any& oldValue, const std::any& newValue) {
    GraphicElement::attributeChanged(event, attribute, oldValue, newValue);

    if (attribute.rfind("ui.sprite.", 0) == 0) {
        myGraph->spriteAttribute(event, shared_from_this(), attribute, newValue);
    }

    myGraph->getListeners()->sendAttributeChangedEvent(getId(), ElementType::EDGE, attribute, event, oldValue, newValue);
}

// EdgeGroup Implementation

GraphicEdge::EdgeGroup::EdgeGroup(const std::shared_ptr<GraphicEdge>& first, const std::shared_ptr<GraphicEdge>& second) {
    edges.push_back(first);
    edges.push_back(second);
    first->group = shared_from_this();
    second->group = shared_from_this();
    first->multi = 0;
    second->multi = 1;
}

std::shared_ptr<GraphicEdge> GraphicEdge::EdgeGroup::getEdge(int i) const {
    return edges.at(i);
}

int GraphicEdge::EdgeGroup::getCount() const {
    return edges.size();
}

void GraphicEdge::EdgeGroup::increment(const std::shared_ptr<GraphicEdge>& edge) {
    edge->multi = edges.size();
    edges.push_back(edge);
}

void GraphicEdge::EdgeGroup::decrement(const std::shared_ptr<GraphicEdge>& edge) {
    edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
    for (int i = 0; i < edges.size(); ++i) {
        edges[i]->multi = i;
    }
}
