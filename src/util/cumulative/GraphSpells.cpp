#include "GraphSpells.hpp"
#include "CumulativeSpells.hpp"
#include "CumulativeAttributes.hpp"

GraphSpells::GraphSpells()
    : date(std::numeric_limits<double>::quiet_NaN()) {
    graph = std::make_shared<CumulativeSpells>();
    graphAttributes = std::make_shared<CumulativeAttributes>(0.0);
}

std::vector<std::string> GraphSpells::getNodes() const {
    std::vector<std::string> nodeKeys;
    for (const auto& node : nodes) {
        nodeKeys.push_back(node.first);
    }
    return nodeKeys;
}

std::vector<std::string> GraphSpells::getEdges() const {
    std::vector<std::string> edgeKeys;
    for (const auto& edge : edges) {
        edgeKeys.push_back(edge.first);
    }
    return edgeKeys;
}

std::shared_ptr<CumulativeSpells> GraphSpells::getNodeSpells(const std::string& nodeId) const {
    auto it = nodes.find(nodeId);
    if (it != nodes.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<CumulativeAttributes> GraphSpells::getNodeAttributes(const std::string& nodeId) const {
    auto it = nodesAttributes.find(nodeId);
    if (it != nodesAttributes.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<CumulativeSpells> GraphSpells::getEdgeSpells(const std::string& edgeId) const {
    auto it = edges.find(edgeId);
    if (it != edges.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<CumulativeAttributes> GraphSpells::getEdgeAttributes(const std::string& edgeId) const {
    auto it = edgesAttributes.find(edgeId);
    if (it != edgesAttributes.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<GraphSpells::EdgeData> GraphSpells::getEdgeData(const std::string& edgeId) const {
    auto it = edgesData.find(edgeId);
    if (it != edgesData.end()) {
        return it->second;
    }
    return nullptr;
}

void GraphSpells::stepBegins(double step) {
    this->date = step;

    graphAttributes->updateDate(step);
    graph->updateCurrentSpell(step);

    for (const auto& node : nodes) {
        node.second->updateCurrentSpell(step);
        nodesAttributes[node.first]->updateDate(step);
    }

    for (const auto& edge : edges) {
        edge.second->updateCurrentSpell(step);
        edgesAttributes[edge.first]->updateDate(step);
    }
}

void GraphSpells::nodeAdded(const std::string& nodeId) {
    if (nodes.find(nodeId) == nodes.end()) {
        nodes[nodeId] = std::make_shared<CumulativeSpells>();
        nodesAttributes[nodeId] = std::make_shared<CumulativeAttributes>(date);
    }
    nodes[nodeId]->startSpell(date);
}

void GraphSpells::nodeRemoved(const std::string& nodeId) {
    auto it = nodes.find(nodeId);
    if (it != nodes.end()) {
        it->second->closeSpell();
        nodesAttributes[nodeId]->remove();
    }
}

void GraphSpells::edgeAdded(const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) {
    if (edges.find(edgeId) == edges.end()) {
        edges[edgeId] = std::make_shared<CumulativeSpells>();
        edgesAttributes[edgeId] = std::make_shared<CumulativeAttributes>(date);

        auto data = std::make_shared<EdgeData>();
        data->source = fromNodeId;
        data->target = toNodeId;
        data->directed = directed;

        edgesData[edgeId] = data;
    }

    edges[edgeId]->startSpell(date);

    auto data = edgesData[edgeId];
    if (data->source != fromNodeId || data->target != toNodeId || data->directed != directed) {
        std::cerr << "Warning: An edge with this id but different properties has already been created in the past." << std::endl;
    }
}

void GraphSpells::edgeRemoved(const std::string& edgeId) {
    auto it = edges.find(edgeId);
    if (it != edges.end()) {
        it->second->closeSpell();
        edgesAttributes[edgeId]->remove();
    }
}

void GraphSpells::graphCleared() {
    for (auto& node : nodes) {
        node.second->closeSpell();
        nodesAttributes[node.first]->remove();
    }

    for (auto& edge : edges) {
        edge.second->closeSpell();
        edgesAttributes[edge.first]->remove();
    }
}

void GraphSpells::graphAttributeAdded(const std::string& attribute, const std::string& value) {
    graphAttributes->set(attribute, value);
}

void GraphSpells::graphAttributeChanged(const std::string& attribute, const std::string& oldValue, const std::string& newValue) {
    graphAttributes->set(attribute, newValue);
}

void GraphSpells::graphAttributeRemoved(const std::string& attribute) {
    graphAttributes->remove(attribute);
}

void GraphSpells::nodeAttributeAdded(const std::string& nodeId, const std::string& attribute, const std::string& value) {
    nodesAttributes[nodeId]->set(attribute, value);
}

void GraphSpells::nodeAttributeChanged(const std::string& nodeId, const std::string& attribute, const std::string& oldValue, const std::string& newValue) {
    nodesAttributes[nodeId]->set(attribute, newValue);
}

void GraphSpells::nodeAttributeRemoved(const std::string& nodeId, const std::string& attribute) {
    nodesAttributes[nodeId]->remove(attribute);
}

void GraphSpells::edgeAttributeAdded(const std::string& edgeId, const std::string& attribute, const std::string& value) {
    edgesAttributes[edgeId]->set(attribute, value);
}

void GraphSpells::edgeAttributeChanged(const std::string& edgeId, const std::string& attribute, const std::string& oldValue, const std::string& newValue) {
    edgesAttributes[edgeId]->set(attribute, newValue);
}

void GraphSpells::edgeAttributeRemoved(const std::string& edgeId, const std::string& attribute) {
    edgesAttributes[edgeId]->remove(attribute);
}

std::string GraphSpells::toString() const {
    std::ostringstream buffer;

    for (const auto& node : nodes) {
        buffer << "node#\"" << node.first << "\" " << *node.second << " " << *nodesAttributes.at(node.first) << "\n";
    }

    for (const auto& edge : edges) {
        buffer << "edge#\"" << edge.first << "\" " << *edge.second << "\n";
    }

    return buffer.str();
}
