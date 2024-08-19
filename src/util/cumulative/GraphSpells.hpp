#ifndef GRAPH_SPELLS_HPP
#define GRAPH_SPELLS_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>

#include "CumulativeSpells.hpp"
#include "CumulativeAttributes.hpp"

class GraphSpells {
public:
    struct EdgeData {
        std::string source;
        std::string target;
        bool directed;

        std::string getSource() const { return source; }
        std::string getTarget() const { return target; }
        bool isDirected() const { return directed; }
    };

    GraphSpells();

    std::vector<std::string> getNodes() const;
    std::vector<std::string> getEdges() const;

    std::shared_ptr<CumulativeSpells> getNodeSpells(const std::string& nodeId) const;
    std::shared_ptr<CumulativeAttributes> getNodeAttributes(const std::string& nodeId) const;

    std::shared_ptr<CumulativeSpells> getEdgeSpells(const std::string& edgeId) const;
    std::shared_ptr<CumulativeAttributes> getEdgeAttributes(const std::string& edgeId) const;
    std::shared_ptr<EdgeData> getEdgeData(const std::string& edgeId) const;

    void stepBegins(double step);
    void nodeAdded(const std::string& nodeId);
    void nodeRemoved(const std::string& nodeId);
    void edgeAdded(const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed);
    void edgeRemoved(const std::string& edgeId);
    void graphCleared();

    void graphAttributeAdded(const std::string& attribute, const std::string& value);
    void graphAttributeChanged(const std::string& attribute, const std::string& oldValue, const std::string& newValue);
    void graphAttributeRemoved(const std::string& attribute);

    void nodeAttributeAdded(const std::string& nodeId, const std::string& attribute, const std::string& value);
    void nodeAttributeChanged(const std::string& nodeId, const std::string& attribute, const std::string& oldValue, const std::string& newValue);
    void nodeAttributeRemoved(const std::string& nodeId, const std::string& attribute);

    void edgeAttributeAdded(const std::string& edgeId, const std::string& attribute, const std::string& value);
    void edgeAttributeChanged(const std::string& edgeId, const std::string& attribute, const std::string& oldValue, const std::string& newValue);
    void edgeAttributeRemoved(const std::string& edgeId, const std::string& attribute);

    std::string toString() const;

private:
    std::shared_ptr<CumulativeSpells> graph;
    std::shared_ptr<CumulativeAttributes> graphAttributes;

    std::unordered_map<std::string, std::shared_ptr<CumulativeSpells>> nodes;
    std::unordered_map<std::string, std::shared_ptr<CumulativeAttributes>> nodesAttributes;

    std::unordered_map<std::string, std::shared_ptr<CumulativeSpells>> edges;
    std::unordered_map<std::string, std::shared_ptr<CumulativeAttributes>> edgesAttributes;
    std::unordered_map<std::string, std::shared_ptr<EdgeData>> edgesData;

    double date;
};

#endif // GRAPH_SPELLS_HPP
