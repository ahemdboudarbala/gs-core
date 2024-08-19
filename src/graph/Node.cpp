#include "Node.hpp"
#include "Edge.hpp"
#include "Graph.hpp"

class NodeImpl : public Node {
private:
    std::shared_ptr<Graph> graph;
    std::string id;
    std::vector<std::shared_ptr<Edge>> edgesList;

public:
    NodeImpl(const std::shared_ptr<Graph>& graph, const std::string& id)
        : graph(graph), id(id) {}

    std::shared_ptr<Graph> getGraph() const override {
        return graph;
    }

    int getDegree() const override {
        return edgesList.size();
    }

    int getOutDegree() const override {
        int count = 0;
        for (const auto& edge : edgesList) {
            if (edge->getSourceNode() == shared_from_this()) {
                count++;
            }
        }
        return count;
    }

    int getInDegree() const override {
        int count = 0;
        for (const auto& edge : edgesList) {
            if (edge->getTargetNode() == shared_from_this()) {
                count++;
            }
        }
        return count;
    }

    std::shared_ptr<Edge> getEdgeToward(const std::string& targetId) const override {
        for (const auto& edge : edgesList) {
            if (edge->getTargetNode()->getId() == targetId) {
                return edge;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Edge> getEdgeFrom(const std::string& sourceId) const override {
        for (const auto& edge : edgesList) {
            if (edge->getSourceNode()->getId() == sourceId) {
                return edge;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Edge> getEdgeBetween(const std::string& otherId) const override {
        for (const auto& edge : edgesList) {
            if (edge->getSourceNode()->getId() == otherId || edge->getTargetNode()->getId() == otherId) {
                return edge;
            }
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<Node>> neighborNodes() const override {
        std::vector<std::shared_ptr<Node>> neighbors;
        for (const auto& edge : edgesList) {
            neighbors.push_back(edge->getOpposite(shared_from_this()));
        }
        return neighbors;
    }

    std::shared_ptr<Edge> getEdge(int i) const override {
        if (i < 0 || i >= getDegree()) {
            throw std::out_of_range("Edge index out of range");
        }
        return edgesList[i];
    }

    std::shared_ptr<Edge> getEnteringEdge(int i) const override {
        int count = 0;
        for (const auto& edge : edgesList) {
            if (edge->getTargetNode() == shared_from_this()) {
                if (count == i) return edge;
                count++;
            }
        }
        throw std::out_of_range("Entering edge index out of range");
    }

    std::shared_ptr<Edge> getLeavingEdge(int i) const override {
        int count = 0;
        for (const auto& edge : edgesList) {
            if (edge->getSourceNode() == shared_from_this()) {
                if (count == i) return edge;
                count++;
            }
        }
        throw std::out_of_range("Leaving edge index out of range");
    }

    std::vector<std::shared_ptr<Node>> getBreadthFirstIterator() const override {
        // Implement a BFS algorithm here
        // This is a placeholder for the BFS iterator
        return {};
    }

    std::vector<std::shared_ptr<Node>> getBreadthFirstIterator(bool directed) const override {
        // Implement a BFS algorithm here with direction consideration
        // This is a placeholder for the BFS iterator
        return {};
    }

    std::vector<std::shared_ptr<Node>> getDepthFirstIterator() const override {
        // Implement a DFS algorithm here
        // This is a placeholder for the DFS iterator
        return {};
    }

    std::vector<std::shared_ptr<Node>> getDepthFirstIterator(bool directed) const override {
        // Implement a DFS algorithm here with direction consideration
        // This is a placeholder for the DFS iterator
        return {};
    }

    std::vector<std::shared_ptr<Edge>> edges() const override {
        return edgesList;
    }

    std::vector<std::shared_ptr<Edge>> leavingEdges() const override {
        std::vector<std::shared_ptr<Edge>> leaving;
        for (const auto& edge : edgesList) {
            if (edge->getSourceNode() == shared_from_this()) {
                leaving.push_back(edge);
            }
        }
        return leaving;
    }

    std::vector<std::shared_ptr<Edge>> enteringEdges() const override {
        std::vector<std::shared_ptr<Edge>> entering;
        for (const auto& edge : edgesList) {
            if (edge->getTargetNode() == shared_from_this()) {
                entering.push_back(edge);
            }
        }
        return entering;
    }

    std::string toString() const override {
        return "Node(" + id + ")";
    }

    bool hasEdgeToward(const std::string& targetId) const override {
        return getEdgeToward(targetId) != nullptr;
    }

    bool hasEdgeToward(std::shared_ptr<Node> node) const override {
        return hasEdgeToward(node->getId());
    }

    bool hasEdgeToward(int index) const override {
        auto node = getGraph()->getNode(index);
        return node && hasEdgeToward(node);
    }

    bool hasEdgeFrom(const std::string& sourceId) const override {
        return getEdgeFrom(sourceId) != nullptr;
    }

    bool hasEdgeFrom(std::shared_ptr<Node> node) const override {
        return hasEdgeFrom(node->getId());
    }

    bool hasEdgeFrom(int index) const override {
        auto node = getGraph()->getNode(index);
        return node && hasEdgeFrom(node);
    }

    bool hasEdgeBetween(const std::string& otherId) const override {
        return getEdgeBetween(otherId) != nullptr;
    }

    bool hasEdgeBetween(std::shared_ptr<Node> node) const override {
        return hasEdgeBetween(node->getId());
    }

    bool hasEdgeBetween(int index) const override {
        auto node = getGraph()->getNode(index);
        return node && hasEdgeBetween(node);
    }

    std::shared_ptr<Edge> getEdgeToward(std::shared_ptr<Node> node) const override {
        return getEdgeToward(node->getId());
    }

    std::shared_ptr<Edge> getEdgeToward(int index) const override {
        auto node = getGraph()->getNode(index);
        if (!node) throw std::out_of_range("Node index out of range");
        return getEdgeToward(node);
    }

    std::shared_ptr<Edge> getEdgeFrom(std::shared_ptr<Node> node) const override {
        return getEdgeFrom(node->getId());
    }

    std::shared_ptr<Edge> getEdgeFrom(int index) const override {
        auto node = getGraph()->getNode(index);
        if (!node) throw std::out_of_range("Node index out of range");
        return getEdgeFrom(node);
    }

    std::shared_ptr<Edge> getEdgeBetween(std::shared_ptr<Node> node) const override {
        return getEdgeBetween(node->getId());
    }

    std::shared_ptr<Edge> getEdgeBetween(int index) const override {
        auto node = getGraph()->getNode(index);
        if (!node) throw std::out_of_range("Node index out of range");
        return getEdgeBetween(node);
    }

    void addEdge(const std::shared_ptr<Edge>& edge) {
        edgesList.push_back(edge);
    }

    std::string getId() const override {
        return id;
    }
};

