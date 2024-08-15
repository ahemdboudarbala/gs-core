#ifndef BYTE_ENCODER_HPP
#define BYTE_ENCODER_HPP

#include <memory>
#include <vector>
#include "Sink.hpp"

class ByteEncoder : public Sink {
public:
    virtual ~ByteEncoder() = default;

    /**
     * Add a new transport to this encoder.
     *
     * @param transport the new transport
     */
    virtual void addTransport(std::shared_ptr<class Transport> transport) = 0;

    /**
     * Remove an existing transport from this encoder.
     *
     * @param transport the transport to remove
     */
    virtual void removeTransport(std::shared_ptr<class Transport> transport) = 0;

    /**
     * Transport interface for sending encoded events.
     */
    class Transport {
    public:
        virtual ~Transport() = default;

        /**
         * Called by the encoder once an event has been encoded.
         * 
         * @param buffer buffer that has to be transported
         */
        virtual void send(const std::vector<uint8_t>& buffer) = 0;
    };
};

#endif // BYTE_ENCODER_HPP
