#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <memory>
#include <vector>
#include <iterator>
#include <stdexcept>
#include "Element.hpp"
#include "Edge.hpp"
#include "Graph.hpp"

class Node : public Element, public std::enable_shared_from_this<Node> {
public:
    virtual ~Node() = default;

    // Accessor methods
    virtual std::shared_ptr<Graph> getGraph() const = 0;
    virtual int getDegree() const = 0;
    virtual int getOutDegree() const = 0;
    virtual int getInDegree() const = 0;

    // Methods to retrieve edges
    virtual std::shared_ptr<Edge> getEdgeToward(const std::string& id) const = 0;
    virtual std::shared_ptr<Edge> getEdgeFrom(const std::string& id) const = 0;
    virtual std::shared_ptr<Edge> getEdgeBetween(const std::string& id) const = 0;

    // Stream over neighbor nodes connected via one or more edges
    virtual std::vector<std::shared_ptr<Node>> neighborNodes() const = 0;

    // Methods to retrieve edges by index
    virtual std::shared_ptr<Edge> getEdge(int i) const = 0;
    virtual std::shared_ptr<Edge> getEnteringEdge(int i) const = 0;
    virtual std::shared_ptr<Edge> getLeavingEdge(int i) const = 0;

    // Iterators for graph exploration
    virtual std::vector<std::shared_ptr<Node>> getBreadthFirstIterator() const = 0;
    virtual std::vector<std::shared_ptr<Node>> getBreadthFirstIterator(bool directed) const = 0;
    virtual std::vector<std::shared_ptr<Node>> getDepthFirstIterator() const = 0;
    virtual std::vector<std::shared_ptr<Node>> getDepthFirstIterator(bool directed) const = 0;

    // Stream over all edges
    virtual std::vector<std::shared_ptr<Edge>> edges() const = 0;

    // Stream over leaving and entering edges
    virtual std::vector<std::shared_ptr<Edge>> leavingEdges() const = 0;
    virtual std::vector<std::shared_ptr<Edge>> enteringEdges() const = 0;

    // Override the toString method
    virtual std::string toString() const = 0;

    // New methods
    virtual bool hasEdgeToward(const std::string& id) const = 0;
    virtual bool hasEdgeToward(std::shared_ptr<Node> node) const = 0;
    virtual bool hasEdgeToward(int index) const = 0;

    virtual bool hasEdgeFrom(const std::string& id) const = 0;
    virtual bool hasEdgeFrom(std::shared_ptr<Node> node) const = 0;
    virtual bool hasEdgeFrom(int index) const = 0;

    virtual bool hasEdgeBetween(const std::string& id) const = 0;
    virtual bool hasEdgeBetween(std::shared_ptr<Node> node) const = 0;
    virtual bool hasEdgeBetween(int index) const = 0;

    virtual std::shared_ptr<Edge> getEdgeToward(std::shared_ptr<Node> node) const = 0;
    virtual std::shared_ptr<Edge> getEdgeToward(int index) const = 0;

    virtual std::shared_ptr<Edge> getEdgeFrom(std::shared_ptr<Node> node) const = 0;
    virtual std::shared_ptr<Edge> getEdgeFrom(int index) const = 0;

    virtual std::shared_ptr<Edge> getEdgeBetween(std::shared_ptr<Node> node) const = 0;
    virtual std::shared_ptr<Edge> getEdgeBetween(int index) const = 0;
};

#endif // NODE_HPP
