#include "MultiNode.hpp"
#include <algorithm>
#include <iterator>

// Constructor
MultiNode::MultiNode(std::shared_ptr<AbstractGraph> graph, const std::string& id)
    : AdjacencyListNode(graph, id) {
    neighborMap.reserve(4 * INITIAL_EDGE_CAPACITY / 3 + 1);
}

// Locate an edge between this node and the opposite node
template<typename T>
std::shared_ptr<T> MultiNode::locateEdge(const std::shared_ptr<AbstractNode>& opposite, char type) {
    auto it = neighborMap.find(opposite);
    if (it == neighborMap.end())
        return nullptr;

    for (const auto& edge : it->second) {
        char edgeType = edgeType(edge);
        if ((type != I_EDGE || edgeType != O_EDGE) && (type != O_EDGE || edgeType != I_EDGE))
            return std::dynamic_pointer_cast<T>(edge);
    }

    return nullptr;
}

// Remove an edge by index
void MultiNode::removeEdge(int i) {
    auto opposite = edges[i]->getOpposite(shared_from_this());
    auto it = neighborMap.find(opposite);
    if (it != neighborMap.end()) {
        it->second.erase(std::remove(it->second.begin(), it->second.end(), edges[i]), it->second.end());
        if (it->second.empty())
            neighborMap.erase(it);
    }
    AdjacencyListNode::removeEdge(i);
}

// Add an edge callback
bool MultiNode::addEdgeCallback(std::shared_ptr<AbstractEdge> edge) {
    auto opposite = edge->getOpposite(shared_from_this());
    auto& edgeList = neighborMap[opposite];
    edgeList.push_back(edge);
    return AdjacencyListNode::addEdgeCallback(edge);
}

// Clear callback
void MultiNode::clearCallback() {
    neighborMap.clear();
    AdjacencyListNode::clearCallback();
}

// Get the set of edges between this node and another node
template<typename T>
std::vector<std::shared_ptr<T>> MultiNode::getEdgeSetBetween(const std::shared_ptr<AbstractNode>& node) const {
    auto it = neighborMap.find(node);
    if (it == neighborMap.end())
        return {};

    std::vector<std::shared_ptr<T>> result;
    std::transform(it->second.begin(), it->second.end(), std::back_inserter(result),
                   [](const std::shared_ptr<AbstractEdge>& edge) {
                       return std::dynamic_pointer_cast<T>(edge);
                   });
    return result;
}

// Get the set of edges between this node and another node by ID
template<typename T>
std::vector<std::shared_ptr<T>> MultiNode::getEdgeSetBetween(const std::string& id) const {
    return getEdgeSetBetween<T>(graph->getNode(id));
}

// Get the set of edges between this node and another node by index
template<typename T>
std::vector<std::shared_ptr<T>> MultiNode::getEdgeSetBetween(int index) const {
    return getEdgeSetBetween<T>(graph->getNode(index));
}
