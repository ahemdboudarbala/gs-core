#include "AnnotatedSink.hpp"

AnnotatedSink::AnnotatedSink() {
    // In the C++ version, methods need to be bound explicitly using bindMethod
}

void AnnotatedSink::invoke(const std::function<void(const std::any&)>& method, const std::any& arg) {
    try {
        method(arg);
    } catch (const std::bad_any_cast& e) {
        std::cerr << "Bad any_cast: " << e.what() << std::endl;
    }
}

void AnnotatedSink::bindMethod(ElementType type, const std::string& attribute, const std::function<void(const std::any&)>& method) {
    methods[type][attribute] = method;
}

void AnnotatedSink::edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, 
                                       const std::string& attribute, const std::any& value) {
    auto it = methods[ElementType::EDGE].find(attribute);
    if (it != methods[ElementType::EDGE].end()) {
        invoke(it->second, value);
    }
}

void AnnotatedSink::edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, 
                                         const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    auto it = methods[ElementType::EDGE].find(attribute);
    if (it != methods[ElementType::EDGE].end()) {
        invoke(it->second, newValue);
    }
}

void AnnotatedSink::edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, 
                                         const std::string& attribute) {
    auto it = methods[ElementType::EDGE].find(attribute);
    if (it != methods[ElementType::EDGE].end()) {
        invoke(it->second, std::any());
    }
}

void AnnotatedSink::graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, 
                                        const std::any& value) {
    auto it = methods[ElementType::GRAPH].find(attribute);
    if (it != methods[ElementType::GRAPH].end()) {
        invoke(it->second, value);
    }
}

void AnnotatedSink::graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, 
                                          const std::any& oldValue, const std::any& newValue) {
    auto it = methods[ElementType::GRAPH].find(attribute);
    if (it != methods[ElementType::GRAPH].end()) {
        invoke(it->second, newValue);
    }
}

void AnnotatedSink::graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) {
    auto it = methods[ElementType::GRAPH].find(attribute);
    if (it != methods[ElementType::GRAPH].end()) {
        invoke(it->second, std::any());
    }
}

void AnnotatedSink::nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, 
                                       const std::string& attribute, const std::any& value) {
    auto it = methods[ElementType::NODE].find(attribute);
    if (it != methods[ElementType::NODE].end()) {
        invoke(it->second, value);
    }
}

void AnnotatedSink::nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, 
                                         const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    auto it = methods[ElementType::NODE].find(attribute);
    if (it != methods[ElementType::NODE].end()) {
        invoke(it->second, newValue);
    }
}

void AnnotatedSink::nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, 
                                         const std::string& attribute) {
    auto it = methods[ElementType::NODE].find(attribute);
    if (it != methods[ElementType::NODE].end()) {
        invoke(it->second, std::any());
    }
}

// Empty implementations for other Sink methods
void AnnotatedSink::edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, 
                              const std::string& fromNodeId, const std::string& toNodeId, bool directed) {}

void AnnotatedSink::edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) {}

void AnnotatedSink::graphCleared(const std::string& sourceId, long timeId) {}

void AnnotatedSink::nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) {}

void AnnotatedSink::nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) {}

void AnnotatedSink::stepBegins(const std::string& sourceId, long timeId, double step) {}
