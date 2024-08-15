#ifndef SINK_HPP
#define SINK_HPP

#include "AttributeSink.hpp"
#include "ElementSink.hpp"

/**
 * Sink interface for graph events.
 *
 * A Sink is something that can receive graph events. It is used to transform
 * these events into another form, such as a file, a network stream, a visualization,
 * an algorithm, a metric, etc.
 *
 * This interface combines the functionality of both AttributeSink and ElementSink.
 *
 * @see Source
 * @see Pipe
 * @see AttributeSink
 * @see ElementSink
 */
class Sink : public AttributeSink, public ElementSink {
public:
    virtual ~Sink() = default;
};

#endif // SINK_HPP
