#ifndef GRAPHS_HPP
#define GRAPHS_HPP

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <mutex>
#include <iterator>
#include <stdexcept>
#include <iostream>
#include <type_traits>
#include "Graph.hpp" // Assuming you have Graph, Node, Edge classes
#include "MultiGraph.hpp"
#include "AdjacencyListGraph.hpp"
#include "GraphReplay.hpp"

class Graphs {
public:
    static std::shared_ptr<Graph> synchronizedGraph(std::shared_ptr<Graph> g);
    static std::shared_ptr<Graph> merge(const std::vector<std::shared_ptr<Graph>>& graphs);
    static void mergeIn(std::shared_ptr<Graph> result, const std::vector<std::shared_ptr<Graph>>& graphs);
    static std::shared_ptr<Graph> clone(std::shared_ptr<Graph> g);

    template<typename T>
    static void copyAttributes(const std::shared_ptr<T>& source, const std::shared_ptr<T>& target);

private:
    class SynchronizedElement;
    class SynchronizedGraph;
    class SynchronizedNode;
    class SynchronizedEdge;

    template<typename T>
    static T checkedArrayOrCollectionCopy(const T& o);
};

class Graphs::SynchronizedElement {
public:
    explicit SynchronizedElement(std::shared_ptr<Element> element);
    virtual ~SynchronizedElement() = default;

    void setAttribute(const std::string& attribute, const std::vector<std::string>& values);
    void clearAttributes();
    std::string getId() const;
    int getIndex() const;

protected:
    std::shared_ptr<Element> wrappedElement;
    static std::mutex attributeMutex;
};

class Graphs::SynchronizedGraph : public SynchronizedElement, public Graph {
public:
    explicit SynchronizedGraph(std::shared_ptr<Graph> g);
    
    std::shared_ptr<Node> addNode(const std::string& id) override;
    std::shared_ptr<Edge> addEdge(const std::string& id, const std::string& node1, const std::string& node2) override;
    std::shared_ptr<Edge> addEdge(const std::string& id, const std::string& from, const std::string& to, bool directed) override;
    std::shared_ptr<Node> getNode(const std::string& id) override;
    std::shared_ptr<Edge> getEdge(const std::string& id) override;

    // Other overridden methods from Graph

private:
    std::shared_ptr<Graph> graph;
    std::mutex elementMutex;
    std::map<std::string, std::shared_ptr<Node>> synchronizedNodes;
    std::map<std::string, std::shared_ptr<Edge>> synchronizedEdges;
};

class Graphs::SynchronizedNode : public SynchronizedElement, public Node {
public:
    SynchronizedNode(std::shared_ptr<SynchronizedGraph> sg, std::shared_ptr<Node> n);
    
    std::shared_ptr<Edge> getEdge(int i) override;
    std::shared_ptr<Edge> getEdgeBetween(const std::string& id) override;

    // Other overridden methods from Node

private:
    std::shared_ptr<SynchronizedGraph> sg;
};

class Graphs::SynchronizedEdge : public SynchronizedElement, public Edge {
public:
    SynchronizedEdge(std::shared_ptr<SynchronizedGraph> sg, std::shared_ptr<Edge> e);

    std::shared_ptr<Node> getNode0() override;
    std::shared_ptr<Node> getNode1() override;
    std::shared_ptr<Node> getOpposite(std::shared_ptr<Node> node) override;

    // Other overridden methods from Edge

private:
    std::shared_ptr<SynchronizedGraph> sg;
};

#endif // GRAPHS_HPP
