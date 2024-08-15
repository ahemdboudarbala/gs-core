#include "AdjacencyListNode.hpp"

// Constructor
AdjacencyListNode::AdjacencyListNode(std::shared_ptr<AbstractGraph> graph, const std::string& id)
    : AbstractNode(graph, id), ioStart(0), oStart(0), degree(0) {
    edges.reserve(INITIAL_EDGE_CAPACITY);
}

// Helpers
char AdjacencyListNode::edgeType(const std::shared_ptr<AbstractEdge>& e) const {
    if (!e->isDirected() || e->getSourceNode() == e->getTargetNode())
        return IO_EDGE;
    return e->getSourceNode() == shared_from_this() ? O_EDGE : I_EDGE;
}

template<typename T>
std::shared_ptr<T> AdjacencyListNode::locateEdge(std::shared_ptr<Node> opposite, char type) {
    int start = 0;
    int end = degree;

    if (type == I_EDGE)
        end = oStart;
    else if (type == O_EDGE)
        start = ioStart;

    for (int i = start; i < end; i++) {
        if (edges[i]->getOpposite(shared_from_this()) == opposite)
            return std::dynamic_pointer_cast<T>(edges[i]);
    }
    return nullptr;
}

void AdjacencyListNode::removeEdge(int i) {
    if (i >= oStart) {
        edges[i] = edges[--degree];
    } else if (i >= ioStart) {
        edges[i] = edges[--oStart];
        edges[oStart] = edges[--degree];
    } else {
        edges[i] = edges[--ioStart];
        edges[ioStart] = edges[--oStart];
        edges[oStart] = edges[--degree];
    }
}

// Callbacks
bool AdjacencyListNode::addEdgeCallback(std::shared_ptr<AbstractEdge> edge) {
    if (edges.size() == degree) {
        edges.resize(static_cast<int>(GROWTH_FACTOR * edges.size()) + 1);
    }

    char type = edgeType(edge);

    if (type == O_EDGE) {
        edges[degree++] = edge;
    } else if (type == IO_EDGE) {
        edges[degree++] = edges[oStart];
        edges[oStart++] = edge;
    } else {
        edges[degree++] = edges[oStart];
        edges[oStart++] = edges[ioStart];
        edges[ioStart++] = edge;
    }
    return true;
}

void AdjacencyListNode::removeEdgeCallback(std::shared_ptr<AbstractEdge> edge) {
    char type = edgeType(edge);
    int i = 0;

    if (type == IO_EDGE)
        i = ioStart;
    else if (type == O_EDGE)
        i = oStart;

    while (edges[i] != edge)
        i++;

    removeEdge(i);
}

void AdjacencyListNode::clearCallback() {
    edges.clear();
    ioStart = oStart = degree = 0;
}

// Access methods
int AdjacencyListNode::getDegree() const {
    return degree;
}

int AdjacencyListNode::getInDegree() const {
    return oStart;
}

int AdjacencyListNode::getOutDegree() const {
    return degree - ioStart;
}

std::shared_ptr<Edge> AdjacencyListNode::getEdge(int i) const {
    if (i < 0 || i >= degree)
        throw std::out_of_range("Node \"" + id + "\" has no edge " + std::to_string(i));
    return edges[i];
}

std::shared_ptr<Edge> AdjacencyListNode::getEnteringEdge(int i) const {
    if (i < 0 || i >= getInDegree())
        throw std::out_of_range("Node \"" + id + "\" has no entering edge " + std::to_string(i));
    return edges[i];
}

std::shared_ptr<Edge> AdjacencyListNode::getLeavingEdge(int i) const {
    if (i < 0 || i >= getOutDegree())
        throw std::out_of_range("Node \"" + id + "\" has no leaving edge " + std::to_string(i));
    return edges[ioStart + i];
}

std::shared_ptr<Edge> AdjacencyListNode::getEdgeBetween(std::shared_ptr<Node> node) const {
    return locateEdge<Edge>(node, IO_EDGE);
}

std::shared_ptr<Edge> AdjacencyListNode::getEdgeFrom(std::shared_ptr<Node> node) const {
    return locateEdge<Edge>(node, I_EDGE);
}

std::shared_ptr<Edge> AdjacencyListNode::getEdgeToward(std::shared_ptr<Node> node) const {
    return locateEdge<Edge>(node, O_EDGE);
}

// Iterators
std::vector<std::shared_ptr<Edge>> AdjacencyListNode::edges() const {
    return std::vector<std::shared_ptr<Edge>>(edges.begin(), edges.begin() + degree);
}

std::vector<std::shared_ptr<Edge>> AdjacencyListNode::enteringEdges() const {
    return std::vector<std::shared_ptr<Edge>>(edges.begin(), edges.begin() + oStart);
}

std::vector<std::shared_ptr<Edge>> AdjacencyListNode::leavingEdges() const {
    return std::vector<std::shared_ptr<Edge>>(edges.begin() + ioStart, edges.begin() + degree);
}

// EdgeIterator implementation
template<typename T>
AdjacencyListNode::EdgeIterator<T>::EdgeIterator(AdjacencyListNode& node, char type)
    : node(node), iPrev(-1), iNext(0), iEnd(node.degree) {
    if (type == I_EDGE)
        iEnd = node.oStart;
    else if (type == O_EDGE)
        iNext = node.ioStart;
}

template<typename T>
bool AdjacencyListNode::EdgeIterator<T>::hasNext() {
    return iNext < iEnd;
}

template<typename T>
std::shared_ptr<T> AdjacencyListNode::EdgeIterator<T>::next() {
    if (iNext >= iEnd)
        throw std::out_of_range("No more edges");
    iPrev = iNext++;
    return std::dynamic_pointer_cast<T>(node.edges[iPrev]);
}

template<typename T>
void AdjacencyListNode::EdgeIterator<T>::remove() {
    if (iPrev == -1)
        throw std::logic_error("Invalid state");
    std::shared_ptr<AbstractEdge> e = node.edges[iPrev];
    node.graph->removeEdge(e, true, e->getSourceNode() != node.shared_from_this(), e->getTargetNode() != node.shared_from_this());
    node.removeEdge(iPrev);
    iNext = iPrev;
    iPrev = -1;
    iEnd--;
}
