#include "AbstractEdge.hpp"
#include <cassert>
#include <sstream>

// Constructor implementation
AbstractEdge::AbstractEdge(const std::string& id, AbstractNode* source, AbstractNode* target, bool directed)
    : AbstractElement(id), source(source), target(target), directed(directed), graph(nullptr)
{
    assert(source != nullptr && target != nullptr && "An edge cannot have null endpoints");
    this->graph = dynamic_cast<AbstractGraph*>(source->getGraph());
}

// Attribute change event handling
void AbstractEdge::attributeChanged(const std::string& event, const std::string& attribute, const std::string& oldValue, const std::string& newValue) {
    if (graph) {
        graph->listeners.sendAttributeChangedEvent(id, ElementType::EDGE, attribute, event, oldValue, newValue);
    }
}

// String representation of the edge
std::string AbstractEdge::toString() const {
    std::ostringstream oss;
    oss << getId() << "[" << source->toString() << (directed ? ">" : "-") << target->toString() << "]";
    return oss.str();
}

// Get the first node (source)
Node* AbstractEdge::getNode0() const {
    return source;
}

// Get the second node (target)
Node* AbstractEdge::getNode1() const {
    return target;
}

// Get the opposite node relative to the given node
Node* AbstractEdge::getOpposite(Node* node) const {
    if (node == source) return target;
    if (node == target) return source;
    return nullptr;
}

// Get the source node
Node* AbstractEdge::getSourceNode() const {
    return source;
}

// Get the target node
Node* AbstractEdge::getTargetNode() const {
    return target;
}

// Check if the edge is directed
bool AbstractEdge::isDirected() const {
    return directed;
}

// Check if the edge is a loop
bool AbstractEdge::isLoop() const {
    return source == target;
}
