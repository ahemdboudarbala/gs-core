#ifndef ADJACENCY_LIST_NODE_HPP
#define ADJACENCY_LIST_NODE_HPP

#include "AbstractNode.hpp"
#include "AbstractEdge.hpp"
#include "AdjacencyListGraph.hpp"
#include <vector>
#include <memory>
#include <stdexcept>
#include <iterator>
#include <algorithm>

class AdjacencyListNode : public AbstractNode {
protected:
    static constexpr int INITIAL_EDGE_CAPACITY = 16;
    static constexpr double GROWTH_FACTOR = 1.1;

    static constexpr char I_EDGE = 0;
    static constexpr char IO_EDGE = 1;
    static constexpr char O_EDGE = 2;

    std::vector<std::shared_ptr<AbstractEdge>> edges;
    int ioStart, oStart, degree;

    // Constructor
    AdjacencyListNode(std::shared_ptr<AbstractGraph> graph, const std::string& id);

    // Helpers
    char edgeType(const std::shared_ptr<AbstractEdge>& e) const;
    
    template<typename T>
    std::shared_ptr<T> locateEdge(std::shared_ptr<Node> opposite, char type);

    void removeEdge(int i);

    // Callbacks
protected:
    bool addEdgeCallback(std::shared_ptr<AbstractEdge> edge) override;
    void removeEdgeCallback(std::shared_ptr<AbstractEdge> edge) override;
    void clearCallback() override;

    // Access methods
public:
    int getDegree() const override;
    int getInDegree() const override;
    int getOutDegree() const override;

    std::shared_ptr<Edge> getEdge(int i) const override;
    std::shared_ptr<Edge> getEnteringEdge(int i) const override;
    std::shared_ptr<Edge> getLeavingEdge(int i) const override;
    std::shared_ptr<Edge> getEdgeBetween(std::shared_ptr<Node> node) const override;
    std::shared_ptr<Edge> getEdgeFrom(std::shared_ptr<Node> node) const override;
    std::shared_ptr<Edge> getEdgeToward(std::shared_ptr<Node> node) const override;

    // Iterators
    std::vector<std::shared_ptr<Edge>> edges() const override;
    std::vector<std::shared_ptr<Edge>> enteringEdges() const override;
    std::vector<std::shared_ptr<Edge>> leavingEdges() const override;

    template<typename T>
    class EdgeIterator : public std::iterator<std::input_iterator_tag, T> {
    protected:
        int iPrev, iNext, iEnd;
        AdjacencyListNode& node;

        EdgeIterator(AdjacencyListNode& node, char type);

    public:
        bool hasNext();
        std::shared_ptr<T> next();
        void remove();
    };
};

#endif // ADJACENCY_LIST_NODE_HPP
