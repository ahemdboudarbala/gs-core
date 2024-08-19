#ifndef ABSTRACT_GRAPH_HPP
#define ABSTRACT_GRAPH_HPP

#include "AbstractElement.hpp"
#include "graph/Node.hpp"
#include "graph/Edge.hpp"
#include "graph/NodeFactory.hpp"
#include "graph/EdgeFactory.hpp"
#include "util/GraphListeners.hpp"
#include "stream/Replayable.hpp"
#include "stream/SourceBase.hpp"
#include <memory>
#include <iterator>
#include <string>
#include <stdexcept>
#include <vector>

class AbstractGraph : public AbstractElement, public Graph, public Replayable {
protected:
    // Fields
    GraphListeners listeners;
    bool strictChecking;
    bool autoCreate;
    std::shared_ptr<NodeFactory<AbstractNode>> nodeFactory;
    std::shared_ptr<EdgeFactory<AbstractEdge>> edgeFactory;
    double step;
    long replayId;

public:
    // Constructors
    AbstractGraph(const std::string& id);
    AbstractGraph(const std::string& id, bool strictChecking, bool autoCreate);

    // Inherited from AbstractElement
    virtual void attributeChanged(AttributeChangeEvent event, const std::string& attribute,
                                  std::shared_ptr<void> oldValue, std::shared_ptr<void> newValue) override;

    // Inherited from Graph
    virtual std::shared_ptr<NodeFactory<Node>> nodeFactory() override;
    virtual std::shared_ptr<EdgeFactory<Edge>> edgeFactory() override;
    virtual void setNodeFactory(std::shared_ptr<NodeFactory<Node>> nf) override;
    virtual void setEdgeFactory(std::shared_ptr<EdgeFactory<Edge>> ef) override;

    virtual bool isStrict() const override;
    virtual void setStrict(bool on) override;
    virtual bool isAutoCreationEnabled() const override;
    virtual void setAutoCreate(bool on) override;
    virtual double getStep() const override;
    virtual void stepBegins(double time) override;

    virtual void clear() override;
    virtual std::shared_ptr<Node> addNode(const std::string& id) override;
    virtual std::shared_ptr<Edge> addEdge(const std::string& id, std::shared_ptr<Node> from, 
                                          std::shared_ptr<Node> to, bool directed) override;
    virtual std::shared_ptr<Node> removeNode(std::shared_ptr<Node> node) override;
    virtual std::shared_ptr<Edge> removeEdge(std::shared_ptr<Edge> edge) override;
    virtual std::shared_ptr<Edge> removeEdge(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) override;

    // Sinks and sources
    virtual std::vector<std::shared_ptr<AttributeSink>> attributeSinks() override;
    virtual std::vector<std::shared_ptr<ElementSink>> elementSinks() override;
    virtual void addAttributeSink(std::shared_ptr<AttributeSink> sink) override;
    virtual void addElementSink(std::shared_ptr<ElementSink> sink) override;
    virtual void addSink(std::shared_ptr<Sink> sink) override;
    virtual void clearAttributeSinks() override;
    virtual void clearElementSinks() override;
    virtual void clearSinks() override;
    virtual void removeAttributeSink(std::shared_ptr<AttributeSink> sink) override;
    virtual void removeElementSink(std::shared_ptr<ElementSink> sink) override;
    virtual void removeSink(std::shared_ptr<Sink> sink) override;

    // Replayable interface
    virtual std::unique_ptr<Replayable::Controller> getReplayController() override;

protected:
    // Callbacks to be implemented by subclasses
    virtual void addNodeCallback(std::shared_ptr<AbstractNode> node) = 0;
    virtual void addEdgeCallback(std::shared_ptr<AbstractEdge> edge) = 0;
    virtual void removeNodeCallback(std::shared_ptr<AbstractNode> node) = 0;
    virtual void removeEdgeCallback(std::shared_ptr<AbstractEdge> edge) = 0;
    virtual void clearCallback() = 0;

    std::shared_ptr<Edge> addEdge(const std::string& edgeId, std::shared_ptr<AbstractNode> src, 
                                  const std::string& srcId, std::shared_ptr<AbstractNode> dst, 
                                  const std::string& dstId, bool directed);

    void removeNode(std::shared_ptr<AbstractNode> node, bool graphCallback);
    void removeEdge(std::shared_ptr<AbstractEdge> edge, bool graphCallback, 
                    bool sourceCallback, bool targetCallback);
    void removeAllEdges(std::shared_ptr<AbstractNode> node);

private:
    class GraphReplayController : public SourceBase, public Replayable::Controller {
    public:
        GraphReplayController(AbstractGraph* graph);
        void replay() override;
        void replay(const std::string& sourceId) override;
    };
};

#endif // ABSTRACT_GRAPH_HPP
