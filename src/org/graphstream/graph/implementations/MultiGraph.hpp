#ifndef MULTIGRAPH_HPP
#define MULTIGRAPH_HPP

#include <string>
#include <memory>
#include "AdjacencyListGraph.hpp"
#include "MultiNode.hpp"

class MultiGraph : public AdjacencyListGraph {
public:
    MultiGraph(const std::string& id, bool strictChecking, bool autoCreate, int initialNodeCapacity, int initialEdgeCapacity);

    MultiGraph(const std::string& id, bool strictChecking, bool autoCreate);

    MultiGraph(const std::string& id);

protected:
    std::shared_ptr<NodeFactory<MultiNode>> createNodeFactory();
};

#endif // MULTIGRAPH_HPP
