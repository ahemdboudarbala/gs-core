#ifndef SINK_ADAPTER_HPP
#define SINK_ADAPTER_HPP

#include "Sink.hpp"

class SinkAdapter : public Sink {
public:
    virtual ~SinkAdapter() = default;

    virtual void edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const void* value) override {}

    virtual void edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const void* oldValue, const void* newValue) override {}

    virtual void edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute) override {}

    virtual void graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, const void* value) override {}

    virtual void graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, const void* oldValue, const void* newValue) override {}

    virtual void graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) override {}

    virtual void nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const void* value) override {}

    virtual void nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const void* oldValue, const void* newValue) override {}

    virtual void nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute) override {}

    virtual void edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) override {}

    virtual void edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) override {}

    virtual void graphCleared(const std::string& sourceId, long timeId) override {}

    virtual void nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) override {}

    virtual void nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) override {}

    virtual void stepBegins(const std::string& sourceId, long timeId, double step) override {}
};

#endif // SINK_ADAPTER_HPP
