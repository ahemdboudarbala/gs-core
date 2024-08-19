#include "GraphDiff.hpp"
#include "Graph.hpp"
#include "Node.hpp"
#include "Edge.hpp"
#include "Sink.hpp"
#include "AdjacencyListGraph.hpp"
#include <chrono>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cstring>

GraphDiff::GraphDiff() : bridge(nullptr) {}

GraphDiff::GraphDiff(Graph* g1, Graph* g2) : GraphDiff() {
    if (g2->getNodeCount() == 0 && g2->getEdgeCount() == 0 && g2->getAttributeCount() == 0 &&
        (g1->getNodeCount() > 0 || g1->getEdgeCount() > 0)) {
        events.push_back(std::make_shared<GraphCleared>(g1));
    } else {
        attributeDiff(ElementType::GRAPH, g1, g2);

        for (int idx = 0; idx < g1->getEdgeCount(); idx++) {
            Edge* e1 = g1->getEdge(idx);
            Edge* e2 = g2->getEdge(e1->getId());

            if (e2 == nullptr) {
                attributeDiff(ElementType::EDGE, e1, e2);
                events.push_back(std::make_shared<EdgeRemoved>(e1->getId(), e1->getSourceNode()->getId(),
                                                               e1->getTargetNode()->getId(), e1->isDirected()));
            }
        }

        for (int idx = 0; idx < g1->getNodeCount(); idx++) {
            Node* n1 = g1->getNode(idx);
            Node* n2 = g2->getNode(n1->getId());

            if (n2 == nullptr) {
                attributeDiff(ElementType::NODE, n1, n2);
                events.push_back(std::make_shared<NodeRemoved>(n1->getId()));
            }
        }

        for (int idx = 0; idx < g2->getNodeCount(); idx++) {
            Node* n2 = g2->getNode(idx);
            Node* n1 = g1->getNode(n2->getId());

            if (n1 == nullptr)
                events.push_back(std::make_shared<NodeAdded>(n2->getId()));

            attributeDiff(ElementType::NODE, n1, n2);
        }

        for (int idx = 0; idx < g2->getEdgeCount(); idx++) {
            Edge* e2 = g2->getEdge(idx);
            Edge* e1 = g1->getEdge(e2->getId());

            if (e1 == nullptr)
                events.push_back(std::make_shared<EdgeAdded>(e2->getId(), e2->getSourceNode()->getId(),
                                                             e2->getTargetNode()->getId(), e2->isDirected()));

            attributeDiff(ElementType::EDGE, e1, e2);
        }
    }
}

void GraphDiff::start(Graph* g) {
    if (bridge != nullptr)
        end();

    bridge = std::make_unique<Bridge>(g, this);
}

void GraphDiff::end() {
    if (bridge != nullptr) {
        bridge->end();
        bridge = nullptr;
    }
}

void GraphDiff::reset() {
    events.clear();
}

void GraphDiff::apply(Sink* g1) {
    std::string sourceId = "GraphDiff@";
    sourceId += std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    apply(sourceId, g1);
}

void GraphDiff::apply(const std::string& sourceId, Sink* g1) {
    for (size_t i = 0; i < events.size(); ++i)
        events[i]->apply(sourceId, i, g1);
}

void GraphDiff::reverse(Sink* g2) {
    std::string sourceId = "GraphDiff@";
    sourceId += std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    reverse(sourceId, g2);
}

void GraphDiff::reverse(const std::string& sourceId, Sink* g2) {
    for (int i = static_cast<int>(events.size()) - 1; i >= 0; --i)
        events[i]->reverse(sourceId, events.size() + 1 - i, g2);
}

std::string GraphDiff::toString() const {
    std::ostringstream buffer;
    for (const auto& event : events)
        buffer << event->toString() << "\n";
    return buffer.str();
}

void GraphDiff::attributeDiff(ElementType type, Element* e1, Element* e2) {
    if (e1 == nullptr && e2 == nullptr)
        return;
    else if (e1 == nullptr) {
        for (const auto& key : e2->attributeKeys()) {
            events.push_back(std::make_shared<AttributeAdded>(type, e2->getId(), key, e2->getAttribute(key)));
        }
    } else if (e2 == nullptr) {
        for (const auto& key : e1->attributeKeys()) {
            events.push_back(std::make_shared<AttributeRemoved>(type, e1->getId(), key, e1->getAttribute(key)));
        }
    } else {
        for (const auto& key : e2->attributeKeys()) {
            if (e1->hasAttribute(key)) {
                auto o1 = e1->getAttribute(key);
                auto o2 = e2->getAttribute(key);

                if (o1 != o2)
                    events.push_back(std::make_shared<AttributeChanged>(type, e1->getId(), key, o2, o1));
            } else {
                events.push_back(std::make_shared<AttributeAdded>(type, e1->getId(), key, e2->getAttribute(key)));
            }
        }

        for (const auto& key : e1->attributeKeys()) {
            if (!e2->hasAttribute(key))
                events.push_back(std::make_shared<AttributeRemoved>(type, e1->getId(), key, e1->getAttribute(key)));
        }
    }
}

// NodeAdded
GraphDiff::NodeAdded::NodeAdded(const std::string& nodeId) : nodeId(nodeId) {}

void GraphDiff::NodeAdded::apply(const std::string& sourceId, long timeId, Sink* g) {
    g->nodeAdded(sourceId, timeId, nodeId);
}

void GraphDiff::NodeAdded::reverse(const std::string& sourceId, long timeId, Sink* g) {
    g->nodeRemoved(sourceId, timeId, nodeId);
}

std::string GraphDiff::NodeAdded::toString() const {
    return "an \"" + nodeId + "\"";
}

// NodeRemoved
GraphDiff::NodeRemoved::NodeRemoved(const std::string& nodeId) : NodeAdded(nodeId) {}

void GraphDiff::NodeRemoved::apply(const std::string& sourceId, long timeId, Sink* g) {
    NodeAdded::reverse(sourceId, timeId, g);
}

void GraphDiff::NodeRemoved::reverse(const std::string& sourceId, long timeId, Sink* g) {
    NodeAdded::apply(sourceId, timeId, g);
}

std::string GraphDiff::NodeRemoved::toString() const {
    return "dn \"" + nodeId + "\"";
}

// AttributeAdded
GraphDiff::AttributeAdded::AttributeAdded(ElementType type, const std::string& elementId, const std::string& attrId, const void* value)
    : ElementEvent(type, elementId), attrId(attrId), value(value) {}

void GraphDiff::AttributeAdded::apply(const std::string& sourceId, long timeId, Sink* g) {
    switch (type) {
    case ElementType::NODE:
        g->nodeAttributeAdded(sourceId, timeId, elementId, attrId, value);
        break;
    case ElementType::EDGE:
        g->edgeAttributeAdded(sourceId, timeId, elementId, attrId, value);
        break;
    case ElementType::GRAPH:
        g->graphAttributeAdded(sourceId, timeId, attrId, value);
        break;
    }
}

void GraphDiff::AttributeAdded::reverse(const std::string& sourceId, long timeId, Sink* g) {
    switch (type) {
    case ElementType::NODE:
        g->nodeAttributeRemoved(sourceId, timeId, elementId, attrId);
        break;
    case ElementType::EDGE:
        g->edgeAttributeRemoved(sourceId, timeId, elementId, attrId);
        break;
    case ElementType::GRAPH:
        g->graphAttributeRemoved(sourceId, timeId, attrId);
        break;
    }
}

std::string GraphDiff::AttributeAdded::toString() const {
    return toStringHeader() + " +\"" + attrId + "\":<value>";
}

// AttributeChanged
GraphDiff::AttributeChanged::AttributeChanged(ElementType type, const std::string& elementId, const std::string& attrId, const void* newValue, const void* oldValue)
    : ElementEvent(type, elementId), attrId(attrId), newValue(newValue), oldValue(oldValue) {}

void GraphDiff::AttributeChanged::apply(const std::string& sourceId, long timeId, Sink* g) {
    switch (type) {
    case ElementType::NODE:
        g->nodeAttributeChanged(sourceId, timeId, elementId, attrId, oldValue, newValue);
        break;
    case ElementType::EDGE:
        g->edgeAttributeChanged(sourceId, timeId, elementId, attrId, oldValue, newValue);
        break;
    case ElementType::GRAPH:
        g->graphAttributeChanged(sourceId, timeId, attrId, oldValue, newValue);
        break;
    }
}

void GraphDiff::AttributeChanged::reverse(const std::string& sourceId, long timeId, Sink* g) {
    switch (type) {
    case ElementType::NODE:
        g->nodeAttributeChanged(sourceId, timeId, elementId, attrId, newValue, oldValue);
        break;
    case ElementType::EDGE:
        g->edgeAttributeChanged(sourceId, timeId, elementId, attrId, newValue, oldValue);
        break;
    case ElementType::GRAPH:
        g->graphAttributeChanged(sourceId, timeId, attrId, newValue, oldValue);
        break;
    }
}

std::string GraphDiff::AttributeChanged::toString() const {
    return toStringHeader() + " \"" + attrId + "\":<newValue>";
}

// AttributeRemoved
GraphDiff::AttributeRemoved::AttributeRemoved(ElementType type, const std::string& elementId, const std::string& attrId, const void* oldValue)
    : ElementEvent(type, elementId), attrId(attrId), oldValue(oldValue) {}

void GraphDiff::AttributeRemoved::apply(const std::string& sourceId, long timeId, Sink* g) {
    switch (type) {
    case ElementType::NODE:
        g->nodeAttributeRemoved(sourceId, timeId, elementId, attrId);
        break;
    case ElementType::EDGE:
        g->edgeAttributeRemoved(sourceId, timeId, elementId, attrId);
        break;
    case ElementType::GRAPH:
        g->graphAttributeRemoved(sourceId, timeId, attrId);
        break;
    }
}

void GraphDiff::AttributeRemoved::reverse(const std::string& sourceId, long timeId, Sink* g) {
    switch (type) {
    case ElementType::NODE:
        g->nodeAttributeAdded(sourceId, timeId, elementId, attrId, oldValue);
        break;
    case ElementType::EDGE:
        g->edgeAttributeAdded(sourceId, timeId, elementId, attrId, oldValue);
        break;
    case ElementType::GRAPH:
        g->graphAttributeAdded(sourceId, timeId, attrId, oldValue);
        break;
    }
}

std::string GraphDiff::AttributeRemoved::toString() const {
    return toStringHeader() + " -\"" + attrId + "\":<oldValue>";
}

// EdgeAdded
GraphDiff::EdgeAdded::EdgeAdded(const std::string& edgeId, const std::string& source, const std::string& target, bool directed)
    : edgeId(edgeId), source(source), target(target), directed(directed) {}

void GraphDiff::EdgeAdded::apply(const std::string& sourceId, long timeId, Sink* g) {
    g->edgeAdded(sourceId, timeId, edgeId, source, target, directed);
}

void GraphDiff::EdgeAdded::reverse(const std::string& sourceId, long timeId, Sink* g) {
    g->edgeRemoved(sourceId, timeId, edgeId);
}

std::string GraphDiff::EdgeAdded::toString() const {
    return "ae \"" + edgeId + "\" \"" + source + "\" " + (directed ? ">" : "--") + " \"" + target + "\"";
}

// EdgeRemoved
GraphDiff::EdgeRemoved::EdgeRemoved(const std::string& edgeId, const std::string& source, const std::string& target, bool directed)
    : EdgeAdded(edgeId, source, target, directed) {}

void GraphDiff::EdgeRemoved::apply(const std::string& sourceId, long timeId, Sink* g) {
    EdgeAdded::reverse(sourceId, timeId, g);
}

void GraphDiff::EdgeRemoved::reverse(const std::string& sourceId, long timeId, Sink* g) {
    EdgeAdded::apply(sourceId, timeId, g);
}

std::string GraphDiff::EdgeRemoved::toString() const {
    return "de \"" + edgeId + "\"";
}

// StepBegins
GraphDiff::StepBegins::StepBegins(double oldStep, double newStep) : newStep(newStep), oldStep(oldStep) {}

void GraphDiff::StepBegins::apply(const std::string& sourceId, long timeId, Sink* g) {
    g->stepBegins(sourceId, timeId, newStep);
}

void GraphDiff::StepBegins::reverse(const std::string& sourceId, long timeId, Sink* g) {
    g->stepBegins(sourceId, timeId, oldStep);
}

std::string GraphDiff::StepBegins::toString() const {
    return "st " + std::to_string(newStep);
}

// GraphCleared
GraphDiff::GraphCleared::GraphCleared(Graph* g) {
    this->data = nullptr;

    try {
        FileSinkDGS sink;
        std::ostringstream bytes;
        sink.writeAll(g, bytes);
        this->data = bytes.str();
    } catch (const std::exception& e) {
        std::cerr << "GraphCleared error: " << e.what() << std::endl;
    }
}

void GraphDiff::GraphCleared::apply(const std::string& sourceId, long timeId, Sink* g) {
    g->graphCleared(sourceId, timeId);
}

void GraphDiff::GraphCleared::reverse(const std::string& sourceId, long timeId, Sink* g) {
    try {
        std::istringstream bytes(this->data);
        FileSourceDGS dgs;
        dgs.addSink(g);
        dgs.readAll(bytes);
        dgs.removeSink(g);
    } catch (const std::exception& e) {
        std::cerr << "GraphCleared reverse error: " << e.what() << std::endl;
    }
}

std::string GraphDiff::GraphCleared::toString() const {
    return "cl";
}

// Bridge
GraphDiff::Bridge::Bridge(Graph* g, GraphDiff* diff) : g(g), diff(diff) {
    g->addSink(this);
}

void GraphDiff::Bridge::end() {
    g->removeSink(this);
}

void GraphDiff::Bridge::graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, const void* value) {
    diff->events.push_back(std::make_shared<AttributeAdded>(ElementType::GRAPH, "", attribute, value));
}

void GraphDiff::Bridge::graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, const void* oldValue, const void* newValue) {
    diff->events.push_back(std::make_shared<AttributeChanged>(ElementType::GRAPH, "", attribute, newValue, oldValue));
}

void GraphDiff::Bridge::graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) {
    diff->events.push_back(std::make_shared<AttributeRemoved>(ElementType::GRAPH, "", attribute, nullptr));
}

void GraphDiff::Bridge::nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const void* value) {
    diff->events.push_back(std::make_shared<AttributeAdded>(ElementType::NODE, nodeId, attribute, value));
}

void GraphDiff::Bridge::nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const void* oldValue, const void* newValue) {
    diff->events.push_back(std::make_shared<AttributeChanged>(ElementType::NODE, nodeId, attribute, newValue, oldValue));
}

void GraphDiff::Bridge::nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute) {
    diff->events.push_back(std::make_shared<AttributeRemoved>(ElementType::NODE, nodeId, attribute, nullptr));
}

void GraphDiff::Bridge::edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const void* value) {
    diff->events.push_back(std::make_shared<AttributeAdded>(ElementType::EDGE, edgeId, attribute, value));
}

void GraphDiff::Bridge::edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const void* oldValue, const void* newValue) {
    diff->events.push_back(std::make_shared<AttributeChanged>(ElementType::EDGE, edgeId, attribute, newValue, oldValue));
}

void GraphDiff::Bridge::edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute) {
    diff->events.push_back(std::make_shared<AttributeRemoved>(ElementType::EDGE, edgeId, attribute, nullptr));
}

void GraphDiff::Bridge::nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) {
    diff->events.push_back(std::make_shared<NodeAdded>(nodeId));
}

void GraphDiff::Bridge::nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) {
    Node* n = g->getNode(nodeId);
    for (const auto& key : n->attributeKeys())
        nodeAttributeRemoved(sourceId, timeId, nodeId, key);
    diff->events.push_back(std::make_shared<NodeRemoved>(nodeId));
}

void GraphDiff::Bridge::edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) {
    diff->events.push_back(std::make_shared<EdgeAdded>(edgeId, fromNodeId, toNodeId, directed));
}

void GraphDiff::Bridge::edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) {
    Edge* edge = g->getEdge(edgeId);
    for (const auto& key : edge->attributeKeys())
        edgeAttributeRemoved(sourceId, timeId, edgeId, key);
    diff->events.push_back(std::make_shared<EdgeRemoved>(edgeId, edge->getSourceNode()->getId(), edge->getTargetNode()->getId(), edge->isDirected()));
}

void GraphDiff::Bridge::graphCleared(const std::string& sourceId, long timeId) {
    diff->events.push_back(std::make_shared<GraphCleared>(g));
}

void GraphDiff::Bridge::stepBegins(const std::string& sourceId, long timeId, double step) {
    diff->events.push_back(std::make_shared<StepBegins>(g->getStep(), step));
}
