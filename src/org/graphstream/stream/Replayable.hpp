#ifndef REPLAYABLE_HPP
#define REPLAYABLE_HPP

#include "Source.hpp"
#include "Sink.hpp"

/**
 * Interface for sources that can be replayed.
 * 
 * This is useful when you connect a sink to a source and need to get information
 * about the current state of the dynamic graph.
 */
class Replayable {
public:
    virtual ~Replayable() = default;

    /**
     * Get a controller to replay the graph.
     *
     * @return a new replay controller
     */
    virtual std::shared_ptr<Controller> getReplayController() = 0;

    /**
     * A controller used to replay a source. Controllers should be used as sources
     * by adding sinks to them. When sinks are set, a call to replay() sends
     * events describing the current state of the original source to sinks.
     */
    class Controller : public Source {
    public:
        virtual ~Controller() = default;

        /**
         * Replay events describing the current state of the object being built by the
         * source.
         */
        virtual void replay() = 0;

        /**
         * Same as replay() but allows setting the ID of the source sent in events.
         *
         * @param sourceId ID of the event source
         */
        virtual void replay(const std::string& sourceId) = 0;
    };

    /**
     * Util method to replay a replayable source into a sink.
     *
     * @param source a source implementing the Replayable interface
     * @param sink sink which will receive the events produced by the replay
     */
    static void replay(std::shared_ptr<Replayable> source, std::shared_ptr<Sink> sink) {
        auto controller = source->getReplayController();
        controller->addSink(sink);
        controller->replay();
        controller->removeSink(sink);
    }

    /**
     * Same as replay(Replayable, Sink) but the first parameter is just a Source
     * and it will be replayed only if the Replayable interface is implemented.
     *
     * @param source a source
     * @param sink sink which will receive the events produced by the replay
     */
    static void tryReplay(std::shared_ptr<Source> source, std::shared_ptr<Sink> sink) {
        if (auto replayableSource = std::dynamic_pointer_cast<Replayable>(source)) {
            replay(replayableSource, sink);
        }
    }
};

#endif // REPLAYABLE_HPP
