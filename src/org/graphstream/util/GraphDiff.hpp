#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>
#include <algorithm>
#include "Graph.hpp"
#include "Element.hpp"
#include "Node.hpp"
#include "Edge.hpp"
#include "Sink.hpp"
#include "AdjacencyListGraph.hpp"

class GraphDiff {
public:
    enum class ElementType {
        NODE, EDGE, GRAPH
    };

    GraphDiff();
    GraphDiff(Graph* g1, Graph* g2);

    void start(Graph* g);
    void end();
    void reset();
    void apply(Sink* g1);
    void apply(const std::string& sourceId, Sink* g1);
    void reverse(Sink* g2);
    void reverse(const std::string& sourceId, Sink* g2);

    std::string toString() const;

private:
    class Event {
    public:
        virtual void apply(const std::string& sourceId, long timeId, Sink* g) = 0;
        virtual void reverse(const std::string& sourceId, long timeId, Sink* g) = 0;
        virtual std::string toString() const = 0;
        virtual ~Event() = default;
    };

    void attributeDiff(ElementType type, Element* e1, Element* e2);

    class NodeAdded : public Event {
    public:
        NodeAdded(const std::string& nodeId);
        void apply(const std::string& sourceId, long timeId, Sink* g) override;
        void reverse(const std::string& sourceId, long timeId, Sink* g) override;
        std::string toString() const override;

    private:
        std::string nodeId;
    };

    class NodeRemoved : public NodeAdded {
    public:
        NodeRemoved(const std::string& nodeId);
        void apply(const std::string& sourceId, long timeId, Sink* g) override;
        void reverse(const std::string& sourceId, long timeId, Sink* g) override;
        std::string toString() const override;
    };

    class ElementEvent : public Event {
    public:
        ElementEvent(ElementType type, const std::string& elementId);

    protected:
        Element* getElement(Graph* g);
        std::string toStringHeader() const;
        std::string toStringValue(const void* value) const;

        ElementType type;
        std::string elementId;
    };

    class AttributeAdded : public ElementEvent {
    public:
        AttributeAdded(ElementType type, const std::string& elementId, const std::string& attrId, const void* value);
        void apply(const std::string& sourceId, long timeId, Sink* g) override;
        void reverse(const std::string& sourceId, long timeId, Sink* g) override;
        std::string toString() const override;

    private:
        std::string attrId;
        const void* value;
    };

    class AttributeChanged : public ElementEvent {
    public:
        AttributeChanged(ElementType type, const std::string& elementId, const std::string& attrId, const void* newValue, const void* oldValue);
        void apply(const std::string& sourceId, long timeId, Sink* g) override;
        void reverse(const std::string& sourceId, long timeId, Sink* g) override;
        std::string toString() const override;

    private:
        std::string attrId;
        const void* newValue;
        const void* oldValue;
    };

    class AttributeRemoved : public ElementEvent {
    public:
        AttributeRemoved(ElementType type, const std::string& elementId, const std::string& attrId, const void* oldValue);
        void apply(const std::string& sourceId, long timeId, Sink* g) override;
        void reverse(const std::string& sourceId, long timeId, Sink* g) override;
        std::string toString() const override;

    private:
        std::string attrId;
        const void* oldValue;
    };

    class EdgeAdded : public Event {
    public:
        EdgeAdded(const std::string& edgeId, const std::string& source, const std::string& target, bool directed);
        void apply(const std::string& sourceId, long timeId, Sink* g) override;
        void reverse(const std::string& sourceId, long timeId, Sink* g) override;
        std::string toString() const override;

    private:
        std::string edgeId;
        std::string source;
        std::string target;
        bool directed;
    };

    class EdgeRemoved : public EdgeAdded {
    public:
        EdgeRemoved(const std::string& edgeId, const std::string& source, const std::string& target, bool directed);
        void apply(const std::string& sourceId, long timeId, Sink* g) override;
        void reverse(const std::string& sourceId, long timeId, Sink* g) override;
        std::string toString() const override;
    };

    class StepBegins : public Event {
    public:
        StepBegins(double oldStep, double newStep);
        void apply(const std::string& sourceId, long timeId, Sink* g) override;
        void reverse(const std::string& sourceId, long timeId, Sink* g) override;
        std::string toString() const override;

    private:
        double newStep;
        double oldStep;
    };

    class GraphCleared : public Event {
    public:
        GraphCleared(Graph* g);
        void apply(const std::string& sourceId, long timeId, Sink* g) override;
        void reverse(const std::string& sourceId, long timeId, Sink* g) override;
        std::string toString() const override;

    private:
        std::vector<char> data;
    };

    class Bridge : public Sink {
    public:
        Bridge(Graph* g, GraphDiff* diff);
        void end();

        void graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, const void* value) override;
        void graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, const void* oldValue, const void* newValue) override;
        void graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) override;

        void nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const void* value) override;
        void nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const void* oldValue, const void* newValue) override;
        void nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute) override;

        void edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const void* value) override;
        void edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const void* oldValue, const void* newValue) override;
        void edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute) override;

        void nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) override;
        void nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) override;

        void edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) override;
        void edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) override;

        void graphCleared(const std::string& sourceId, long timeId) override;
        void stepBegins(const std::string& sourceId, long timeId, double step) override;

    private:
        Graph* g;
        GraphDiff* diff;
    };

    std::vector<std::shared_ptr<Event>> events;
    std::unique_ptr<Bridge> bridge;
};
