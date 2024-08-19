#include "FileSinkBaseFiltered.hpp"

FileSinkBaseFiltered::FileSinkBaseFiltered()
    : noFilterGraphAttributeAdded(true),
      noFilterGraphAttributeChanged(true),
      noFilterGraphAttributeRemoved(true),
      noFilterNodeAttributeAdded(true),
      noFilterNodeAttributeChanged(true),
      noFilterNodeAttributeRemoved(true),
      noFilterNodeAdded(true),
      noFilterNodeRemoved(true),
      noFilterEdgeAttributeAdded(true),
      noFilterEdgeAttributeChanged(true),
      noFilterEdgeAttributeRemoved(true),
      noFilterEdgeAdded(true),
      noFilterEdgeRemoved(true),
      noFilterGraphCleared(true),
      noFilterStepBegins(true) {
}

std::vector<std::string> FileSinkBaseFiltered::getGraphAttributesFiltered() const {
    return graphAttributesFiltered;
}

void FileSinkBaseFiltered::setGraphAttributesFiltered(const std::vector<std::string>& graphAttributesFiltered) {
    this->graphAttributesFiltered = graphAttributesFiltered;
}

bool FileSinkBaseFiltered::addGraphAttributeFiltered(const std::string& attr) {
    graphAttributesFiltered.push_back(attr);
    return true;
}

bool FileSinkBaseFiltered::removeGraphAttributeFilter(const std::string& attr) {
    auto it = std::find(graphAttributesFiltered.begin(), graphAttributesFiltered.end(), attr);
    if (it != graphAttributesFiltered.end()) {
        graphAttributesFiltered.erase(it);
        return true;
    }
    return false;
}

std::vector<std::string> FileSinkBaseFiltered::getNodeAttributesFiltered() const {
    return nodeAttributesFiltered;
}

void FileSinkBaseFiltered::setNodeAttributesFiltered(const std::vector<std::string>& nodeAttributesFiltered) {
    this->nodeAttributesFiltered = nodeAttributesFiltered;
}

bool FileSinkBaseFiltered::addNodeAttributeFiltered(const std::string& attr) {
    nodeAttributesFiltered.push_back(attr);
    return true;
}

bool FileSinkBaseFiltered::removeNodeAttributeFilter(const std::string& attr) {
    auto it = std::find(nodeAttributesFiltered.begin(), nodeAttributesFiltered.end(), attr);
    if (it != nodeAttributesFiltered.end()) {
        nodeAttributesFiltered.erase(it);
        return true;
    }
    return false;
}

std::vector<std::string> FileSinkBaseFiltered::getEdgeAttributesFiltered() const {
    return edgeAttributesFiltered;
}

void FileSinkBaseFiltered::setEdgeAttributesFiltered(const std::vector<std::string>& edgeAttributesFiltered) {
    this->edgeAttributesFiltered = edgeAttributesFiltered;
}

bool FileSinkBaseFiltered::addEdgeAttributeFiltered(const std::string& attr) {
    edgeAttributesFiltered.push_back(attr);
    return true;
}

bool FileSinkBaseFiltered::removeEdgeAttributeFilter(const std::string& attr) {
    auto it = std::find(edgeAttributesFiltered.begin(), edgeAttributesFiltered.end(), attr);
    if (it != edgeAttributesFiltered.end()) {
        edgeAttributesFiltered.erase(it);
        return true;
    }
    return false;
}

bool FileSinkBaseFiltered::isNoFilterGraphAttributeAdded() const {
    return noFilterGraphAttributeAdded;
}

void FileSinkBaseFiltered::setNoFilterGraphAttributeAdded(bool noFilter) {
    noFilterGraphAttributeAdded = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterGraphAttributeChanged() const {
    return noFilterGraphAttributeChanged;
}

void FileSinkBaseFiltered::setNoFilterGraphAttributeChanged(bool noFilter) {
    noFilterGraphAttributeChanged = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterGraphAttributeRemoved() const {
    return noFilterGraphAttributeRemoved;
}

void FileSinkBaseFiltered::setNoFilterGraphAttributeRemoved(bool noFilter) {
    noFilterGraphAttributeRemoved = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterNodeAttributeAdded() const {
    return noFilterNodeAttributeAdded;
}

void FileSinkBaseFiltered::setNoFilterNodeAttributeAdded(bool noFilter) {
    noFilterNodeAttributeAdded = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterNodeAttributeChanged() const {
    return noFilterNodeAttributeChanged;
}

void FileSinkBaseFiltered::setNoFilterNodeAttributeChanged(bool noFilter) {
    noFilterNodeAttributeChanged = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterNodeAttributeRemoved() const {
    return noFilterNodeAttributeRemoved;
}

void FileSinkBaseFiltered::setNoFilterNodeAttributeRemoved(bool noFilter) {
    noFilterNodeAttributeRemoved = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterNodeAdded() const {
    return noFilterNodeAdded;
}

void FileSinkBaseFiltered::setNoFilterNodeAdded(bool noFilter) {
    noFilterNodeAdded = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterNodeRemoved() const {
    return noFilterNodeRemoved;
}

void FileSinkBaseFiltered::setNoFilterNodeRemoved(bool noFilter) {
    noFilterNodeRemoved = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterEdgeAttributeAdded() const {
    return noFilterEdgeAttributeAdded;
}

void FileSinkBaseFiltered::setNoFilterEdgeAttributeAdded(bool noFilter) {
    noFilterEdgeAttributeAdded = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterEdgeAttributeChanged() const {
    return noFilterEdgeAttributeChanged;
}

void FileSinkBaseFiltered::setNoFilterEdgeAttributeChanged(bool noFilter) {
    noFilterEdgeAttributeChanged = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterEdgeAttributeRemoved() const {
    return noFilterEdgeAttributeRemoved;
}

void FileSinkBaseFiltered::setNoFilterEdgeAttributeRemoved(bool noFilter) {
    noFilterEdgeAttributeRemoved = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterEdgeAdded() const {
    return noFilterEdgeAdded;
}

void FileSinkBaseFiltered::setNoFilterEdgeAdded(bool noFilter) {
    noFilterEdgeAdded = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterEdgeRemoved() const {
    return noFilterEdgeRemoved;
}

void FileSinkBaseFiltered::setNoFilterEdgeRemoved(bool noFilter) {
    noFilterEdgeRemoved = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterGraphCleared() const {
    return noFilterGraphCleared;
}

void FileSinkBaseFiltered::setNoFilterGraphCleared(bool noFilter) {
    noFilterGraphCleared = noFilter;
}

bool FileSinkBaseFiltered::isNoFilterStepBegins() const {
    return noFilterStepBegins;
}

void FileSinkBaseFiltered::setNoFilterStepBegins(bool noFilter) {
    noFilterStepBegins = noFilter;
}
