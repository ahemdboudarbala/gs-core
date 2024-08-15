#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include <vector>
#include <memory>
#include "Node.hpp"
#include "Edge.hpp"

class Structure {
public:
    virtual ~Structure() = default;

    /**
     * Get the number of nodes in this graph.
     * 
     * @return The number of nodes.
     */
    virtual int getNodeCount() const = 0;

    /**
     * Get the number of edges in this graph.
     * 
     * @return The number of edges.
     */
    virtual int getEdgeCount() const = 0;

    /**
     * Get a list of nodes in this structure.
     * 
     * @return A vector of shared pointers to Node objects.
     */
    virtual std::vector<std::shared_ptr<Node>> nodes() const = 0;

    /**
     * Get a list of edges in this structure.
     * 
     * @return A vector of shared pointers to Edge objects.
     */
    virtual std::vector<std::shared_ptr<Edge>> edges() const = 0;
};

#endif // STRUCTURE_HPP
