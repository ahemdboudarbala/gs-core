#include "SingleGraph.hpp"
#include "SingleNode.hpp"

// Constructor with full parameters
SingleGraph::SingleGraph(const std::string& id, bool strictChecking, bool autoCreate, int initialNodeCapacity, int initialEdgeCapacity)
    : AdjacencyListGraph(id, strictChecking, autoCreate, initialNodeCapacity, initialEdgeCapacity) {
    setNodeFactory();
}

// Constructor with default node and edge capacities
SingleGraph::SingleGraph(const std::string& id, bool strictChecking, bool autoCreate)
    : SingleGraph(id, strictChecking, autoCreate, DEFAULT_NODE_CAPACITY, DEFAULT_EDGE_CAPACITY) {}

// Constructor with strict checking and without auto-creation
SingleGraph::SingleGraph(const std::string& id)
    : SingleGraph(id, true, false) {}

void SingleGraph::setNodeFactory() {
    this->nodeFactory = std::make_shared<NodeFactory<SingleNode>>(
        [](const std::string& id, std::shared_ptr<Graph> graph) {
            return std::make_shared<SingleNode>(std::dynamic_pointer_cast<AbstractGraph>(graph), id);
        }
    );
}
