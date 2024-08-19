#ifndef ATTRIBUTE_SINK_HPP
#define ATTRIBUTE_SINK_HPP

#include <string>
#include <any>

/**
 * Interface to listen to changes on attributes of a graph.
 *
 * <p>
 * The graph attributes listener is called each time an attribute is added, or
 * removed, and each time its value is changed.
 * </p>
 */
class AttributeSink {
public:
    virtual ~AttributeSink() = default;

    /**
     * A graph attribute was added.
     *
     * @param sourceId Identifier of the graph where the attribute changed.
     * @param timeId Timestamp associated with the event.
     * @param attribute The attribute name.
     * @param value The attribute new value.
     */
    virtual void graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, const std::any& value) = 0;

    /**
     * A graph attribute was changed.
     *
     * @param sourceId Identifier of the graph where the attribute changed.
     * @param timeId Timestamp associated with the event.
     * @param attribute The attribute name.
     * @param oldValue The attribute old value.
     * @param newValue The attribute new value.
     */
    virtual void graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) = 0;

    /**
     * A graph attribute was removed.
     *
     * @param sourceId Identifier of the graph where the attribute was removed.
     * @param timeId Timestamp associated with the event.
     * @param attribute The removed attribute name.
     */
    virtual void graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) = 0;

    /**
     * A node attribute was added.
     *
     * @param sourceId Identifier of the graph where the change occurred.
     * @param timeId Timestamp associated with the event.
     * @param nodeId Identifier of the node whose attribute changed.
     * @param attribute The attribute name.
     * @param value The attribute new value.
     */
    virtual void nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& value) = 0;

    /**
     * A node attribute was changed.
     *
     * @param sourceId Identifier of the graph where the change occurred.
     * @param timeId Timestamp associated with the event.
     * @param nodeId Identifier of the node whose attribute changed.
     * @param attribute The attribute name.
     * @param oldValue The attribute old value.
     * @param newValue The attribute new value.
     */
    virtual void nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) = 0;

    /**
     * A node attribute was removed.
     *
     * @param sourceId Identifier of the graph where the attribute was removed.
     * @param timeId Timestamp associated with the event.
     * @param nodeId Identifier of the node whose attribute was removed.
     * @param attribute The removed attribute name.
     */
    virtual void nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute) = 0;

    /**
     * An edge attribute was added.
     *
     * @param sourceId Identifier of the graph where the change occurred.
     * @param timeId Timestamp associated with the event.
     * @param edgeId Identifier of the edge whose attribute changed.
     * @param attribute The attribute name.
     * @param value The attribute new value.
     */
    virtual void edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& value) = 0;

    /**
     * An edge attribute was changed.
     *
     * @param sourceId Identifier of the graph where the change occurred.
     * @param timeId Timestamp associated with the event.
     * @param edgeId Identifier of the edge whose attribute changed.
     * @param attribute The attribute name.
     * @param oldValue The attribute old value.
     * @param newValue The attribute new value.
     */
    virtual void edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) = 0;

    /**
     * An edge attribute was removed.
     *
     * @param sourceId Identifier of the graph where the attribute was removed.
     * @param timeId Timestamp associated with the event.
     * @param edgeId Identifier of the edge whose attribute was removed.
     * @param attribute The removed attribute name.
     */
    virtual void edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute) = 0;
};

#endif // ATTRIBUTE_SINK_HPP
