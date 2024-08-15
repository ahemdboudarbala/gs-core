#include "AttributePipe.hpp"

AttributePipe::AttributePipe() 
    : globalPredicate(std::make_shared<FalsePredicate>()),
      graphPredicate(std::make_shared<FalsePredicate>()),
      nodePredicate(std::make_shared<FalsePredicate>()),
      edgePredicate(std::make_shared<FalsePredicate>()) {}

void AttributePipe::setGlobalAttributeFilter(std::shared_ptr<AttributePredicate> filter) {
    globalPredicate = filter ? filter : std::make_shared<FalsePredicate>();
}

void AttributePipe::setGraphAttributeFilter(std::shared_ptr<AttributePredicate> filter) {
    graphPredicate = filter ? filter : std::make_shared<FalsePredicate>();
}

void AttributePipe::setNodeAttributeFilter(std::shared_ptr<AttributePredicate> filter) {
    nodePredicate = filter ? filter : std::make_shared<FalsePredicate>();
}

void AttributePipe::setEdgeAttributeFilter(std::shared_ptr<AttributePredicate> filter) {
    edgePredicate = filter ? filter : std::make_shared<FalsePredicate>();
}

std::shared_ptr<AttributePredicate> AttributePipe::getGlobalAttributeFilter() const {
    return globalPredicate;
}

std::shared_ptr<AttributePredicate> AttributePipe::getGraphAttributeFilter() const {
    return graphPredicate;
}

std::shared_ptr<AttributePredicate> AttributePipe::getNodeAttributeFilter() const {
    return nodePredicate;
}

std::shared_ptr<AttributePredicate> AttributePipe::getEdgeAttributeFilter() const {
    return edgePredicate;
}

void AttributePipe::edgeAttributeAdded(const std::string& graphId, long timeId, const std::string& edgeId,
                                       const std::string& attribute, const std::any& value) {
    if (!edgePredicate->matches(attribute, value) && !globalPredicate->matches(attribute, value)) {
        sendEdgeAttributeAdded(graphId, timeId, edgeId, attribute, value);
    }
}

void AttributePipe::edgeAttributeChanged(const std::string& graphId, long timeId, const std::string& edgeId,
                                         const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    if (!edgePredicate->matches(attribute, newValue) && !globalPredicate->matches(attribute, newValue)) {
        sendEdgeAttributeChanged(graphId, timeId, edgeId, attribute, oldValue, newValue);
    }
}

void AttributePipe::edgeAttributeRemoved(const std::string& graphId, long timeId, const std::string& edgeId,
                                         const std::string& attribute) {
    if (!edgePredicate->matches(attribute, std::any()) && !globalPredicate->matches(attribute, std::any())) {
        sendEdgeAttributeRemoved(graphId, timeId, edgeId, attribute);
    }
}

void AttributePipe::graphAttributeAdded(const std::string& graphId, long timeId, const std::string& attribute,
                                        const std::any& value) {
    if (!graphPredicate->matches(attribute, value) && !globalPredicate->matches(attribute, value)) {
        sendGraphAttributeAdded(graphId, timeId, attribute, value);
    }
}

void AttributePipe::graphAttributeChanged(const std::string& graphId, long timeId, const std::string& attribute,
                                          const std::any& oldValue, const std::any& newValue) {
    if (!graphPredicate->matches(attribute, newValue) && !globalPredicate->matches(attribute, newValue)) {
        sendGraphAttributeChanged(graphId, timeId, attribute, oldValue, newValue);
    }
}

void AttributePipe::graphAttributeRemoved(const std::string& graphId, long timeId, const std::string& attribute) {
    if (!graphPredicate->matches(attribute, std::any()) && !globalPredicate->matches(attribute, std::any())) {
        sendGraphAttributeRemoved(graphId, timeId, attribute);
    }
}

void AttributePipe::nodeAttributeAdded(const std::string& graphId, long timeId, const std::string& nodeId,
                                       const std::string& attribute, const std::any& value) {
    if (!nodePredicate->matches(attribute, value) && !globalPredicate->matches(attribute, value)) {
        sendNodeAttributeAdded(graphId, timeId, nodeId, attribute, value);
    }
}

void AttributePipe::nodeAttributeChanged(const std::string& graphId, long timeId, const std::string& nodeId,
                                         const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    if (!nodePredicate->matches(attribute, newValue) && !globalPredicate->matches(attribute, newValue)) {
        sendNodeAttributeChanged(graphId, timeId, nodeId, attribute, oldValue, newValue);
    }
}

void AttributePipe::nodeAttributeRemoved(const std::string& graphId, long timeId, const std::string& nodeId,
                                         const std::string& attribute) {
    if (!nodePredicate->matches(attribute, std::any()) && !globalPredicate->matches(attribute, std::any())) {
        sendNodeAttributeRemoved(graphId, timeId, nodeId, attribute);
    }
}
