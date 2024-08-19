#include "BreadthFirstIterator.hpp"

// Constructor
BreadthFirstIterator::BreadthFirstIterator(std::shared_ptr<Node> startNode, bool directed)
    : directed(directed), graph(startNode->getGraph()), qHead(0), qTail(1) {
    int n = graph->getNodeCount();
    queue.resize(n);
    depth.resize(n, -1);

    int s = startNode->getIndex();
    depth[s] = 0;
    queue[0] = startNode;
}

// Check if there are more nodes to iterate
bool BreadthFirstIterator::hasNext() const {
    return qHead < qTail;
}

// Get the next node in the iteration
std::shared_ptr<Node> BreadthFirstIterator::next() {
    if (!hasNext()) {
        throw std::out_of_range("No more elements in the iterator");
    }

    std::shared_ptr<Node> current = queue[qHead++];
    int level = depth[current->getIndex()] + 1;

    std::vector<std::shared_ptr<Edge>> edges = directed ? current->leavingEdges() : current->edges();

    for (auto& e : edges) {
        std::shared_ptr<Node> node = e->getOpposite(current);
        int j = node->getIndex();

        if (depth[j] == -1) {
            queue[qTail++] = node;
            depth[j] = level;
        }
    }

    return current;
}

// Get the depth of a specific node
int BreadthFirstIterator::getDepthOf(std::shared_ptr<Node> node) const {
    return depth[node->getIndex()];
}

// Get the maximum depth reached during the iteration
int BreadthFirstIterator::getDepthMax() const {
    return depth[queue[qTail - 1]->getIndex()];
}

// Check if a node is tabu (already visited)
bool BreadthFirstIterator::tabu(std::shared_ptr<Node> node) const {
    return depth[node->getIndex()] != -1;
}

// Check if the iteration is directed
bool BreadthFirstIterator::isDirected() const {
    return directed;
}
