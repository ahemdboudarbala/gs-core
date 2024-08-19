#ifndef PIPE_BASE_HPP
#define PIPE_BASE_HPP

#include "SourceBase.hpp"
#include "Pipe.hpp"

/**
 * A base pipe that lets all events pass.
 * 
 * This pipe does nothing and lets all events pass. It can be used as a base to
 * implement more specific filters by refining some of its methods.
 * 
 * Another use of this pipe is to duplicate a stream of events from one input
 * toward several outputs.
 */
class PipeBase : public SourceBase, public Pipe {
public:
    virtual void edgeAttributeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& value) override;
    virtual void edgeAttributeChanged(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;
    virtual void edgeAttributeRemoved(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute) override;

    virtual void graphAttributeAdded(const std::string& graphId, long timeId, const std::string& attribute, const std::any& value) override;
    virtual void graphAttributeChanged(const std::string& graphId, long timeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;
    virtual void graphAttributeRemoved(const std::string& graphId, long timeId, const std::string& attribute) override;

    virtual void nodeAttributeAdded(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& value) override;
    virtual void nodeAttributeChanged(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;
    virtual void nodeAttributeRemoved(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute) override;

    virtual void edgeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) override;
    virtual void edgeRemoved(const std::string& graphId, long timeId, const std::string& edgeId) override;

    virtual void graphCleared(const std::string& graphId, long timeId) override;

    virtual void nodeAdded(const std::string& graphId, long timeId, const std::string& nodeId) override;
    virtual void nodeRemoved(const std::string& graphId, long timeId, const std::string& nodeId) override;

    virtual void stepBegins(const std::string& graphId, long timeId, double step) override;
};

#endif // PIPE_BASE_HPP
