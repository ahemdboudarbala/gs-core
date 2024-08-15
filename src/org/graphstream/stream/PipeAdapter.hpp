#ifndef PIPE_ADAPTER_HPP
#define PIPE_ADAPTER_HPP

#include "Pipe.hpp"
#include <string>
#include <any>

/**
 * Adapter for the Pipe interface.
 * 
 * All methods are empty, serving as a base class for other implementations that only need to override some of the methods.
 */
class PipeAdapter : public Pipe {
public:
    virtual void addAttributeSink(AttributeSink* listener) override;
    virtual void addElementSink(ElementSink* listener) override;
    virtual void addSink(Sink* listener) override;

    virtual void removeAttributeSink(AttributeSink* listener) override;
    virtual void removeElementSink(ElementSink* listener) override;
    virtual void removeSink(Sink* listener) override;

    virtual void edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& value) override;
    virtual void edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;
    virtual void edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute) override;

    virtual void graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, const std::any& value) override;
    virtual void graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;
    virtual void graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) override;

    virtual void nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& value) override;
    virtual void nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;
    virtual void nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute) override;

    virtual void edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) override;
    virtual void edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) override;

    virtual void graphCleared(const std::string& sourceId, long timeId) override;

    virtual void nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) override;
    virtual void nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) override;

    virtual void stepBegins(const std::string& sourceId, long timeId, double step) override;

    virtual void clearAttributeSinks() override;
    virtual void clearElementSinks() override;
    virtual void clearSinks() override;
};

#endif // PIPE_ADAPTER_HPP
