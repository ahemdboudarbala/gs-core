#pragma once

#include <string>
#include "SinkTime.hpp"
#include "SourceBase.hpp"
#include "Graph.hpp"
#include "AttributeChangeEvent.hpp"

class GraphListeners : public SourceBase, public Pipe {
public:
    GraphListeners(Graph* graph);

    long newEvent();

    void sendAttributeChangedEvent(const std::string& eltId, ElementType eltType, const std::string& attribute,
                                   AttributeChangeEvent event, const void* oldValue, const void* newValue);

    void sendNodeAdded(const std::string& nodeId);
    void sendNodeRemoved(const std::string& nodeId);

    void sendEdgeAdded(const std::string& edgeId, const std::string& source, const std::string& target, bool directed);
    void sendEdgeRemoved(const std::string& edgeId);

    void sendGraphCleared();
    void sendStepBegins(double step);

    void edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId,
                            const std::string& attribute, const void* value);

    void edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId,
                              const std::string& attribute, const void* oldValue, const void* newValue);

    void edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId,
                              const std::string& attribute);

    void graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, const void* value);

    void graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute,
                               const void* oldValue, const void* newValue);

    void graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute);

    void nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId,
                            const std::string& attribute, const void* value);

    void nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId,
                              const std::string& attribute, const void* oldValue, const void* newValue);

    void nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId,
                              const std::string& attribute);

    void edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId,
                   const std::string& toNodeId, bool directed);

    void edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId);

    void graphCleared(const std::string& sourceId, long timeId);

    void nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId);

    void nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId);

    void stepBegins(const std::string& sourceId, long timeId, double step);

    std::string toString() const;

private:
    SinkTime sinkTime;
    bool passYourWay;
    bool passYourWayAE;
    std::string dnSourceId;
    long dnTimeId;
    Graph* g;
};
