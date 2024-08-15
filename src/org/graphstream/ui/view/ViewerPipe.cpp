#include "ViewerPipe.hpp"

ViewerPipe::ViewerPipe(const std::string& id, std::shared_ptr<ProxyPipe> pipeIn)
    : id(id), pipeIn(pipeIn) {
    pipeIn->addSink(shared_from_this());
}

std::string ViewerPipe::getId() const {
    return id;
}

void ViewerPipe::pump() {
    pipeIn->pump();
}

void ViewerPipe::blockingPump() {
    pipeIn->blockingPump();
}

void ViewerPipe::blockingPump(long timeout) {
    pipeIn->blockingPump(timeout);
}

void ViewerPipe::addViewerListener(std::shared_ptr<ViewerListener> listener) {
    viewerListeners.insert(listener);
}

void ViewerPipe::removeViewerListener(std::shared_ptr<ViewerListener> listener) {
    viewerListeners.erase(listener);
}

void ViewerPipe::edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, void* value) {
    sendEdgeAttributeAdded(sourceId, timeId, edgeId, attribute, value);
}

void ViewerPipe::edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, void* oldValue, void* newValue) {
    sendEdgeAttributeChanged(sourceId, timeId, edgeId, attribute, oldValue, newValue);
}

void ViewerPipe::edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute) {
    sendEdgeAttributeRemoved(sourceId, timeId, edgeId, attribute);
}

void ViewerPipe::graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, void* value) {
    sendGraphAttributeAdded(sourceId, timeId, attribute, value);

    if (attribute == "ui.viewClosed" && value) {
        for (const auto& listener : viewerListeners) {
            listener->viewClosed(static_cast<const char*>(value));
        }
        sendGraphAttributeRemoved(id, attribute);
    } else if (attribute == "ui.clicked" && value) {
        for (const auto& listener : viewerListeners) {
            listener->buttonPushed(static_cast<const char*>(value));
        }
        sendGraphAttributeRemoved(id, attribute);
    }
}

void ViewerPipe::graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, void* oldValue, void* newValue) {
    sendGraphAttributeChanged(sourceId, timeId, attribute, oldValue, newValue);
}

void ViewerPipe::graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) {
    sendGraphAttributeRemoved(sourceId, timeId, attribute);
}

void ViewerPipe::nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, void* value) {
    sendNodeAttributeAdded(sourceId, timeId, nodeId, attribute, value);

    if (attribute == "ui.clicked") {
        for (const auto& listener : viewerListeners) {
            listener->buttonPushed(nodeId);
        }
    }

    if (attribute == "ui.mouseOver") {
        for (const auto& listener : viewerListeners) {
            listener->mouseOver(nodeId);
        }
    }
}

void ViewerPipe::nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, void* oldValue, void* newValue) {
    sendNodeAttributeChanged(sourceId, timeId, nodeId, attribute, oldValue, newValue);
}

void ViewerPipe::nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute) {
    sendNodeAttributeRemoved(sourceId, timeId, nodeId, attribute);

    if (attribute == "ui.clicked") {
        for (const auto& listener : viewerListeners) {
            listener->buttonReleased(nodeId);
        }
    }

    if (attribute == "ui.mouseOver") {
        for (const auto& listener : viewerListeners) {
            listener->mouseLeft(nodeId);
        }
    }
}

void ViewerPipe::edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) {
    sendEdgeAdded(sourceId, timeId, edgeId, fromNodeId, toNodeId, directed);
}

void ViewerPipe::edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) {
    sendEdgeRemoved(sourceId, timeId, edgeId);
}

void ViewerPipe::graphCleared(const std::string& sourceId, long timeId) {
    sendGraphCleared(sourceId, timeId);
}

void ViewerPipe::nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) {
    sendNodeAdded(sourceId, timeId, nodeId);
}

void ViewerPipe::nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) {
    sendNodeRemoved(sourceId, timeId, nodeId);
}

void ViewerPipe::stepBegins(const std::string& sourceId, long timeId, double step) {
    sendStepBegins(sourceId, timeId, step);
}
