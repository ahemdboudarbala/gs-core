#ifndef EDGE_FACTORY_HPP
#define EDGE_FACTORY_HPP

#include <string>
#include <memory>
#include "Edge.hpp"
#include "Node.hpp"

template <typename T>
class EdgeFactory {
public:
    virtual ~EdgeFactory() = default;

    /**
     * Create a new instance of edge.
     * 
     * @param id The new edge identifier.
     * @param src The source node.
     * @param dst The target node.
     * @param directed Is the edge directed (in the direction source toward target).
     * @return The newly created edge.
     */
    virtual std::shared_ptr<T> newInstance(const std::string& id, Node* src, Node* dst, bool directed) = 0;
};

#endif // EDGE_FACTORY_HPP
