#ifndef NODEFACTORY_HPP
#define NODEFACTORY_HPP

#include <memory>
#include <string>
#include "Node.hpp"
#include "Graph.hpp"

template <typename T>
class NodeFactory {
public:
    virtual ~NodeFactory() = default;

    /**
     * Create a new instance of node.
     * 
     * @param id The identifier for the new node.
     * @param graph The graph to which the new node belongs.
     * @return The newly created node.
     */
    virtual std::shared_ptr<T> newInstance(const std::string& id, std::shared_ptr<Graph> graph) = 0;
};

#endif // NODEFACTORY_HPP
