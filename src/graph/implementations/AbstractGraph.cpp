#include "AbstractGraph.hpp"
#include "graph/ElementNotFoundException.hpp"
#include "graph/IdAlreadyInUseException.hpp"
#include "graph/EdgeRejectedException.hpp"
#include "util/MissingDisplayException.hpp"
#include <algorithm>
#include <sstream>

// Constructors
AbstractGraph::AbstractGraph(const std::string& id)
    : AbstractGraph(id, true, false) {}

AbstractGraph::AbstractGraph(const std::string& id, bool strictChecking, bool autoCreate)
    : AbstractElement(id), listeners(this), strictChecking(strictChecking), autoCreate(autoCreate), step(0), replayId(0) {}

// Attribute change handling
void AbstractGraph::attributeChanged(AttributeChangeEvent event, const std::string& attribute,
                                     std::shared_ptr<void> oldValue, std::shared_ptr<void> newValue) {
    listeners.sendAttributeChangedEvent(id, SourceBase::ElementType::GRAPH, attribute, event, oldValue, newValue);
}

// Node and Edge Factories
std::shared_ptr<NodeFactory<Node>> AbstractGraph::nodeFactory() {
    return nodeFactory;
}

std::shared_ptr<EdgeFactory<Edge>> AbstractGraph::edgeFactory() {
    return edgeFactory;
}

void AbstractGraph::setNodeFactory(std::shared_ptr<NodeFactory<Node>> nf) {
    nodeFactory = std::dynamic_pointer_cast<NodeFactory<AbstractNode>>(nf);
}

void AbstractGraph::setEdgeFactory(std::shared_ptr<EdgeFactory<Edge>> ef) {
    edgeFactory = std::dynamic_pointer_cast<EdgeFactory<AbstractEdge>>(ef);
}

// Graph properties
bool AbstractGraph::isStrict() const {
    return strictChecking;
}

void AbstractGraph::setStrict(bool on) {
    strictChecking = on;
}

bool AbstractGraph::isAutoCreationEnabled() const {
    return autoCreate;
}

void AbstractGraph::setAutoCreate(bool on) {
    autoCreate = on;
}

double AbstractGraph::getStep() const {
    return step;
}

void AbstractGraph::stepBegins(double time) {
    listeners.sendStepBegins(time);
    this->step = time;
}

// Graph operations
void AbstractGraph::clear() {
    listeners.sendGraphCleared();

    auto nodesToClear = nodes();
    for (auto& node : nodesToClear) {
        std::dynamic_pointer_cast<AbstractNode>(node)->clearCallback();
    }

    clearCallback();
    clearAttributesWithNoEvent();
}

std::shared_ptr<Node> AbstractGraph::addNode(const std::string& id) {
    auto node = std::dynamic_pointer_cast<AbstractNode>(getNode(id));

    if (node != nullptr) {
        if (strictChecking) {
            throw IdAlreadyInUseException("id \"" + id + "\" already in use. Cannot create a node.");
        }
        return node;
    }

    node = nodeFactory->newInstance(id, shared_from_this());
    addNodeCallback(node);

    listeners.sendNodeAdded(id);

    return node;
}

std::shared_ptr<Edge> AbstractGraph::addEdge(const std::string& id, std::shared_ptr<Node> from, 
                                             std::shared_ptr<Node> to, bool directed) {
    return addEdge(id, std::dynamic_pointer_cast<AbstractNode>(from), from->getId(), 
                   std::dynamic_pointer_cast<AbstractNode>(to), to->getId(), directed);
}

std::shared_ptr<Node> AbstractGraph::removeNode(std::shared_ptr<Node> node) {
    if (node == nullptr) return nullptr;

    removeNode(std::dynamic_pointer_cast<AbstractNode>(node), true);
    return node;
}

std::shared_ptr<Edge> AbstractGraph::removeEdge(std::shared_ptr<Edge> edge) {
    if (edge == nullptr) return nullptr;

    removeEdge(std::dynamic_pointer_cast<AbstractEdge>(edge), true, true, true);
    return edge;
}

std::shared_ptr<Edge> AbstractGraph::removeEdge(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
    auto edge = node1->getEdgeToward(node2);

    if (edge == nullptr) {
        if (strictChecking) {
            std::ostringstream oss;
            oss << "There is no edge from \"" << node1->getId() << "\" to \"" << node2->getId() << "\". Cannot remove it.";
            throw ElementNotFoundException(oss.str());
        }
        return nullptr;
    }

    return removeEdge(edge);
}

// Sinks and sources
std::vector<std::shared_ptr<AttributeSink>> AbstractGraph::attributeSinks() {
    return listeners.attributeSinks();
}

std::vector<std::shared_ptr<ElementSink>> AbstractGraph::elementSinks() {
    return listeners.elementSinks();
}

void AbstractGraph::addAttributeSink(std::shared_ptr<AttributeSink> sink) {
    listeners.addAttributeSink(sink);
}

void AbstractGraph::addElementSink(std::shared_ptr<ElementSink> sink) {
    listeners.addElementSink(sink);
}

void AbstractGraph::addSink(std::shared_ptr<Sink> sink) {
    listeners.addSink(sink);
}

void AbstractGraph::clearAttributeSinks() {
    listeners.clearAttributeSinks();
}

void AbstractGraph::clearElementSinks() {
    listeners.clearElementSinks();
}

void AbstractGraph::clearSinks() {
    listeners.clearSinks();
}

void AbstractGraph::removeAttributeSink(std::shared_ptr<AttributeSink> sink) {
    listeners.removeAttributeSink(sink);
}

void AbstractGraph::removeElementSink(std::shared_ptr<ElementSink> sink) {
    listeners.removeElementSink(sink);
}

void AbstractGraph::removeSink(std::shared_ptr<Sink> sink) {
    listeners.removeSink(sink);
}

// Replayable interface
std::unique_ptr<Replayable::Controller> AbstractGraph::getReplayController() {
    return std::make_unique<GraphReplayController>(this);
}

// Private methods
std::shared_ptr<Edge> AbstractGraph::addEdge(const std::string& edgeId, std::shared_ptr<AbstractNode> src, 
                                             const std::string& srcId, std::shared_ptr<AbstractNode> dst, 
                                             const std::string& dstId, bool directed) {
    auto edge = std::dynamic_pointer_cast<AbstractEdge>(getEdge(edgeId));

    if (edge != nullptr) {
        if (strictChecking) {
            throw IdAlreadyInUseException("id \"" + edgeId + "\" already in use. Cannot create an edge.");
        }
        if ((edge->getSourceNode() == src && edge->getTargetNode() == dst) || 
            (!directed && edge->getTargetNode() == src && edge->getSourceNode() == dst)) {
            return edge;
        }
        return nullptr;
    }

    if (src == nullptr || dst == nullptr) {
        if (strictChecking) {
            std::ostringstream oss;
            oss << "Cannot create edge " << edgeId << "[" << srcId << (directed ? ">" : "-") << dstId << "].";
            oss << " Node '" << (src == nullptr ? srcId : dstId) << "' does not exist.";
            throw ElementNotFoundException(oss.str());
        }
        if (!autoCreate) return nullptr;

        if (src == nullptr) src = std::dynamic_pointer_cast<AbstractNode>(addNode(srcId));
        if (dst == nullptr) dst = std::dynamic_pointer_cast<AbstractNode>(addNode(dstId));
    }

    if (src->getGraph() != shared_from_this() || dst->getGraph() != shared_from_this()) {
        throw ElementNotFoundException("At least one of two nodes does not belong to the graph.");
    }

    edge = edgeFactory->newInstance(edgeId, src, dst, directed);

    if (!src->addEdgeCallback(edge)) {
        if (strictChecking) {
            throw EdgeRejectedException("Edge " + edgeId + " was rejected by node " + src->getId());
        }
        return nullptr;
    }

    if (src != dst && !dst->addEdgeCallback(edge)) {
        src->removeEdgeCallback(edge);
        if (strictChecking) {
            throw EdgeRejectedException("Edge " + edgeId + " was rejected by node " + dst->getId());
        }
        return nullptr;
    }

    addEdgeCallback(edge);
    listeners.sendEdgeAdded(edgeId, srcId, dstId, directed);

    return edge;
}

void AbstractGraph::removeAllEdges(std::shared_ptr<AbstractNode> node) {
    auto toRemove = node->edges();
    for (auto& edge : toRemove) {
        removeEdge(edge);
    }
}

void AbstractGraph::removeNode(std::shared_ptr<AbstractNode> node, bool graphCallback) {
    if (node == nullptr) {
        throw std::runtime_error("node reference is null");
    }
    if (node->getGraph() != shared_from_this()) {
        throw ElementNotFoundException("Node \"" + node->getId() + "\" does not belong to this graph");
    }

    removeAllEdges(node);
    listeners.sendNodeRemoved(node->getId());

    if (graphCallback) {
        removeNodeCallback(node);
    }
}

void AbstractGraph::removeEdge(std::shared_ptr<AbstractEdge> edge, bool graphCallback, 
                               bool sourceCallback, bool targetCallback) {
    if (edge == nullptr) {
        throw std::runtime_error("edge reference is null");
    }

    auto src = std::dynamic_pointer_cast<AbstractNode>(edge->getSourceNode());
    auto dst = std::dynamic_pointer_cast<AbstractNode>(edge->getTargetNode());

    if (src->getGraph() != shared_from_this() || dst->getGraph() != shared_from_this()) {
        throw ElementNotFoundException("Edge \"" + edge->getId() + "\" does not belong to this graph");
    }

    listeners.sendEdgeRemoved(edge->getId());

    if (sourceCallback) src->removeEdgeCallback(edge);
    if (src != dst && targetCallback) dst->removeEdgeCallback(edge);

    if (graphCallback) removeEdgeCallback(edge);
}

// GraphReplayController implementation
AbstractGraph::GraphReplayController::GraphReplayController(AbstractGraph* graph)
    : SourceBase(graph->getId() + "replay") {}

void AbstractGraph::GraphReplayController::replay() {
    replay(getId() + "-replay-" + std::to_string(graph->replayId++));
}

void AbstractGraph::GraphReplayController::replay(const std::string& sourceId) {
    for (const auto& key : graph->attributeKeys()) {
        sendGraphAttributeAdded(sourceId, key, graph->getAttribute(key));
    }

    for (int i = 0; i < graph->getNodeCount(); ++i) {
        auto node = graph->getNode(i);
        auto nodeId = node->getId();
        sendNodeAdded(sourceId, nodeId);

        for (const auto& key : node->attributeKeys()) {
            sendNodeAttributeAdded(sourceId, nodeId, key, node->getAttribute(key));
        }
    }

    for (int i = 0; i < graph->getEdgeCount(); ++i) {
        auto edge = graph->getEdge(i);
        auto edgeId = edge->getId();
        sendEdgeAdded(sourceId, edgeId, edge->getNode0()->getId(), edge->getNode1()->getId(), edge->isDirected());

        for (const auto& key : edge->attributeKeys()) {
            sendEdgeAttributeAdded(sourceId, edgeId, key, edge->getAttribute(key));
        }
    }
}
