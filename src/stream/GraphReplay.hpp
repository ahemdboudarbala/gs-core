#ifndef GRAPH_REPLAY_HPP
#define GRAPH_REPLAY_HPP

#include "SourceBase.hpp"
#include "Source.hpp"
#include "Graph.hpp"

/**
 * A simple source of graph events that takes an existing graph and creates a
 * flow of events by enumerating all nodes, edges, and attributes of the graph.
 * 
 * This class echoes the structure of a graph to registered sinks, exporting
 * graph attributes, node creations, and edge creations.
 */
class GraphReplay : public SourceBase, public Source {
public:
    explicit GraphReplay(const std::string& id);

    /**
     * Echo each element and attribute of the graph to the registered sinks.
     *
     * @param graph The graph to export.
     */
    void replay(const Graph& graph);
};

#endif // GRAPH_REPLAY_HPP
