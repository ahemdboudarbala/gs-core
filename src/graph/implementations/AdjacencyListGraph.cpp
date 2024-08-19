#include "AdjacencyListGraph.hpp"
#include "AbstractEdge.hpp"
#include "AdjacencyListNode.hpp"
#include "graph/ElementNotFoundException.hpp"
#include "graph/IdAlreadyInUseException.hpp"
#include <stdexcept>
#include <algorithm>

// Constructors
AdjacencyListGraph::AdjacencyListGraph(const std::string& id, bool strictChecking, bool autoCreate, 
                                       int initialNodeCapacity, int initialEdgeCapacity)
    : AbstractGraph(id, strictChecking, autoCreate), nodeCount(0), edgeCount(0) {
    
    if (initialNodeCapacity < DEFAULT_NODE_CAPACITY) {
        initialNodeCapacity = DEFAULT_NODE_CAPACITY;
    }
    if (initialEdgeCapacity < DEFAULT_EDGE_CAPACITY) {
        initialEdgeCapacity = DEFAULT_EDGE_CAPACITY;
    }

    nodeMap.reserve(4 * initialNodeCapacity / 3 + 1);
    edgeMap.reserve(4 * initialEdgeCapacity / 3 + 1);
    nodeArray.reserve(initialNodeCapacity);
    edgeArray.reserve(initialEdgeCapacity);

    setNodeFactory(std::make_shared<NodeFactory<AdjacencyListNode>>([](const std::string& id, std::shared_ptr<Graph> graph) {
        return std::make_shared<AdjacencyListNode>(std::dynamic_pointer_cast<AbstractGraph>(graph), id);
    }));

    setEdgeFactory(std::make_shared<EdgeFactory<AbstractEdge>>([](const std::string& id, std::shared_ptr<Node> src, 
                                                                  std::shared_ptr<Node> dst, bool directed) {
        return std::make_shared<AbstractEdge>(id, std::dynamic_pointer_cast<AbstractNode>(src), 
                                              std::dynamic_pointer_cast<AbstractNode>(dst), directed);
    }));
}

AdjacencyListGraph::AdjacencyListGraph(const std::string& id, bool strictChecking, bool autoCreate)
    : AdjacencyListGraph(id, strictChecking, autoCreate, DEFAULT_NODE_CAPACITY, DEFAULT_EDGE_CAPACITY) {}

AdjacencyListGraph::AdjacencyListGraph(const std::string& id)
    : AdjacencyListGraph(id, true, false) {}

// Callbacks
void AdjacencyListGraph::addEdgeCallback(std::shared_ptr<AbstractEdge> edge) {
    edgeMap[edge->getId()] = edge;

    if (edgeCount == edgeArray.size()) {
        edgeArray.resize(static_cast<int>(edgeArray.size() * GROW_FACTOR) + 1);
    }
    edgeArray[edgeCount] = edge;
    edge->setIndex(edgeCount++);
}

void AdjacencyListGraph::addNodeCallback(std::shared_ptr<AbstractNode> node) {
    nodeMap[node->getId()] = node;

    if (nodeCount == nodeArray.size()) {
        nodeArray.resize(static_cast<int>(nodeArray.size() * GROW_FACTOR) + 1);
    }
    nodeArray[nodeCount] = node;
    node->setIndex(nodeCount++);
}

void AdjacencyListGraph::removeEdgeCallback(std::shared_ptr<AbstractEdge> edge) {
    edgeMap.erase(edge->getId());
    int i = edge->getIndex();
    edgeArray[i] = edgeArray[--edgeCount];
    edgeArray[i]->setIndex(i);
    edgeArray[edgeCount] = nullptr;
}

void AdjacencyListGraph::removeNodeCallback(std::shared_ptr<AbstractNode> node) {
    nodeMap.erase(node->getId());
    int i = node->getIndex();
    nodeArray[i] = nodeArray[--nodeCount];
    nodeArray[i]->setIndex(i);
    nodeArray[nodeCount] = nullptr;
}

void AdjacencyListGraph::clearCallback() {
    nodeMap.clear();
    edgeMap.clear();
    nodeArray.clear();
    edgeArray.clear();
    nodeCount = edgeCount = 0;
}

// Streams and Counts
std::vector<std::shared_ptr<Node>> AdjacencyListGraph::nodes() const {
    return std::vector<std::shared_ptr<Node>>(nodeArray.begin(), nodeArray.begin() + nodeCount);
}

std::vector<std::shared_ptr<Edge>> AdjacencyListGraph::edges() const {
    return std::vector<std::shared_ptr<Edge>>(edgeArray.begin(), edgeArray.begin() + edgeCount);
}

std::shared_ptr<Edge> AdjacencyListGraph::getEdge(const std::string& id) const {
    auto it = edgeMap.find(id);
    return (it != edgeMap.end()) ? it->second : nullptr;
}

std::shared_ptr<Edge> AdjacencyListGraph::getEdge(int index) const {
    if (index < 0 || index >= edgeCount) {
        throw std::out_of_range("Edge " + std::to_string(index) + " does not exist");
    }
    return edgeArray[index];
}

int AdjacencyListGraph::getEdgeCount() const {
    return edgeCount;
}

std::shared_ptr<Node> AdjacencyListGraph::getNode(const std::string& id) const {
    auto it = nodeMap.find(id);
    return (it != nodeMap.end()) ? it->second : nullptr;
}

std::shared_ptr<Node> AdjacencyListGraph::getNode(int index) const {
    if (index < 0 || index >= nodeCount) {
        throw std::out_of_range("Node " + std::to_string(index) + " does not exist");
    }
    return nodeArray[index];
}

int AdjacencyListGraph::getNodeCount() const {
    return nodeCount;
}

// Iterators
template<typename T>
AdjacencyListGraph::EdgeIterator<T>::EdgeIterator(AdjacencyListGraph& graph)
    : graph(graph), iNext(0), iPrev(-1) {}

template<typename T>
bool AdjacencyListGraph::EdgeIterator<T>::hasNext() {
    return iNext < graph.edgeCount;
}

template<typename T>
T AdjacencyListGraph::EdgeIterator<T>::next() {
    if (iNext >= graph.edgeCount) {
        throw std::out_of_range("No more edges");
    }
    iPrev = iNext++;
    return std::dynamic_pointer_cast<T>(graph.edgeArray[iPrev]);
}

template<typename T>
void AdjacencyListGraph::EdgeIterator<T>::remove() {
    if (iPrev == -1) {
        throw std::logic_error("Invalid state");
    }
    graph.removeEdge(graph.edgeArray[iPrev], true, true, true);
    iNext = iPrev;
    iPrev = -1;
}

template<typename T>
AdjacencyListGraph::NodeIterator<T>::NodeIterator(AdjacencyListGraph& graph)
    : graph(graph), iNext(0), iPrev(-1) {}

template<typename T>
bool AdjacencyListGraph::NodeIterator<T>::hasNext() {
    return iNext < graph.nodeCount;
}

template<typename T>
T AdjacencyListGraph::NodeIterator<T>::next() {
    if (iNext >= graph.nodeCount) {
        throw std::out_of_range("No more nodes");
    }
    iPrev = iNext++;
    return std::dynamic_pointer_cast<T>(graph.nodeArray[iPrev]);
}

template<typename T>
void AdjacencyListGraph::NodeIterator<T>::remove() {
    if (iPrev == -1) {
        throw std::logic_error("Invalid state");
    }
    graph.removeNode(graph.nodeArray[iPrev], true);
    iNext = iPrev;
    iPrev = -1;
}
