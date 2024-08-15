#include "PipeBase.hpp"

void PipeBase::edgeAttributeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& value) {
    sendEdgeAttributeAdded(graphId, timeId, edgeId, attribute, value);
}

void PipeBase::edgeAttributeChanged(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    sendEdgeAttributeChanged(graphId, timeId, edgeId, attribute, oldValue, newValue);
}

void PipeBase::edgeAttributeRemoved(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute) {
    sendEdgeAttributeRemoved(graphId, timeId, edgeId, attribute);
}

void PipeBase::graphAttributeAdded(const std::string& graphId, long timeId, const std::string& attribute, const std::any& value) {
    sendGraphAttributeAdded(graphId, timeId, attribute, value);
}

void PipeBase::graphAttributeChanged(const std::string& graphId, long timeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    sendGraphAttributeChanged(graphId, timeId, attribute, oldValue, newValue);
}

void PipeBase::graphAttributeRemoved(const std::string& graphId, long timeId, const std::string& attribute) {
    sendGraphAttributeRemoved(graphId, timeId, attribute);
}

void PipeBase::nodeAttributeAdded(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& value) {
    sendNodeAttributeAdded(graphId, timeId, nodeId, attribute, value);
}

void PipeBase::nodeAttributeChanged(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    sendNodeAttributeChanged(graphId, timeId, nodeId, attribute, oldValue, newValue);
}

void PipeBase::nodeAttributeRemoved(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute) {
    sendNodeAttributeRemoved(graphId, timeId, nodeId, attribute);
}

void PipeBase::edgeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) {
    sendEdgeAdded(graphId, timeId, edgeId, fromNodeId, toNodeId, directed);
}

void PipeBase::edgeRemoved(const std::string& graphId, long timeId, const std::string& edgeId) {
    sendEdgeRemoved(graphId, timeId, edgeId);
}

void PipeBase::graphCleared(const std::string& graphId, long timeId) {
    sendGraphCleared(graphId, timeId);
}

void PipeBase::nodeAdded(const std::string& graphId, long timeId, const std::string& nodeId) {
    sendNodeAdded(graphId, timeId, nodeId);
}

void PipeBase::nodeRemoved(const std::string& graphId, long timeId, const std::string& nodeId) {
    sendNodeRemoved(graphId, timeId, nodeId);
}

void PipeBase::stepBegins(const std::string& graphId, long timeId, double step) {
    sendStepBegins(graphId, timeId, step);
}
