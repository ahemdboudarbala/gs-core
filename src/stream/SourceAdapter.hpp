#ifndef SOURCE_ADAPTER_HPP
#define SOURCE_ADAPTER_HPP

#include "Source.hpp"
#include "AttributeSink.hpp"
#include "ElementSink.hpp"
#include "Sink.hpp"

class SourceAdapter : public Source {
public:
    // Virtual methods inherited from Source
    virtual void addAttributeSink(AttributeSink* sink) override;
    virtual void addElementSink(ElementSink* sink) override;
    virtual void addSink(Sink* sink) override;
    virtual void removeAttributeSink(AttributeSink* sink) override;
    virtual void removeElementSink(ElementSink* sink) override;
    virtual void removeSink(Sink* sink) override;
    virtual void clearAttributeSinks() override;
    virtual void clearElementSinks() override;
    virtual void clearSinks() override;
};

#endif // SOURCE_ADAPTER_HPP
