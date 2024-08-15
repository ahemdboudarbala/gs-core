#ifndef ELEMENT_SINK_HPP
#define ELEMENT_SINK_HPP

#include <string>

/**
 * Interface to listen to changes in the graph structure.
 *
 * <p>
 * Graph elements listeners are called each time an element of the graph (node
 * or edge) is added or removed. It is also called for special events like
 * "steps" that introduce the notion of time in graphs.
 * </p>
 */
class ElementSink {
public:
    virtual ~ElementSink() = default;

    /**
     * A node was inserted in the given graph.
     *
     * @param sourceId Identifier of the graph where the node was added.
     * @param timeId Timestamp associated with the event.
     * @param nodeId Identifier of the added node.
     */
    virtual void nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) = 0;

    /**
     * A node was removed from the graph.
     *
     * @param sourceId Identifier of the graph where the node will be removed.
     * @param timeId Timestamp associated with the event.
     * @param nodeId Identifier of the removed node.
     */
    virtual void nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) = 0;

    /**
     * An edge was inserted in the graph.
     *
     * @param sourceId Identifier of the graph where the edge was added.
     * @param timeId Timestamp associated with the event.
     * @param edgeId Identifier of the added edge.
     * @param fromNodeId Identifier of the first node of the edge.
     * @param toNodeId Identifier of the second node of the edge.
     * @param directed If true, the edge is directed.
     */
    virtual void edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId,
                           const std::string& fromNodeId, const std::string& toNodeId, bool directed) = 0;

    /**
     * An edge of the graph was removed. The nodes the edge connects may already have
     * been removed from the graph.
     *
     * @param sourceId The graph where the edge will be removed.
     * @param timeId Timestamp associated with the event.
     * @param edgeId The edge that will be removed.
     */
    virtual void edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) = 0;

    /**
     * The whole graph was cleared. All the nodes, edges, and attributes of the graph
     * are removed.
     *
     * @param sourceId The graph cleared.
     * @param timeId Timestamp associated with the event.
     */
    virtual void graphCleared(const std::string& sourceId, long timeId) = 0;

    /**
     * <p>
     * Since dynamic graphs are based on discrete event modifications, the notion of
     * step is defined to simulate elapsed time between events. So a step is an event
     * that occurs in the graph; it does not modify it, but it gives a kind of
     * timestamp that allows the tracking of the progress of the graph over time.
     * </p>
     *
     * <p>
     * This kind of event is useful for dynamic algorithms that listen to the
     * dynamic graph and need to measure the time in the graph's evolution.
     * </p>
     *
     * @param sourceId Identifier of the graph where the step starts.
     * @param timeId A numerical value that may give a timestamp to track the evolution
     *               of the graph over time.
     * @param step The step value.
     */
    virtual void stepBegins(const std::string& sourceId, long timeId, double step) = 0;
};

#endif // ELEMENT_SINK_HPP
