#include "MultiGraph.hpp"
#include "MultiNode.hpp"

// Constructor with custom capacities
MultiGraph::MultiGraph(const std::string& id, bool strictChecking, bool autoCreate, int initialNodeCapacity, int initialEdgeCapacity)
    : AdjacencyListGraph(id, strictChecking, autoCreate, initialNodeCapacity, initialEdgeCapacity) {
    // Set the node factory to produce MultiNode instances
    setNodeFactory(createNodeFactory());
}

// Constructor with default capacities
MultiGraph::MultiGraph(const std::string& id, bool strictChecking, bool autoCreate)
    : AdjacencyListGraph(id, strictChecking, autoCreate, DEFAULT_NODE_CAPACITY, DEFAULT_EDGE_CAPACITY) {
    // Set the node factory to produce MultiNode instances
    setNodeFactory(createNodeFactory());
}

// Constructor with strict checking and without auto-creation
MultiGraph::MultiGraph(const std::string& id)
    : AdjacencyListGraph(id, true, false) {
    // Set the node factory to produce MultiNode instances
    setNodeFactory(createNodeFactory());
}

// Creates and returns a node factory for MultiNode
std::shared_ptr<NodeFactory<MultiNode>> MultiGraph::createNodeFactory() {
    return std::make_shared<NodeFactory<MultiNode>>(
        [](const std::string& id, std::shared_ptr<Graph> graph) -> std::shared_ptr<MultiNode> {
            return std::make_shared<MultiNode>(std::dynamic_pointer_cast<AbstractGraph>(graph), id);
        }
    );
}
