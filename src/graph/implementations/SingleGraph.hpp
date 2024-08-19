#ifndef SINGLE_GRAPH_HPP
#define SINGLE_GRAPH_HPP

#include "AdjacencyListGraph.hpp"
#include "SingleNode.hpp"
#include <string>
#include <memory>

class SingleGraph : public AdjacencyListGraph {
public:
    // Constructor with full parameters
    SingleGraph(const std::string& id, bool strictChecking, bool autoCreate, int initialNodeCapacity, int initialEdgeCapacity);

    // Constructor with default node and edge capacities
    SingleGraph(const std::string& id, bool strictChecking, bool autoCreate);

    // Constructor with strict checking and without auto-creation
    SingleGraph(const std::string& id);

private:
    void setNodeFactory();  // Helper method to set the node factory
};

#endif // SINGLE_GRAPH_HPP
