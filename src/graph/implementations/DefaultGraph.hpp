#ifndef DEFAULT_GRAPH_HPP
#define DEFAULT_GRAPH_HPP

#include "SingleGraph.hpp"

class DefaultGraph : public SingleGraph {
public:
    // Constructors

    /**
     * Creates an empty graph.
     * 
     * @param id
     *        Unique identifier of the graph.
     * @param strictChecking
     *        If true any non-fatal error throws an exception.
     * @param autoCreate
     *        If true (and strict checking is false), nodes are automatically
     *        created when referenced when creating an edge, even if not yet
     *        inserted in the graph.
     * @param initialNodeCapacity
     *        Initial capacity of the node storage data structures. Use this if
     *        you know the approximate maximum number of nodes of the graph. The
     *        graph can grow beyond this limit, but storage reallocation is
     *        an expensive operation.
     * @param initialEdgeCapacity
     *        Initial capacity of the edge storage data structures. Use this if
     *        you know the approximate maximum number of edges of the graph. The
     *        graph can grow beyond this limit, but storage reallocation is
     *        an expensive operation.
     */
    DefaultGraph(const std::string& id, bool strictChecking, bool autoCreate, int initialNodeCapacity, int initialEdgeCapacity)
        : SingleGraph(id, strictChecking, autoCreate, initialNodeCapacity, initialEdgeCapacity) {}

    /**
     * Creates an empty graph with default edge and node capacity.
     * 
     * @param id
     *        Unique identifier of the graph.
     * @param strictChecking
     *        If true any non-fatal error throws an exception.
     * @param autoCreate
     *        If true (and strict checking is false), nodes are automatically
     *        created when referenced when creating an edge, even if not yet
     *        inserted in the graph.
     */
    DefaultGraph(const std::string& id, bool strictChecking, bool autoCreate)
        : SingleGraph(id, strictChecking, autoCreate) {}

    /**
     * Creates an empty graph with strict checking and without auto-creation.
     * 
     * @param id
     *        Unique identifier of the graph.
     */
    DefaultGraph(const std::string& id)
        : SingleGraph(id) {}
};

#endif // DEFAULT_GRAPH_HPP
