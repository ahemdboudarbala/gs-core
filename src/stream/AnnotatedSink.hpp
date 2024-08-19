#ifndef ANNOTATED_SINK_HPP
#define ANNOTATED_SINK_HPP

#include "Sink.hpp"
#include "SourceBase.hpp"
#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <stdexcept>

/**
 * A sink that allows binding between attribute modifications and method calls.
 */
class AnnotatedSink : public Sink {
public:
    enum class ElementType {
        GRAPH,
        NODE,
        EDGE
    };

    /**
     * Annotation used to bind an event to a method.
     */
    struct Bind {
        std::string value;
        ElementType type = ElementType::GRAPH;
    };

    AnnotatedSink();

    void edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, 
                            const std::string& attribute, const std::any& value) override;

    void edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, 
                              const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;

    void edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, 
                              const std::string& attribute) override;

    void graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, 
                             const std::any& value) override;

    void graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, 
                               const std::any& oldValue, const std::any& newValue) override;

    void graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) override;

    void nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, 
                            const std::string& attribute, const std::any& value) override;

    void nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, 
                              const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;

    void nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, 
                              const std::string& attribute) override;

    // Other Sink methods
    void edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, 
                   const std::string& fromNodeId, const std::string& toNodeId, bool directed) override;

    void edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) override;

    void graphCleared(const std::string& sourceId, long timeId) override;

    void nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) override;

    void nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) override;

    void stepBegins(const std::string& sourceId, long timeId, double step) override;

protected:
    void invoke(const std::function<void(const std::any&)>& method, const std::any& arg);

    void bindMethod(ElementType type, const std::string& attribute, const std::function<void(const std::any&)>& method);

private:
    std::map<ElementType, std::map<std::string, std::function<void(const std::any&)>>> methods;
};

#endif // ANNOTATED_SINK_HPP
