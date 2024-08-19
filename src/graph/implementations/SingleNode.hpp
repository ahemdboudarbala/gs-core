#ifndef SINGLE_NODE_HPP
#define SINGLE_NODE_HPP

#include "AdjacencyListNode.hpp"
#include "AbstractEdge.hpp"
#include "AbstractGraph.hpp"
#include <string>
#include <unordered_map>
#include <memory>

class SingleNode : public AdjacencyListNode {
public:
    // Constructor
    SingleNode(std::shared_ptr<AbstractGraph> graph, const std::string& id);

protected:
    // TwoEdges structure to hold in and out edges
    struct TwoEdges {
        std::shared_ptr<AbstractEdge> in, out;
    };

    std::unordered_map<std::shared_ptr<AbstractNode>, TwoEdges> neighborMap;

    // Helper method to locate an edge
    template <typename T>
    std::shared_ptr<T> locateEdge(std::shared_ptr<Node> opposite, char type);

    // Remove an edge
    void removeEdge(int i) override;

    // Add an edge callback
    bool addEdgeCallback(std::shared_ptr<AbstractEdge> edge) override;

    // Clear callback
    void clearCallback() override;
};

#endif // SINGLE_NODE_HPP
