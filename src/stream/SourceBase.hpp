#ifndef SOURCE_BASE_HPP
#define SOURCE_BASE_HPP

#include <string>
#include <vector>
#include <list>
#include <memory>

// Forward declarations
class AttributeSink;
class ElementSink;
class Sink;
class SourceTime;

enum class ElementType {
    NODE, EDGE, GRAPH
};

class SourceBase {
public:
    SourceBase();
    SourceBase(const std::string& sourceId);

    // Access
    const std::vector<AttributeSink*>& attributeSinks() const;
    const std::vector<ElementSink*>& elementSinks() const;

    // Commands
    void addSink(Sink* sink);
    void addAttributeSink(AttributeSink* sink);
    void addElementSink(ElementSink* sink);
    void clearSinks();
    void clearElementSinks();
    void clearAttributeSinks();
    void removeSink(Sink* sink);
    void removeAttributeSink(AttributeSink* sink);
    void removeElementSink(ElementSink* sink);

    void sendGraphCleared(const std::string& sourceId);
    void sendGraphCleared(const std::string& sourceId, long timeId);
    void sendStepBegins(const std::string& sourceId, double step);
    void sendStepBegins(const std::string& sourceId, long timeId, double step);
    void sendNodeAdded(const std::string& sourceId, const std::string& nodeId);
    void sendNodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId);
    void sendNodeRemoved(const std::string& sourceId, const std::string& nodeId);
    void sendNodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId);
    void sendEdgeAdded(const std::string& sourceId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed);
    void sendEdgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed);
    void sendEdgeRemoved(const std::string& sourceId, const std::string& edgeId);
    void sendEdgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId);
    void sendEdgeAttributeAdded(const std::string& sourceId, const std::string& edgeId, const std::string& attribute, const std::any& value);
    void sendEdgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& value);
    void sendEdgeAttributeChanged(const std::string& sourceId, const std::string& edgeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue);
    void sendEdgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue);
    void sendEdgeAttributeRemoved(const std::string& sourceId, const std::string& edgeId, const std::string& attribute);
    void sendEdgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute);
    void sendGraphAttributeAdded(const std::string& sourceId, const std::string& attribute, const std::any& value);
    void sendGraphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, const std::any& value);
    void sendGraphAttributeChanged(const std::string& sourceId, const std::string& attribute, const std::any& oldValue, const std::any& newValue);
    void sendGraphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue);
    void sendGraphAttributeRemoved(const std::string& sourceId, const std::string& attribute);
    void sendGraphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute);
    void sendNodeAttributeAdded(const std::string& sourceId, const std::string& nodeId, const std::string& attribute, const std::any& value);
    void sendNodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& value);
    void sendNodeAttributeChanged(const std::string& sourceId, const std::string& nodeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue);
    void sendNodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue);
    void sendNodeAttributeRemoved(const std::string& sourceId, const std::string& nodeId, const std::string& attribute);
    void sendNodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute);

    void sendAttributeChangedEvent(const std::string& sourceId, const std::string& eltId, ElementType eltType, const std::string& attribute, AttributeChangeEvent event, const std::any& oldValue, const std::any& newValue);
    void sendAttributeChangedEvent(const std::string& sourceId, long timeId, const std::string& eltId, ElementType eltType, const std::string& attribute, AttributeChangeEvent event, const std::any& oldValue, const std::any& newValue);

protected:
    void manageEvents();

    class GraphEvent {
    public:
        GraphEvent(const std::string& sourceId, long timeId);
        virtual void trigger() = 0;

    protected:
        std::string sourceId;
        long timeId;
    };

    class AfterEdgeAddEvent : public GraphEvent {
    public:
        AfterEdgeAddEvent(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed);
        void trigger() override;

    private:
        std::string edgeId;
        std::string fromNodeId;
        std::string toNodeId;
        bool directed;
    };

    class BeforeEdgeRemoveEvent : public GraphEvent {
    public:
        BeforeEdgeRemoveEvent(const std::string& sourceId, long timeId, const std::string& edgeId);
        void trigger() override;

    private:
        std::string edgeId;
    };

    class AfterNodeAddEvent : public GraphEvent {
    public:
        AfterNodeAddEvent(const std::string& sourceId, long timeId, const std::string& nodeId);
        void trigger() override;

    private:
        std::string nodeId;
    };

    class BeforeNodeRemoveEvent : public GraphEvent {
    public:
        BeforeNodeRemoveEvent(const std::string& sourceId, long timeId, const std::string& nodeId);
        void trigger() override;

    private:
        std::string nodeId;
    };

    class BeforeGraphClearEvent : public GraphEvent {
    public:
        BeforeGraphClearEvent(const std::string& sourceId, long timeId);
        void trigger() override;
    };

    class StepBeginsEvent : public GraphEvent {
    public:
        StepBeginsEvent(const std::string& sourceId, long timeId, double step);
        void trigger() override;

    private:
        double step;
    };

    class AttributeChangedEvent : public GraphEvent {
    public:
        AttributeChangedEvent(const std::string& sourceId, long timeId, const std::string& eltId, ElementType eltType, const std::string& attribute, AttributeChangeEvent event, const std::any& oldValue, const std::any& newValue);
        void trigger() override;

    private:
        ElementType eltType;
        std::string eltId;
        std::string attribute;
        AttributeChangeEvent event;
        std::any oldValue;
        std::any newValue;
    };

    template <typename T>
    class AddToListEvent : public GraphEvent {
    public:
        AddToListEvent(std::vector<T*>& list, T* obj);
        void trigger() override;

    private:
        std::vector<T*>& list;
        T* obj;
    };

    template <typename T>
    class RemoveFromListEvent : public GraphEvent {
    public:
        RemoveFromListEvent(std::vector<T*>& list, T* obj);
        void trigger() override;

    private:
        std::vector<T*>& list;
        T* obj;
    };

    template <typename T>
    class ClearListEvent : public GraphEvent {
    public:
        ClearListEvent(std::vector<T*>& list);
        void trigger() override;

    private:
        std::vector<T*>& list;
    };

    std::vector<AttributeSink*> attrSinks;
    std::vector<ElementSink*> eltsSinks;
    std::list<std::unique_ptr<GraphEvent>> eventQueue;
    bool eventProcessing;
    std::string sourceId;
    std::unique_ptr<SourceTime> sourceTime;
};

#endif // SOURCE_BASE_HPP
