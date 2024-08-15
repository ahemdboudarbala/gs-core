#ifndef VERBOSE_SINK_HPP
#define VERBOSE_SINK_HPP

#include "Sink.hpp"
#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <any>

/**
 * A sink that can be used to display events in a PrintStream like std::cout.
 * Format of messages can be customized by inserting keywords quoted with '%' in the format.
 */
class VerboseSink : public Sink {
public:
    enum class EventType {
        ADD_NODE, ADD_NODE_ATTRIBUTE, SET_NODE_ATTRIBUTE, DEL_NODE_ATTRIBUTE, DEL_NODE,
        ADD_EDGE, ADD_EDGE_ATTRIBUTE, SET_EDGE_ATTRIBUTE, DEL_EDGE_ATTRIBUTE, DEL_EDGE,
        ADD_GRAPH_ATTRIBUTE, SET_GRAPH_ATTRIBUTE, DEL_GRAPH_ATTRIBUTE,
        CLEAR, STEP_BEGINS
    };

    VerboseSink();
    VerboseSink(std::ostream& out);

    void setAutoFlush(bool on);
    void setEventFormat(EventType type, const std::string& format);
    void setEventEnabled(EventType type, bool on);
    void setElementEventEnabled(bool on);
    void setAttributeEventEnabled(bool on);
    void setPrefix(const std::string& prefix);
    void setSuffix(const std::string& suffix);

    void edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, 
                            const std::string& attribute, const std::any& value) override;

    void edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, 
                              const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;

    void edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, 
                              const std::string& attribute) override;

    void graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, 
                             const std::any& value) override;

    void graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, 
                               const std::any& oldValue, const std::any& newValue) override;

    void graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) override;

    void nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, 
                            const std::string& attribute, const std::any& value) override;

    void nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, 
                              const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;

    void nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, 
                              const std::string& attribute) override;

    void edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, 
                   const std::string& fromNodeId, const std::string& toNodeId, bool directed) override;

    void edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) override;

    void graphCleared(const std::string& sourceId, long timeId) override;

    void nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) override;

    void nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) override;

    void stepBegins(const std::string& sourceId, long timeId, double step) override;

private:
    using Args = std::map<std::string, std::any>;

    void print(EventType type, Args& args);
    Args* argsPnP(Args* args);
    std::string toStringValue(const std::any& value);

    std::ostream& out;
    bool autoflush = false;
    std::map<EventType, std::string> formats;
    std::map<EventType, bool> enable;
    std::stack<Args*> argsStack;

    std::string prefix;
    std::string suffix;
};

#endif // VERBOSE_SINK_HPP
