#include "AbstractNode.hpp"
#include "BreadthFirstIterator.hpp"
#include "DepthFirstIterator.hpp"

// Constructor
AbstractNode::AbstractNode(std::shared_ptr<AbstractGraph> graph, const std::string& id)
    : AbstractElement(id), graph(graph) {}

// Attribute change handling
void AbstractNode::attributeChanged(AttributeChangeEvent event, const std::string& attribute,
                                    std::shared_ptr<void> oldValue, std::shared_ptr<void> newValue) {
    graph->listeners.sendAttributeChangedEvent(id, SourceBase::ElementType::NODE, attribute, event, oldValue, newValue);
}

// Get the graph to which this node belongs
std::shared_ptr<Graph> AbstractNode::getGraph() const {
    return graph;
}

// Edge management
std::shared_ptr<Edge> AbstractNode::getEdgeToward(int index) {
    return getEdgeToward(graph->getNode(index));
}

std::shared_ptr<Edge> AbstractNode::getEdgeToward(const std::string& id) {
    return getEdgeToward(graph->getNode(id));
}

std::shared_ptr<Edge> AbstractNode::getEdgeFrom(int index) {
    return getEdgeFrom(graph->getNode(index));
}

std::shared_ptr<Edge> AbstractNode::getEdgeFrom(const std::string& id) {
    return getEdgeFrom(graph->getNode(id));
}

std::shared_ptr<Edge> AbstractNode::getEdgeBetween(int index) {
    return getEdgeBetween(graph->getNode(index));
}

std::shared_ptr<Edge> AbstractNode::getEdgeBetween(const std::string& id) {
    return getEdgeBetween(graph->getNode(id));
}

// Iterators
std::unique_ptr<std::iterator<std::input_iterator_tag, Node>> AbstractNode::getBreadthFirstIterator() {
    return std::make_unique<BreadthFirstIterator>(shared_from_this());
}

std::unique_ptr<std::iterator<std::input_iterator_tag, Node>> AbstractNode::getBreadthFirstIterator(bool directed) {
    return std::make_unique<BreadthFirstIterator>(shared_from_this(), directed);
}

std::unique_ptr<std::iterator<std::input_iterator_tag, Node>> AbstractNode::getDepthFirstIterator() {
    return std::make_unique<DepthFirstIterator>(shared_from_this());
}

std::unique_ptr<std::iterator<std::input_iterator_tag, Node>> AbstractNode::getDepthFirstIterator(bool directed) {
    return std::make_unique<DepthFirstIterator>(shared_from_this(), directed);
}

// Utility methods
bool AbstractNode::isEnteringEdge(std::shared_ptr<Edge> e) const {
    return e->getTargetNode() == shared_from_this() || (!e->isDirected() && e->getSourceNode() == shared_from_this());
}

bool AbstractNode::isLeavingEdge(std::shared_ptr<Edge> e) const {
    return e->getSourceNode() == shared_from_this() || (!e->isDirected() && e->getTargetNode() == shared_from_this());
}

bool AbstractNode::isIncidentEdge(std::shared_ptr<Edge> e) const {
    return e->getSourceNode() == shared_from_this() || e->getTargetNode() == shared_from_this();
}
