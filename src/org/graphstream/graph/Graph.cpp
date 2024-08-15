#include "Graph.hpp"
#include <iostream>

class SimpleGraph : public Graph {
private:
    std::map<std::string, std::shared_ptr<Node>> nodes;
    std::map<std::string, std::shared_ptr<Edge>> edges;
    std::shared_ptr<NodeFactory<Node>> node_factory;
    std::shared_ptr<EdgeFactory<Edge>> edge_factory;
    bool strict;
    bool auto_create;
    double step;
    std::vector<std::shared_ptr<AttributeSink>> attribute_sinks;
    std::vector<std::shared_ptr<ElementSink>> element_sinks;

public:
    SimpleGraph()
        : strict(true), auto_create(false), step(0.0) {}

    ~SimpleGraph() override = default;

    std::shared_ptr<Node> getNode(const std::string& id) override {
        if (nodes.find(id) != nodes.end())
            return nodes[id];
        if (strict)
            throw ElementNotFoundException("Node '" + id + "' not found.");
        return nullptr;
    }

    std::shared_ptr<Edge> getEdge(const std::string& id) override {
        if (edges.find(id) != edges.end())
            return edges[id];
        if (strict)
            throw ElementNotFoundException("Edge '" + id + "' not found.");
        return nullptr;
    }

    std::shared_ptr<NodeFactory<Node>> nodeFactory() override {
        return node_factory;
    }

    std::shared_ptr<EdgeFactory<Edge>> edgeFactory() override {
        return edge_factory;
    }

    bool isStrict() const override {
        return strict;
    }

    bool isAutoCreationEnabled() const override {
        return auto_create;
    }

    double getStep() const override {
        return step;
    }

    void setNodeFactory(std::shared_ptr<NodeFactory<Node>> nf) override {
        node_factory = nf;
    }

    void setEdgeFactory(std::shared_ptr<EdgeFactory<Edge>> ef) override {
        edge_factory = ef;
    }

    void setStrict(bool on) override {
        strict = on;
    }

    void setAutoCreate(bool on) override {
        auto_create = on;
    }

    void clear() override {
        nodes.clear();
        edges.clear();
        attribute_sinks.clear();
        element_sinks.clear();
    }

    std::shared_ptr<Node> addNode(const std::string& id) override {
        if (nodes.find(id) != nodes.end()) {
            if (strict)
                throw IdAlreadyInUseException("Node id '" + id + "' already in use.");
            return nodes[id];
        }
        auto node = node_factory->newInstance(id);
        nodes[id] = node;
        return node;
    }

    std::shared_ptr<Edge> addEdge(const std::string& id, const std::string& node1, const std::string& node2) override {
        return addEdge(id, node1, node2, false);
    }

    std::shared_ptr<Edge> addEdge(const std::string& id, const std::string& from, const std::string& to, bool directed) override {
        if (edges.find(id) != edges.end()) {
            if (strict)
                throw IdAlreadyInUseException("Edge id '" + id + "' already in use.");
            return edges[id];
        }
        auto src_node = getNode(from);
        auto dst_node = getNode(to);

        if (!src_node || !dst_node) {
            if (strict)
                throw ElementNotFoundException("One of the nodes does not exist.");
            if (auto_create) {
                if (!src_node) src_node = addNode(from);
                if (!dst_node) dst_node = addNode(to);
            } else {
                return nullptr;
            }
        }

        auto edge = edge_factory->newInstance(id, src_node, dst_node, directed);
        edges[id] = edge;
        return edge;
    }

    void stepBegins(double time) override {
        step = time;
    }

    std::vector<std::shared_ptr<AttributeSink>> attributeSinks() override {
        return attribute_sinks;
    }

    std::vector<std::shared_ptr<ElementSink>> elementSinks() override {
        return element_sinks;
    }

    void read(const std::string& filename) override {
        std::cerr << "Read method not implemented." << std::endl;
    }

    void read(std::shared_ptr<FileSource> input, const std::string& filename) override {
        std::cerr << "Read from source not implemented." << std::endl;
    }

    void write(const std::string& filename) override {
        std::cerr << "Write method not implemented." << std::endl;
    }

    void write(std::shared_ptr<FileSink> output, const std::string& filename) override {
        std::cerr << "Write to sink not implemented." << std::endl;
    }

    std::shared_ptr<Viewer> display() override {
        return display(true);
    }

    std::shared_ptr<Viewer> display(bool autoLayout) override {
        std::cerr << "Display method not implemented." << std::endl;
        return nullptr;
    }

    std::shared_ptr<Node> getNode(int index) override {
        if (index < 0 || index >= nodes.size()) {
            throw std::out_of_range("Node index out of range.");
        }
        auto it = nodes.begin();
        std::advance(it, index);
        return it->second;
    }

    std::shared_ptr<Edge> getEdge(int index) override {
        if (index < 0 || index >= edges.size()) {
            throw std::out_of_range("Edge index out of range.");
        }
        auto it = edges.begin();
        std::advance(it, index);
        return it->second;
    }

    std::shared_ptr<Edge> addEdge(const std::string& id, int index1, int index2) override {
        auto node1 = getNode(index1);
        auto node2 = getNode(index2);
        return addEdge(id, node1, node2, false);
    }

    std::shared_ptr<Edge> addEdge(const std::string& id, int fromIndex, int toIndex, bool directed) override {
        auto fromNode = getNode(fromIndex);
        auto toNode = getNode(toIndex);
        return addEdge(id, fromNode, toNode, directed);
    }

    std::shared_ptr<Edge> addEdge(const std::string& id, std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) override {
        return addEdge(id, node1, node2, false);
    }

    std::shared_ptr<Edge> addEdge(const std::string& id, std::shared_ptr<Node> from, std::shared_ptr<Node> to, bool directed) override {
        return addEdge(id, from->getId(), to->getId(), directed);
    }

    std::shared_ptr<Edge> removeEdge(int index) override {
        auto edge = getEdge(index);
        edges.erase(edge->getId());
        return edge;
    }

    std::shared_ptr<Edge> removeEdge(int fromIndex, int toIndex) override {
        auto fromNode = getNode(fromIndex);
        auto toNode = getNode(toIndex);
        for (auto it = edges.begin(); it != edges.end(); ++it) {
            if (it->second->getSourceNode() == fromNode && it->second->getTargetNode() == toNode) {
                auto edge = it->second;
                edges.erase(it);
                return edge;
            }
        }
        if (strict)
            throw ElementNotFoundException("Edge not found.");
        return nullptr;
    }

    std::shared_ptr<Edge> removeEdge(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) override {
        return removeEdge(node1->getId(), node2->getId());
    }

    std::shared_ptr<Edge> removeEdge(const std::string& from, const std::string& to) override {
        for (auto it = edges.begin(); it != edges.end(); ++it) {
            if (it->second->getSourceNode()->getId() == from && it->second->getTargetNode()->getId() == to) {
                auto edge = it->second;
                edges.erase(it);
                return edge;
            }
        }
        if (strict)
            throw ElementNotFoundException("Edge not found.");
        return nullptr;
    }

    std::shared_ptr<Edge> removeEdge(const std::string& id) override {
        auto edge = getEdge(id);
        edges.erase(id);
        return edge;
    }

    std::shared_ptr<Edge> removeEdge(std::shared_ptr<Edge> edge) override {
        return removeEdge(edge->getId());
    }

    std::shared_ptr<Node> removeNode(int index) override {
        auto node = getNode(index);
        nodes.erase(node->getId());
        return node;
    }

    std::shared_ptr<Node> removeNode(const std::string& id) override {
        auto node = getNode(id);
        nodes.erase(id);
        return node;
    }

    std::shared_ptr<Node> removeNode(std::shared_ptr<Node> node) override {
        return removeNode(node->getId());
    }
};

#endif // GRAPH_HPP
