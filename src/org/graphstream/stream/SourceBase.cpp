#include "SourceBase.hpp"
#include "AttributeSink.hpp"
#include "ElementSink.hpp"
#include "Sink.hpp"
#include "SourceTime.hpp"

SourceBase::SourceBase()
    : eventProcessing(false), sourceId("sourceOnThread#" + std::to_string(std::this_thread::get_id()) + "_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count())) {
    sourceTime = std::make_unique<SourceTime>(sourceId);
}

SourceBase::SourceBase(const std::string& sourceId)
    : sourceId(sourceId), eventProcessing(false) {
    sourceTime = std::make_unique<SourceTime>(sourceId);
}

const std::vector<AttributeSink*>& SourceBase::attributeSinks() const {
    return attrSinks;
}

const std::vector<ElementSink*>& SourceBase::elementSinks() const {
    return eltsSinks;
}

void SourceBase::addSink(Sink* sink) {
    addAttributeSink(sink);
    addElementSink(sink);
}

void SourceBase::addAttributeSink(AttributeSink* sink) {
    if (!eventProcessing) {
        eventProcessing = true;
        manageEvents();
        attrSinks.push_back(sink);
        manageEvents();
        eventProcessing = false;
    } else {
        eventQueue.emplace_back(new AddToListEvent<AttributeSink>(attrSinks, sink));
    }
}

void SourceBase::addElementSink(ElementSink* sink) {
    if (!eventProcessing) {
        eventProcessing = true;
        manageEvents();
        eltsSinks.push_back(sink);
        manageEvents();
        eventProcessing = false;
    } else {
        eventQueue.emplace_back(new AddToListEvent<ElementSink>(eltsSinks, sink));
    }
}

void SourceBase::clearSinks() {
    clearElementSinks();
    clearAttributeSinks();
}

void SourceBase::clearElementSinks() {
    if (!eventProcessing) {
        eventProcessing = true;
        manageEvents();
        eltsSinks.clear();
        manageEvents();
        eventProcessing = false;
    } else {
        eventQueue.emplace_back(new ClearListEvent<ElementSink>(eltsSinks));
    }
}

void SourceBase::clearAttributeSinks() {
    if (!eventProcessing) {
        eventProcessing = true;
        manageEvents();
        attrSinks.clear();
        manageEvents();
        eventProcessing = false;
    } else {
        eventQueue.emplace_back(new ClearListEvent<AttributeSink>(attrSinks));
    }
}

void SourceBase::removeSink(Sink* sink) {
    removeAttributeSink(sink);
    removeElementSink(sink);
}

void SourceBase::removeAttributeSink(AttributeSink* sink) {
    if (!eventProcessing) {
        eventProcessing = true;
        manageEvents();
        attrSinks.erase(std::remove(attrSinks.begin(), attrSinks.end(), sink), attrSinks.end());
        manageEvents();
        eventProcessing = false;
    } else {
        eventQueue.emplace_back(new RemoveFromListEvent<AttributeSink>(attrSinks, sink));
    }
}

void SourceBase::removeElementSink(ElementSink* sink) {
    if (!eventProcessing) {
        eventProcessing = true;
        manageEvents();
        eltsSinks.erase(std::remove(eltsSinks.begin(), eltsSinks.end(), sink), eltsSinks.end());
        manageEvents();
        eventProcessing = false;
    } else {
        eventQueue.emplace_back(new RemoveFromListEvent<ElementSink>(eltsSinks, sink));
    }
}

void SourceBase::manageEvents() {
    if (eventProcessing) {
        while (!eventQueue.empty()) {
            eventQueue.front()->trigger();
            eventQueue.pop_front();
        }
    }
}

// Implementations for events
SourceBase::GraphEvent::GraphEvent(const std::string& sourceId, long timeId)
    : sourceId(sourceId), timeId(timeId) {}

SourceBase::AfterEdgeAddEvent::AfterEdgeAddEvent(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed)
    : GraphEvent(sourceId, timeId), edgeId(edgeId), fromNodeId(fromNodeId), toNodeId(toNodeId), directed(directed) {}

void SourceBase::AfterEdgeAddEvent::trigger() {
    for (ElementSink* sink : eltsSinks) {
        sink->edgeAdded(sourceId, timeId, edgeId, fromNodeId, toNodeId, directed);
    }
}

SourceBase::BeforeEdgeRemoveEvent::BeforeEdgeRemoveEvent(const std::string& sourceId, long timeId, const std::string& edgeId)
    : GraphEvent(sourceId, timeId), edgeId(edgeId) {}

void SourceBase::BeforeEdgeRemoveEvent::trigger() {
    for (ElementSink* sink : eltsSinks) {
        sink->edgeRemoved(sourceId, timeId, edgeId);
    }
}

SourceBase::AfterNodeAddEvent::AfterNodeAddEvent(const std::string& sourceId, long timeId, const std::string& nodeId)
    : GraphEvent(sourceId, timeId), nodeId(nodeId) {}

void SourceBase::AfterNodeAddEvent::trigger() {
    for (ElementSink* sink : eltsSinks) {
        sink->nodeAdded(sourceId, timeId, nodeId);
    }
}

SourceBase::BeforeNodeRemoveEvent::BeforeNodeRemoveEvent(const std::string& sourceId, long timeId, const std::string& nodeId)
    : GraphEvent(sourceId, timeId), nodeId(nodeId) {}

void SourceBase::BeforeNodeRemoveEvent::trigger() {
    for (ElementSink* sink : eltsSinks) {
        sink->nodeRemoved(sourceId, timeId, nodeId);
    }
}

SourceBase::BeforeGraphClearEvent::BeforeGraphClearEvent(const std::string& sourceId, long timeId)
    : GraphEvent(sourceId, timeId) {}

void SourceBase::BeforeGraphClearEvent::trigger() {
    for (ElementSink* sink : eltsSinks) {
        sink->graphCleared(sourceId, timeId);
    }
}

SourceBase::StepBeginsEvent::StepBeginsEvent(const std::string& sourceId, long timeId, double step)
    : GraphEvent(sourceId, timeId), step(step) {}

void SourceBase::StepBeginsEvent::trigger() {
    for (ElementSink* sink : eltsSinks) {
        sink->stepBegins(sourceId, timeId, step);
    }
}

SourceBase::AttributeChangedEvent::AttributeChangedEvent(const std::string& sourceId, long timeId, const std::string& eltId, ElementType eltType, const std::string& attribute, AttributeChangeEvent event, const std::any& oldValue, const std::any& newValue)
    : GraphEvent(sourceId, timeId), eltType(eltType), eltId(eltId), attribute(attribute), event(event), oldValue(oldValue), newValue(newValue) {}

void SourceBase::AttributeChangedEvent::trigger() {
    switch (event) {
    case AttributeChangeEvent::ADD:
        if (eltType == ElementType::NODE) {
            for (AttributeSink* sink : attrSinks) {
                sink->nodeAttributeAdded(sourceId, timeId, eltId, attribute, newValue);
            }
        } else if (eltType == ElementType::EDGE) {
            for (AttributeSink* sink : attrSinks) {
                sink->edgeAttributeAdded(sourceId, timeId, eltId, attribute, newValue);
            }
        } else {
            for (AttributeSink* sink : attrSinks) {
                sink->graphAttributeAdded(sourceId, timeId, attribute, newValue);
            }
        }
        break;
    case AttributeChangeEvent::REMOVE:
        if (eltType == ElementType::NODE) {
            for (AttributeSink* sink : attrSinks) {
                sink->nodeAttributeRemoved(sourceId, timeId, eltId, attribute);
            }
        } else if (eltType == ElementType::EDGE) {
            for (AttributeSink* sink : attrSinks) {
                sink->edgeAttributeRemoved(sourceId, timeId, eltId, attribute);
            }
        } else {
            for (AttributeSink* sink : attrSinks) {
                sink->graphAttributeRemoved(sourceId, timeId, attribute);
            }
        }
        break;
    case AttributeChangeEvent::CHANGE:
        if (eltType == ElementType::NODE) {
            for (AttributeSink* sink : attrSinks) {
                sink->nodeAttributeChanged(sourceId, timeId, eltId, attribute, oldValue, newValue);
            }
        } else if (eltType == ElementType::EDGE) {
            for (AttributeSink* sink : attrSinks) {
                sink->edgeAttributeChanged(sourceId, timeId, eltId, attribute, oldValue, newValue);
            }
        } else {
            for (AttributeSink* sink : attrSinks) {
                sink->graphAttributeChanged(sourceId, timeId, attribute, oldValue, newValue);
            }
        }
        break;
    }
}

// Implementations for templated events
template <typename T>
SourceBase::AddToListEvent<T>::AddToListEvent(std::vector<T*>& list, T* obj)
    : GraphEvent("", -1), list(list), obj(obj) {}

template <typename T>
void SourceBase::AddToListEvent<T>::trigger() {
    list.push_back(obj);
}

template <typename T>
SourceBase::RemoveFromListEvent<T>::RemoveFromListEvent(std::vector<T*>& list, T* obj)
    : GraphEvent("", -1), list(list), obj(obj) {}

template <typename T>
void SourceBase::RemoveFromListEvent<T>::trigger() {
    list.erase(std::remove(list.begin(), list.end(), obj), list.end());
}

template <typename T>
SourceBase::ClearListEvent<T>::ClearListEvent(std::vector<T*>& list)
    : GraphEvent("", -1), list(list) {}

template <typename T>
void SourceBase::ClearListEvent<T>::trigger() {
    list.clear();
}
