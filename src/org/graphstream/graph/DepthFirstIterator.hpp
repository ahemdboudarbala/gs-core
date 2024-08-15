#ifndef DEPTH_FIRST_ITERATOR_HPP
#define DEPTH_FIRST_ITERATOR_HPP

#include "Node.hpp"
#include "Graph.hpp"
#include "Edge.hpp"
#include <iterator>
#include <vector>
#include <stdexcept>

class DepthFirstIterator : public std::iterator<std::input_iterator_tag, Node> {
public:
    // Constructor with directed option
    DepthFirstIterator(Node* startNode, bool directed = true);

    // Checks if there are more elements to iterate
    bool hasNext() const;

    // Returns the next element in the iteration
    Node* next();

    // Removes the current element (not supported)
    void remove();

    // Gets the depth of a specific node
    int getDepthOf(Node* node) const;

    // Gets the maximum depth reached during the iteration
    int getDepthMax() const;

    // Checks if a node is already visited (tabu)
    bool tabu(Node* node) const;

    // Checks if the iteration is directed
    bool isDirected() const;

private:
    // Moves to the next node in the iteration
    void gotoNext();

    bool directed;
    Graph* graph;
    std::vector<Node*> parent;
    std::vector<std::vector<Edge*>::iterator> iterator;
    std::vector<int> depth;
    Node* nextNode;
    int maxDepth;
};

#endif // DEPTH_FIRST_ITERATOR_HPP
