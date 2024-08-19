#include "GraphListeners.hpp"

GraphListeners::GraphListeners(Graph* graph)
    : SourceBase(graph->getId()), sinkTime(), passYourWay(false), passYourWayAE(false),
      dnSourceId(""), dnTimeId(LONG_MIN), g(graph) {
    sourceTime.setSinkTime(&sinkTime);
}

long GraphListeners::newEvent() {
    return sourceTime.newEvent();
}

void GraphListeners::sendAttributeChangedEvent(const std::string& eltId, ElementType eltType, const std::string& attribute,
                                               AttributeChangeEvent event, const void* oldValue, const void* newValue) {
    if (passYourWay || attribute[0] == '.') return;

    sendAttributeChangedEvent(sourceId, newEvent(), eltId, eltType, attribute, event, oldValue, newValue);
}

void GraphListeners::sendNodeAdded(const std::string& nodeId) {
    if (passYourWay) return;

    sendNodeAdded(sourceId, newEvent(), nodeId);
}

void GraphListeners::sendNodeRemoved(const std::string& nodeId) {
    if (!dnSourceId.empty()) {
        sendNodeRemoved(dnSourceId, dnTimeId, nodeId);
    } else {
        sendNodeRemoved(sourceId, newEvent(), nodeId);
    }
}

void GraphListeners::sendEdgeAdded(const std::string& edgeId, const std::string& source, const std::string& target, bool directed) {
    if (passYourWayAE) return;

    sendEdgeAdded(sourceId, newEvent(), edgeId, source, target, directed);
}

void GraphListeners::sendEdgeRemoved(const std::string& edgeId) {
    if (passYourWay) return;

    sendEdgeRemoved(sourceId, newEvent(), edgeId);
}

void GraphListeners::sendGraphCleared() {
    if (passYourWay) return;

    sendGraphCleared(sourceId, newEvent());
}

void GraphListeners::sendStepBegins(double step) {
    if (passYourWay) return;

    sendStepBegins(sourceId, newEvent(), step);
}

void GraphListeners::edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId,
                                        const std::string& attribute, const void* value) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        Edge* edge = g->getEdge(edgeId);
        if (edge != nullptr) {
            passYourWay = true;

            try {
                edge->setAttribute(attribute, value);
            } catch (...) {
                passYourWay = false;
                throw;
            }

            passYourWay = false;
            sendEdgeAttributeAdded(sourceId, timeId, edgeId, attribute, value);
        }
    }
}

void GraphListeners::edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId,
                                          const std::string& attribute, const void* oldValue, const void* newValue) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        Edge* edge = g->getEdge(edgeId);
        if (edge != nullptr) {
            passYourWay = true;

            void* actualOldValue = oldValue == nullptr ? edge->getAttribute(attribute) : oldValue;

            try {
                edge->setAttribute(attribute, newValue);
            } catch (...) {
                passYourWay = false;
                throw;
            }

            passYourWay = false;
            sendEdgeAttributeChanged(sourceId, timeId, edgeId, attribute, actualOldValue, newValue);
        }
    }
}

void GraphListeners::edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId,
                                          const std::string& attribute) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        Edge* edge = g->getEdge(edgeId);
        if (edge != nullptr) {
            sendEdgeAttributeRemoved(sourceId, timeId, edgeId, attribute);
            passYourWay = true;

            try {
                edge->removeAttribute(attribute);
            } catch (...) {
                passYourWay = false;
                throw;
            }

            passYourWay = false;
        }
    }
}

void GraphListeners::graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute,
                                         const void* value) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        passYourWay = true;

        try {
            g->setAttribute(attribute, value);
        } catch (...) {
            passYourWay = false;
            throw;
        }

        passYourWay = false;
        sendGraphAttributeAdded(sourceId, timeId, attribute, value);
    }
}

void GraphListeners::graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute,
                                           const void* oldValue, const void* newValue) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        passYourWay = true;

        void* actualOldValue = oldValue == nullptr ? g->getAttribute(attribute) : oldValue;

        try {
            g->setAttribute(attribute, newValue);
        } catch (...) {
            passYourWay = false;
            throw;
        }

        passYourWay = false;
        sendGraphAttributeChanged(sourceId, timeId, attribute, actualOldValue, newValue);
    }
}

void GraphListeners::graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        sendGraphAttributeRemoved(sourceId, timeId, attribute);
        passYourWay = true;

        try {
            g->removeAttribute(attribute);
        } catch (...) {
            passYourWay = false;
            throw;
        }

        passYourWay = false;
    }
}

void GraphListeners::nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId,
                                        const std::string& attribute, const void* value) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        Node* node = g->getNode(nodeId);
        if (node != nullptr) {
            passYourWay = true;

            try {
                node->setAttribute(attribute, value);
            } catch (...) {
                passYourWay = false;
                throw;
            }

            passYourWay = false;
            sendNodeAttributeAdded(sourceId, timeId, nodeId, attribute, value);
        }
    }
}

void GraphListeners::nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId,
                                          const std::string& attribute, const void* oldValue, const void* newValue) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        Node* node = g->getNode(nodeId);
        if (node != nullptr) {
            passYourWay = true;

            void* actualOldValue = oldValue == nullptr ? node->getAttribute(attribute) : oldValue;

            try {
                node->setAttribute(attribute, newValue);
            } catch (...) {
                passYourWay = false;
                throw;
            }

            passYourWay = false;
            sendNodeAttributeChanged(sourceId, timeId, nodeId, attribute, actualOldValue, newValue);
        }
    }
}

void GraphListeners::nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId,
                                          const std::string& attribute) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        Node* node = g->getNode(nodeId);
        if (node != nullptr) {
            sendNodeAttributeRemoved(sourceId, timeId, nodeId, attribute);
            passYourWay = true;

            try {
                node->removeAttribute(attribute);
            } catch (...) {
                passYourWay = false;
                throw;
            }

            passYourWay = false;
        }
    }
}

void GraphListeners::edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId,
                               const std::string& fromNodeId, const std::string& toNodeId, bool directed) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        passYourWayAE = true;

        try {
            g->addEdge(edgeId, fromNodeId, toNodeId, directed);
        } catch (...) {
            passYourWayAE = false;
            throw;
        }

        passYourWayAE = false;
        sendEdgeAdded(sourceId, timeId, edgeId, fromNodeId, toNodeId, directed);
    }
}

void GraphListeners::edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        sendEdgeRemoved(sourceId, timeId, edgeId);
        passYourWay = true;

        try {
            g->removeEdge(edgeId);
        } catch (...) {
            passYourWay = false;
            throw;
        }

        passYourWay = false;
    }
}

void GraphListeners::graphCleared(const std::string& sourceId, long timeId) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        sendGraphCleared(sourceId, timeId);
        passYourWay = true;

        try {
            g->clear();
        } catch (...) {
            passYourWay = false;
            throw;
        }

        passYourWay = false;
    }
}

void GraphListeners::nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        passYourWay = true;

        try {
            g->addNode(nodeId);
        } catch (...) {
            passYourWay = false;
            throw;
        }

        passYourWay = false;
        sendNodeAdded(sourceId, timeId, nodeId);
    }
}

void GraphListeners::nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        dnSourceId = sourceId;
        dnTimeId = timeId;

        try {
            g->removeNode(nodeId);
        } catch (...) {
            dnSourceId.clear();
            dnTimeId = LONG_MIN;
            throw;
        }

        dnSourceId.clear();
        dnTimeId = LONG_MIN;
    }
}

void GraphListeners::stepBegins(const std::string& sourceId, long timeId, double step) {
    if (sinkTime.isNewEvent(sourceId, timeId)) {
        passYourWay = true;

        try {
            g->stepBegins(step);
        } catch (...) {
            passYourWay = false;
            throw;
        }

        passYourWay = false;
        sendStepBegins(sourceId, timeId, step);
    }
}

std::string GraphListeners::toString() const {
    return "GraphListeners of " + g->getClassSimpleName() + "." + g->getId();
}
