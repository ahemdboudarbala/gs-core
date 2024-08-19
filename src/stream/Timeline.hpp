#ifndef TIMELINE_HPP
#define TIMELINE_HPP

#include <vector>
#include <memory>
#include <iterator>

// Forward declarations
class Graph;
class Sink;
class GraphDiff;

class Timeline : public Source, public Replayable {
public:
    Timeline();
    void reset();
    void play(double from, double to);
    void play(double from, double to, Sink* sink);
    void play();
    void play(Sink* sink);
    void playback();
    void playback(Sink* sink);
    void seek(int i);
    void seekStart();
    void seekEnd();
    bool hasNext() const;
    void next();
    bool hasPrevious() const;
    void previous();
    void begin(Source* source);
    void begin(Graph* source);
    void end();
    
    void addSink(Sink* sink);
    void removeSink(Sink* sink);
    void addAttributeSink(AttributeSink* sink);
    void removeAttributeSink(AttributeSink* sink);
    void addElementSink(ElementSink* sink);
    void removeElementSink(ElementSink* sink);
    void clearElementSinks();
    void clearAttributeSinks();
    void clearSinks();
    
    class Iterator;
    Iterator begin();
    Iterator end();

protected:
    class StepDiff {
    public:
        StepDiff(double step, std::shared_ptr<GraphDiff> diff);
        double step;
        std::shared_ptr<GraphDiff> diff;
    };
    
    class Connector : public SinkAdapter {
    public:
        void stepBegins(const std::string& sourceId, long timeId, double step) override;
    };

    class TimelineReplayController : public PipeBase, public Controller {
    public:
        void replay();
        void replay(const std::string& sourceId);
    };

    void pushDiff();

private:
    std::vector<StepDiff> diffs;
    bool changed;
    std::shared_ptr<Graph> initialGraph;
    std::shared_ptr<Graph> currentGraph;
    std::shared_ptr<GraphDiff> currentDiff;
    std::unique_ptr<Connector> connector;
    std::unique_ptr<PipeBase> pipe;
    int seeker;

    class TimelineIterator : public std::iterator<std::input_iterator_tag, Graph> {
    public:
        TimelineIterator(Timeline& timeline, int index);
        bool operator!=(const TimelineIterator& other) const;
        const Graph& operator*() const;
        TimelineIterator& operator++();

    private:
        Timeline& timeline;
        int index;
        std::shared_ptr<Graph> current;
    };
};

#endif // TIMELINE_HPP
