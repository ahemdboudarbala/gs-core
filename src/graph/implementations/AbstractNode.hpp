#ifndef ABSTRACT_NODE_HPP
#define ABSTRACT_NODE_HPP

#include "AbstractElement.hpp"
#include "graph/Node.hpp"
#include "graph/Edge.hpp"
#include "AbstractGraph.hpp"
#include "graph/BreadthFirstIterator.hpp"
#include "graph/DepthFirstIterator.hpp"
#include <string>
#include <memory>
#include <iterator>

class AbstractNode : public AbstractElement, public Node {
protected:
    // The graph to which this node belongs
    std::shared_ptr<AbstractGraph> graph;

public:
    // Constructor
    AbstractNode(std::shared_ptr<AbstractGraph> graph, const std::string& id);

    // Inherited from AbstractElement
    virtual void attributeChanged(AttributeChangeEvent event, const std::string& attribute,
                                  std::shared_ptr<void> oldValue, std::shared_ptr<void> newValue) override;

    // Inherited from Node
    std::shared_ptr<Graph> getGraph() const override;

    std::shared_ptr<Edge> getEdgeToward(int index) override;
    std::shared_ptr<Edge> getEdgeToward(const std::string& id) override;
    std::shared_ptr<Edge> getEdgeFrom(int index) override;
    std::shared_ptr<Edge> getEdgeFrom(const std::string& id) override;
    std::shared_ptr<Edge> getEdgeBetween(int index) override;
    std::shared_ptr<Edge> getEdgeBetween(const std::string& id) override;

    std::unique_ptr<std::iterator<std::input_iterator_tag, Node>> getBreadthFirstIterator() override;
    std::unique_ptr<std::iterator<std::input_iterator_tag, Node>> getBreadthFirstIterator(bool directed) override;
    std::unique_ptr<std::iterator<std::input_iterator_tag, Node>> getDepthFirstIterator() override;
    std::unique_ptr<std::iterator<std::input_iterator_tag, Node>> getDepthFirstIterator(bool directed) override;

    // Other methods
    virtual bool addEdgeCallback(std::shared_ptr<AbstractEdge> edge) = 0;
    virtual void removeEdgeCallback(std::shared_ptr<AbstractEdge> edge) = 0;
    virtual void clearCallback() = 0;

    bool isEnteringEdge(std::shared_ptr<Edge> e) const;
    bool isLeavingEdge(std::shared_ptr<Edge> e) const;
    bool isIncidentEdge(std::shared_ptr<Edge> e) const;
};

#endif // ABSTRACT_NODE_HPP
