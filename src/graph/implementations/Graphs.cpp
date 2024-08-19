#include "Graphs.hpp"

// Static member initialization
std::mutex Graphs::SynchronizedElement::attributeMutex;

std::shared_ptr<Graph> Graphs::synchronizedGraph(std::shared_ptr<Graph> g) {
    return std::make_shared<SynchronizedGraph>(g);
}

std::shared_ptr<Graph> Graphs::merge(const std::vector<std::shared_ptr<Graph>>& graphs) {
    if (graphs.empty()) {
        return std::make_shared<MultiGraph>("void-merge");
    }

    std::string id = "merge";
    for (const auto& g : graphs) {
        id += "-" + g->getId();
    }

    std::shared_ptr<Graph> result;
    try {
        result = graphs[0]->clone(id);  // Assuming Graph has a clone method
    } catch (const std::exception& e) {
        std::cerr << "Cannot create a graph of " << graphs[0]->getId() << "." << std::endl;
        result = std::make_shared<MultiGraph>(id);
    }

    mergeIn(result, graphs);
    return result;
}

void Graphs::mergeIn(std::shared_ptr<Graph> result, const std::vector<std::shared_ptr<Graph>>& graphs) {
    bool strict = result->isStrict();
    GraphReplay replay("replay-" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()));

    replay.addSink(result);
    result->setStrict(false);

    for (const auto& g : graphs) {
        replay.replay(g);
    }

    replay.removeSink(result);
    result->setStrict(strict);
}

std::shared_ptr<Graph> Graphs::clone(std::shared_ptr<Graph> g) {
    std::shared_ptr<Graph> copy;
    try {
        copy = g->clone(g->getId());
    } catch (const std::exception& e) {
        std::cerr << "Cannot create a graph of " << g->getId() << "." << std::endl;
        copy = std::make_shared<AdjacencyListGraph>(g->getId());
    }

    copyAttributes(g, copy);

    for (size_t i = 0; i < g->getNodeCount(); ++i) {
        auto source = g->getNode(i);
        auto target = copy->addNode(source->getId());
        copyAttributes(source, target);
    }

    for (size_t i = 0; i < g->getEdgeCount(); ++i) {
        auto source = g->getEdge(i);
        auto target = copy->addEdge(source->getId(), source->getSourceNode()->getId(), source->getTargetNode()->getId(),
                                    source->isDirected());
        copyAttributes(source, target);
    }

    return copy;
}

template<typename T>
void Graphs::copyAttributes(const std::shared_ptr<T>& source, const std::shared_ptr<T>& target) {
    for (const auto& key : source->attributeKeys()) {
        auto value = source->getAttribute(key);
        value = checkedArrayOrCollectionCopy(value);
        target->setAttribute(key, value);
    }
}

template<typename T>
T Graphs::checkedArrayOrCollectionCopy(const T& o) {
    if constexpr (std::is_array_v<T>) {
        T c;
        for (size_t i = 0; i < std::size(o); ++i) {
            c[i] = checkedArrayOrCollectionCopy(o[i]);
        }
        return c;
    } else if constexpr (std::is_same_v<T, std::vector<typename T::value_type>>) {
        T t;
        for (const auto& element : o) {
            t.push_back(checkedArrayOrCollectionCopy(element));
        }
        return t;
    } else {
        return o;
    }
}

// SynchronizedElement class implementation
Graphs::SynchronizedElement::SynchronizedElement(std::shared_ptr<Element> element)
    : wrappedElement(std::move(element)) {}

void Graphs::SynchronizedElement::setAttribute(const std::string& attribute, const std::vector<std::string>& values) {
    std::lock_guard<std::mutex> lock(attributeMutex);
    wrappedElement->setAttribute(attribute, values);
}

void Graphs::SynchronizedElement::clearAttributes() {
    std::lock_guard<std::mutex> lock(attributeMutex);
    wrappedElement->clearAttributes();
}

std::string Graphs::SynchronizedElement::getId() const {
    return wrappedElement->getId();
}

int Graphs::SynchronizedElement::getIndex() const {
    return wrappedElement->getIndex();
}

// SynchronizedGraph class implementation
Graphs::SynchronizedGraph::SynchronizedGraph(std::shared_ptr<Graph> g)
    : SynchronizedElement(g), graph(std::move(g)) {}

std::shared_ptr<Node> Graphs::SynchronizedGraph::addNode(const std::string& id) {
    std::lock_guard<std::mutex> lock(elementMutex);
    auto node = graph->addNode(id);
    auto synchronizedNode = std::make_shared<SynchronizedNode>(std::dynamic_pointer_cast<SynchronizedGraph>(shared_from_this()), node);
    synchronizedNodes[id] = synchronizedNode;
    return synchronizedNode;
}

std::shared_ptr<Edge> Graphs::SynchronizedGraph::addEdge(const std::string& id, const std::string& node1, const std::string& node2) {
    std::lock_guard<std::mutex> lock(elementMutex);
    auto edge = graph->addEdge(id, node1, node2);
    auto synchronizedEdge = std::make_shared<SynchronizedEdge>(std::dynamic_pointer_cast<SynchronizedGraph>(shared_from_this()), edge);
    synchronizedEdges[id] = synchronizedEdge;
    return synchronizedEdge;
}

std::shared_ptr<Edge> Graphs::SynchronizedGraph::addEdge(const std::string& id, const std::string& from, const std::string& to, bool directed) {
    std::lock_guard<std::mutex> lock(elementMutex);
    auto edge = graph->addEdge(id, from, to, directed);
    auto synchronizedEdge = std::make_shared<SynchronizedEdge>(std::dynamic_pointer_cast<SynchronizedGraph>(shared_from_this()), edge);
    synchronizedEdges[id] = synchronizedEdge;
    return synchronizedEdge;
}

std::shared_ptr<Node> Graphs::SynchronizedGraph::getNode(const std::string& id) {
    std::lock_guard<std::mutex> lock(elementMutex);
    auto it = synchronizedNodes.find(id);
    if (it != synchronizedNodes.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<Edge> Graphs::SynchronizedGraph::getEdge(const std::string& id) {
    std::lock_guard<std::mutex> lock(elementMutex);
    auto it = synchronizedEdges.find(id);
    if (it != synchronizedEdges.end()) {
        return it->second;
    }
    return nullptr;
}

// SynchronizedNode class implementation
Graphs::SynchronizedNode::SynchronizedNode(std::shared_ptr<SynchronizedGraph> sg, std::shared_ptr<Node> n)
    : SynchronizedElement(n), sg(std::move(sg)) {}

std::shared_ptr<Edge> Graphs::SynchronizedNode::getEdge(int i) {
    std::lock_guard<std::mutex> lock(attributeMutex);
    return sg->getEdge(wrappedElement->getEdge(i)->getId());
}

std::shared_ptr<Edge> Graphs::SynchronizedNode::getEdgeBetween(const std::string& id) {
    std::lock_guard<std::mutex> lock(attributeMutex);
    return sg->getEdge(wrappedElement->getEdgeBetween(id)->getId());
}

// SynchronizedEdge class implementation
Graphs::SynchronizedEdge::SynchronizedEdge(std::shared_ptr<SynchronizedGraph> sg, std::shared_ptr<Edge> e)
    : SynchronizedElement(e), sg(std::move(sg)) {}

std::shared_ptr<Node> Graphs::SynchronizedEdge::getNode0() {
    std::lock_guard<std::mutex> lock(attributeMutex);
    return sg->getNode(wrappedElement->getNode0()->getId());
}

std::shared_ptr<Node> Graphs::SynchronizedEdge::getNode1() {
    std::lock_guard<std::mutex> lock(attributeMutex);
    return sg->getNode(wrappedElement->getNode1()->getId());
}

std::shared_ptr<Node> Graphs::SynchronizedEdge::getOpposite(std::shared_ptr<Node> node) {
    std::lock_guard<std::mutex> lock(attributeMutex);
    return sg->getNode(wrappedElement->getOpposite(node)->getId());
}
