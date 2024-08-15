#ifndef PATH_HPP
#define PATH_HPP

#include <stack>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "Node.hpp"
#include "Edge.hpp"
#include "Structure.hpp"

class Path : public Structure {
public:
    Path() = default;

    std::shared_ptr<Node> getRoot() const {
        return root;
    }

    void setRoot(const std::shared_ptr<Node>& rootNode) {
        if (!root) {
            root = rootNode;
            nodePath.push(rootNode);
        } else {
            throw std::runtime_error("Root node is not null - first use the clear method.");
        }
    }

    bool contains(const std::shared_ptr<Node>& node) const {
        return std::find(nodePath.begin(), nodePath.end(), node) != nodePath.end();
    }

    bool contains(const std::shared_ptr<Edge>& edge) const {
        return std::find(edgePath.begin(), edgePath.end(), edge) != edgePath.end();
    }

    bool empty() const {
        return nodePath.empty();
    }

    int size() const {
        return static_cast<int>(nodePath.size());
    }

    double getPathWeight(const std::string& characteristic) const {
        double weight = 0;
        for (const auto& edge : edgePath) {
            weight += edge->getAttribute<double>(characteristic);
        }
        return weight;
    }

    std::vector<std::shared_ptr<Edge>> getEdgePath() const {
        return std::vector<std::shared_ptr<Edge>>(edgePath.begin(), edgePath.end());
    }

    std::vector<std::shared_ptr<Node>> getNodePath() const {
        return std::vector<std::shared_ptr<Node>>(nodePath.begin(), nodePath.end());
    }

    void add(const std::shared_ptr<Node>& from, const std::shared_ptr<Edge>& edge) {
        if (!root) {
            if (!from) {
                throw std::invalid_argument("From node cannot be null.");
            } else {
                setRoot(from);
            }
        }

        auto actualFrom = from ? from : nodePath.top();

        if (nodePath.top() != actualFrom) {
            throw std::invalid_argument("From node must be at the head of the path");
        }

        if (edge->getSourceNode() != actualFrom && edge->getTargetNode() != actualFrom) {
            throw std::invalid_argument("From node must be part of the edge");
        }

        nodePath.push(edge->getOpposite(actualFrom));
        edgePath.push(edge);
    }

    void add(const std::shared_ptr<Edge>& edge) {
        if (nodePath.empty()) {
            add(nullptr, edge);
        } else {
            add(nodePath.top(), edge);
        }
    }

    void push(const std::shared_ptr<Node>& from, const std::shared_ptr<Edge>& edge) {
        add(from, edge);
    }

    void push(const std::shared_ptr<Edge>& edge) {
        add(edge);
    }

    std::shared_ptr<Edge> popEdge() {
        nodePath.pop();
        auto edge = edgePath.top();
        edgePath.pop();
        return edge;
    }

    std::shared_ptr<Node> popNode() {
        edgePath.pop();
        auto node = nodePath.top();
        nodePath.pop();
        return node;
    }

    std::shared_ptr<Node> peekNode() const {
        return nodePath.top();
    }

    std::shared_ptr<Edge> peekEdge() const {
        return edgePath.top();
    }

    void clear() {
        while (!nodePath.empty()) nodePath.pop();
        while (!edgePath.empty()) edgePath.pop();
        root.reset();
    }

    std::shared_ptr<Path> getACopy() const {
        auto newPath = std::make_shared<Path>();
        newPath->root = this->root;
        newPath->nodePath = this->nodePath;
        newPath->edgePath = this->edgePath;
        return newPath;
    }

    void removeLoops() {
        for (auto it1 = nodePath.begin(); it1 != nodePath.end(); ++it1) {
            auto it2 = std::find(it1 + 1, nodePath.end(), *it1);
            if (it2 != nodePath.end()) {
                nodePath.erase(it1 + 1, it2 + 1);
                edgePath.erase(edgePath.begin() + std::distance(nodePath.begin(), it1), 
                               edgePath.begin() + std::distance(nodePath.begin(), it2));
            }
        }
    }

    bool equals(const std::shared_ptr<Path>& p) const {
        return nodePath == p->nodePath;
    }

    std::string toString() const {
        std::string result = "[";
        for (const auto& node : nodePath) {
            result += node->toString() + ", ";
        }
        if (!result.empty()) {
            result.pop_back();
            result.pop_back();
        }
        result += "]";
        return result;
    }

    int getNodeCount() const override {
        return size();
    }

    int getEdgeCount() const override {
        return static_cast<int>(edgePath.size());
    }

    std::vector<std::shared_ptr<Node>> nodes() const override {
        return getNodePath();
    }

    std::vector<std::shared_ptr<Edge>> edges() const override {
        return getEdgePath();
    }

    template <typename T>
    std::vector<std::shared_ptr<T>> getNodeSet() const {
        return std::vector<std::shared_ptr<T>>(nodePath.begin(), nodePath.end());
    }

    template <typename T>
    std::vector<std::shared_ptr<T>> getEdgeSet() const {
        return std::vector<std::shared_ptr<T>>(edgePath.begin(), edgePath.end());
    }

private:
    std::shared_ptr<Node> root = nullptr;
    std::stack<std::shared_ptr<Edge>> edgePath;
    std::stack<std::shared_ptr<Node>> nodePath;
};

#endif // PATH_HPP
