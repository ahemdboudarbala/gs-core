#include "Timeline.hpp"
#include "Graph.hpp"
#include "GraphDiff.hpp"
#include "Sink.hpp"
#include "PipeBase.hpp"

Timeline::Timeline()
    : changed(false), seeker(0) {
    diffs.reserve(10);  // Preallocate some space for diffs
    connector = std::make_unique<Connector>();
    pipe = std::make_unique<PipeBase>();
}

void Timeline::reset() {
    // Implementation for reset
}

void Timeline::play(double from, double to) {
    play(from, to, pipe.get());
}

void Timeline::play(double from, double to, Sink* sink) {
    if (diffs.empty())
        return;

    if (from > to) {
        int i = diffs.size() - 1, j;

        while (i > 0 && diffs[i].step > from)
            --i;

        j = i;

        while (j > 0 && diffs[j].step >= to)
            --j;

        for (int k = i; k >= j; --k)
            diffs[k].diff->reverse(sink);
    } else {
        int i = 0, j;

        while (i < diffs.size() - 1 && diffs[i].step < from)
            ++i;

        j = i;

        while (j < diffs.size() - 1 && diffs[j].step <= to)
            ++j;

        for (int k = i; k <= j; ++k)
            diffs[k].diff->apply(sink);
    }
}

void Timeline::play() {
    play(initialGraph->getStep(), currentGraph->getStep());
}

void Timeline::play(Sink* sink) {
    play(initialGraph->getStep(), currentGraph->getStep(), sink);
}

void Timeline::playback() {
    play(currentGraph->getStep(), initialGraph->getStep());
}

void Timeline::playback(Sink* sink) {
    play(currentGraph->getStep(), initialGraph->getStep(), sink);
}

void Timeline::seek(int i) {
    seeker = i;
}

void Timeline::seekStart() {
    seeker = 0;
}

void Timeline::seekEnd() {
    seeker = diffs.size();
}

bool Timeline::hasNext() const {
    return seeker < diffs.size();
}

void Timeline::next() {
    if (seeker >= diffs.size())
        return;

    diffs[seeker++].diff->apply(pipe.get());
}

bool Timeline::hasPrevious() const {
    return seeker > 0;
}

void Timeline::previous() {
    if (seeker <= 0)
        return;

    diffs[--seeker].diff->reverse(pipe.get());
}

void Timeline::begin(Source* source) {
    initialGraph = std::make_shared<Graph>("initial");
    currentGraph = std::make_shared<Graph>("initial");
    begin();
}

void Timeline::begin(Graph* source) {
    initialGraph = source->clone();
    currentGraph = source;
    begin();
}

void Timeline::begin() {
    currentGraph->addSink(connector.get());
    pushDiff();
}

void Timeline::end() {
    if (currentDiff) {
        currentDiff->end();
        diffs.emplace_back(currentGraph->getStep(), currentDiff);
    }

    currentGraph->removeSink(connector.get());
    currentGraph = currentGraph->clone();
}

void Timeline::pushDiff() {
    if (currentDiff) {
        currentDiff->end();
        diffs.emplace_back(currentGraph->getStep(), currentDiff);
    }

    currentDiff = std::make_shared<GraphDiff>();
    currentDiff->start(currentGraph.get());
}

void Timeline::addSink(Sink* sink) {
    pipe->addSink(sink);
}

void Timeline::removeSink(Sink* sink) {
    pipe->removeSink(sink);
}

void Timeline::addAttributeSink(AttributeSink* sink) {
    pipe->addAttributeSink(sink);
}

void Timeline::removeAttributeSink(AttributeSink* sink) {
    pipe->removeAttributeSink(sink);
}

void Timeline::addElementSink(ElementSink* sink) {
    pipe->addElementSink(sink);
}

void Timeline::removeElementSink(ElementSink* sink) {
    pipe->removeElementSink(sink);
}

void Timeline::clearElementSinks() {
    pipe->clearElementSinks();
}

void Timeline::clearAttributeSinks() {
    pipe->clearAttributeSinks();
}

void Timeline::clearSinks() {
    pipe->clearSinks();
}

Timeline::TimelineIterator Timeline::begin() {
    return TimelineIterator(*this, 0);
}

Timeline::TimelineIterator Timeline::end() {
    return TimelineIterator(*this, diffs.size());
}

// Timeline::TimelineIterator implementation
Timeline::TimelineIterator::TimelineIterator(Timeline& timeline, int index)
    : timeline(timeline), index(index) {
    current = timeline.initialGraph->clone();
}

bool Timeline::TimelineIterator::operator!=(const TimelineIterator& other) const {
    return index != other.index;
}

const Graph& Timeline::TimelineIterator::operator*() const {
    return *current;
}

Timeline::TimelineIterator& Timeline::TimelineIterator::operator++() {
    if (index < timeline.diffs.size()) {
        timeline.diffs[index++].diff->apply(current.get());
    }
    return *this;
}

#endif // TIMELINE_HPP
