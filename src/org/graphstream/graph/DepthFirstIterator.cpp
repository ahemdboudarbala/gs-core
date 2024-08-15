#include "DepthFirstIterator.hpp"

DepthFirstIterator::DepthFirstIterator(Node* startNode, bool directed)
    : directed(directed), graph(startNode->getGraph()), nextNode(startNode), maxDepth(0) {
    int n = graph->getNodeCount();
    parent.resize(n, nullptr);
    iterator.resize(n);
    depth.resize(n, -1);

    int s = startNode->getIndex();
    depth[s] = 0;
}

void DepthFirstIterator::gotoNext() {
    while (nextNode != nullptr) {
        int i = nextNode->getIndex();
        while (iterator[i] != graph->getEdgesEnd()) {
            Node* neighbor = (*iterator[i]++)->getOpposite(nextNode);
            int j = neighbor->getIndex();
            if (iterator[j] == graph->getEdgesEnd()) {
                parent[j] = nextNode;
                iterator[j] = directed ? neighbor->leavingEdgesBegin() : neighbor->enteringEdgesBegin();
                depth[j] = depth[i] + 1;
                if (depth[j] > maxDepth) {
                    maxDepth = depth[j];
                }
                nextNode = neighbor;
                return;
            }
        }
        nextNode = parent[i];
    }
}

bool DepthFirstIterator::hasNext() const {
    return nextNode != nullptr;
}

Node* DepthFirstIterator::next() {
    if (nextNode == nullptr) {
        throw std::out_of_range("No more elements");
    }
    int index = nextNode->getIndex();
    iterator[index] = directed ? nextNode->leavingEdgesBegin() : nextNode->enteringEdgesBegin();
    Node* previous = nextNode;
    gotoNext();
    return previous;
}

void DepthFirstIterator::remove() {
    throw std::logic_error("This iterator does not support remove");
}

int DepthFirstIterator::getDepthOf(Node* node) const {
    return depth[node->getIndex()];
}

int DepthFirstIterator::getDepthMax() const {
    return maxDepth;
}

bool DepthFirstIterator::tabu(Node* node) const {
    return depth[node->getIndex()] != -1;
}

bool DepthFirstIterator::isDirected() const {
    return directed;
}
