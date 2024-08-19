#include "SingleNode.hpp"

// Constructor
SingleNode::SingleNode(std::shared_ptr<AbstractGraph> graph, const std::string& id)
    : AdjacencyListNode(graph, id) {
    neighborMap.reserve(4 * INITIAL_EDGE_CAPACITY / 3 + 1);
}

// Helper method to locate an edge
template <typename T>
std::shared_ptr<T> SingleNode::locateEdge(std::shared_ptr<Node> opposite, char type) {
    auto it = neighborMap.find(std::dynamic_pointer_cast<AbstractNode>(opposite));
    if (it == neighborMap.end()) {
        return nullptr;
    }

    TwoEdges& ee = it->second;

    if (type == IO_EDGE) {
        return std::dynamic_pointer_cast<T>(ee.in ? ee.in : ee.out);
    }

    return std::dynamic_pointer_cast<T>(type == I_EDGE ? ee.in : ee.out);
}

// Remove an edge
void SingleNode::removeEdge(int i) {
    auto opposite = std::dynamic_pointer_cast<AbstractNode>(edges[i]->getOpposite(shared_from_this()));
    TwoEdges& ee = neighborMap[opposite];
    char type = edgeType(edges[i]);

    if (type != O_EDGE) {
        ee.in = nullptr;
    }

    if (type != I_EDGE) {
        ee.out = nullptr;
    }

    if (!ee.in && !ee.out) {
        neighborMap.erase(opposite);
    }

    AdjacencyListNode::removeEdge(i);
}

// Add an edge callback
bool SingleNode::addEdgeCallback(std::shared_ptr<AbstractEdge> edge) {
    auto opposite = std::dynamic_pointer_cast<AbstractNode>(edge->getOpposite(shared_from_this()));
    TwoEdges& ee = neighborMap[opposite];

    char type = edgeType(edge);

    if (type != O_EDGE) {
        if (ee.in) {
            return false;
        }
        ee.in = edge;
    }

    if (type != I_EDGE) {
        if (ee.out) {
            return false;
        }
        ee.out = edge;
    }

    neighborMap[opposite] = ee;

    return AdjacencyListNode::addEdgeCallback(edge);
}

// Clear callback
void SingleNode::clearCallback() {
    neighborMap.clear();
    AdjacencyListNode::clearCallback();
}
