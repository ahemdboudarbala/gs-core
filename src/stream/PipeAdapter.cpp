#include "PipeAdapter.hpp"

void PipeAdapter::addAttributeSink(AttributeSink* listener) {
    // Empty method
}

void PipeAdapter::addElementSink(ElementSink* listener) {
    // Empty method
}

void PipeAdapter::addSink(Sink* listener) {
    // Empty method
}

void PipeAdapter::removeAttributeSink(AttributeSink* listener) {
    // Empty method
}

void PipeAdapter::removeElementSink(ElementSink* listener) {
    // Empty method
}

void PipeAdapter::removeSink(Sink* listener) {
    // Empty method
}

void PipeAdapter::edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& value) {
    // Empty method
}

void PipeAdapter::edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    // Empty method
}

void PipeAdapter::edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute) {
    // Empty method
}

void PipeAdapter::graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, const std::any& value) {
    // Empty method
}

void PipeAdapter::graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    // Empty method
}

void PipeAdapter::graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) {
    // Empty method
}

void PipeAdapter::nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& value) {
    // Empty method
}

void PipeAdapter::nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    // Empty method
}

void PipeAdapter::nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute) {
    // Empty method
}

void PipeAdapter::edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) {
    // Empty method
}

void PipeAdapter::edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) {
    // Empty method
}

void PipeAdapter::graphCleared(const std::string& sourceId, long timeId) {
    // Empty method
}

void PipeAdapter::nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) {
    // Empty method
}

void PipeAdapter::nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) {
    // Empty method
}

void PipeAdapter::stepBegins(const std::string& sourceId, long timeId, double step) {
    // Empty method
}

void PipeAdapter::clearAttributeSinks() {
    // Empty method
}

void PipeAdapter::clearElementSinks() {
    // Empty method
}

void PipeAdapter::clearSinks() {
    // Empty method
}
