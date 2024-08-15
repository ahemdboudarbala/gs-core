#ifndef SOURCE_HPP
#define SOURCE_HPP

#include "Sink.hpp"
#include "AttributeSink.hpp"
#include "ElementSink.hpp"

class Source {
public:
    virtual ~Source() = default;

    // Add a sink for all graph events (attributes and graph elements) coming from this source.
    virtual void addSink(Sink* sink) = 0;

    // Remove a sink.
    virtual void removeSink(Sink* sink) = 0;

    // Add a sink for attribute events only.
    virtual void addAttributeSink(AttributeSink* sink) = 0;

    // Remove an attribute sink.
    virtual void removeAttributeSink(AttributeSink* sink) = 0;

    // Add a sink for elements events only.
    virtual void addElementSink(ElementSink* sink) = 0;

    // Remove an element sink.
    virtual void removeElementSink(ElementSink* sink) = 0;

    // Remove all listener element sinks.
    virtual void clearElementSinks() = 0;

    // Remove all listener attribute sinks.
    virtual void clearAttributeSinks() = 0;

    // Remove all listener sinks.
    virtual void clearSinks() = 0;
};

#endif // SOURCE_HPP
