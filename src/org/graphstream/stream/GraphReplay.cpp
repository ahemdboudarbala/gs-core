#include "GraphReplay.hpp"
#include <string>
#include <vector>

GraphReplay::GraphReplay(const std::string& id)
    : SourceBase(id) {}

void GraphReplay::replay(const Graph& graph) {
    // Send all graph attributes as attribute-addition events.
    for (const auto& key : graph.attributeKeys()) {
        sendGraphAttributeAdded(sourceId, key, graph.getAttribute(key));
    }

    // Send all nodes as node-creation events.
    for (const auto& node : graph.nodes()) {
        std::string nodeId = node.getId();
        sendNodeAdded(sourceId, nodeId);

        if (node.getAttributeCount() > 0) {
            for (const auto& key : node.attributeKeys()) {
                sendNodeAttributeAdded(sourceId, nodeId, key, node.getAttribute(key));
            }
        }
    }

    // Send all edges as edge-creation events.
    for (const auto& edge : graph.edges()) {
        std::string edgeId = edge.getId();
        sendEdgeAdded(sourceId, edgeId, edge.getNode0().getId(), edge.getNode1().getId(), edge.isDirected());

        if (edge.getAttributeCount() > 0) {
            for (const auto& key : edge.attributeKeys()) {
                sendEdgeAttributeAdded(sourceId, edgeId, key, edge.getAttribute(key));
            }
        }
    }
}
