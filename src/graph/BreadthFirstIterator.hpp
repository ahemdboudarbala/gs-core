#ifndef BREADTH_FIRST_ITERATOR_HPP
#define BREADTH_FIRST_ITERATOR_HPP

#include "Node.hpp"
#include "Edge.hpp"
#include "Graph.hpp"
#include <vector>
#include <queue>
#include <stdexcept>

class BreadthFirstIterator {
public:
    // Constructors
    BreadthFirstIterator(std::shared_ptr<Node> startNode, bool directed = true);

    // Iterator methods
    bool hasNext() const;
    std::shared_ptr<Node> next();

    // Other methods
    int getDepthOf(std::shared_ptr<Node> node) const;
    int getDepthMax() const;
    bool tabu(std::shared_ptr<Node> node) const;
    bool isDirected() const;

private:
    bool directed;
    std::shared_ptr<Graph> graph;
    std::vector<std::shared_ptr<Node>> queue;
    std::vector<int> depth;
    size_t qHead, qTail;
};

#endif // BREADTH_FIRST_ITERATOR_HPP
