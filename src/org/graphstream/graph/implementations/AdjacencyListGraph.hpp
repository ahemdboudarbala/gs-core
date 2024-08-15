#ifndef ADJACENCY_LIST_GRAPH_HPP
#define ADJACENCY_LIST_GRAPH_HPP

#include "AbstractGraph.hpp"
#include "AdjacencyListNode.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include <iterator>
#include <stdexcept>
#include <algorithm>

class AdjacencyListGraph : public AbstractGraph {
public:
    static constexpr double GROW_FACTOR = 1.1;
    static constexpr int DEFAULT_NODE_CAPACITY = 128;
    static constexpr int DEFAULT_EDGE_CAPACITY = 1024;

    AdjacencyListGraph(const std::string& id, bool strictChecking, bool autoCreate, 
                       int initialNodeCapacity = DEFAULT_NODE_CAPACITY, 
                       int initialEdgeCapacity = DEFAULT_EDGE_CAPACITY);

    AdjacencyListGraph(const std::string& id, bool strictChecking, bool autoCreate);

    AdjacencyListGraph(const std::string& id);

    // Callbacks
protected:
    void addEdgeCallback(std::shared_ptr<AbstractEdge> edge) override;
    void addNodeCallback(std::shared_ptr<AbstractNode> node) override;
    void removeEdgeCallback(std::shared_ptr<AbstractEdge> edge) override;
    void removeNodeCallback(std::shared_ptr<AbstractNode> node) override;
    void clearCallback() override;

    // Streams and Counts
public:
    std::vector<std::shared_ptr<Node>> nodes() const override;
    std::vector<std::shared_ptr<Edge>> edges() const override;

    std::shared_ptr<Edge> getEdge(const std::string& id) const override;
    std::shared_ptr<Edge> getEdge(int index) const override;
    int getEdgeCount() const override;

    std::shared_ptr<Node> getNode(const std::string& id) const override;
    std::shared_ptr<Node> getNode(int index) const override;
    int getNodeCount() const override;

    // Iterators
protected:
    template<typename T>
    class EdgeIterator : public std::iterator<std::input_iterator_tag, T> {
    public:
        EdgeIterator(AdjacencyListGraph& graph);

        bool hasNext();
        T next();
        void remove();

    private:
        AdjacencyListGraph& graph;
        int iNext;
        int iPrev;
    };

    template<typename T>
    class NodeIterator : public std::iterator<std::input_iterator_tag, T> {
    public:
        NodeIterator(AdjacencyListGraph& graph);

        bool hasNext();
        T next();
        void remove();

    private:
        AdjacencyListGraph& graph;
        int iNext;
        int iPrev;
    };

private:
    std::unordered_map<std::string, std::shared_ptr<AbstractNode>> nodeMap;
    std::unordered_map<std::string, std::shared_ptr<AbstractEdge>> edgeMap;

    std::vector<std::shared_ptr<AbstractNode>> nodeArray;
    std::vector<std::shared_ptr<AbstractEdge>> edgeArray;

    int nodeCount;
    int edgeCount;
};

#endif // ADJACENCY_LIST_GRAPH_HPP
