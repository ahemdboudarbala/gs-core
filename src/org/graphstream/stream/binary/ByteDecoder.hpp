#ifndef BYTE_DECODER_HPP
#define BYTE_DECODER_HPP

#include <memory>
#include <vector>
#include "Source.hpp"

class ByteDecoder : public Source {
public:
    virtual ~ByteDecoder() = default;

    /**
     * Decode the buffer, and dispatch the corresponding event.
     * 
     * @param buffer event as a binary buffer
     */
    virtual void decode(const std::vector<uint8_t>& buffer) = 0;

    /**
     * Tells if the buffer contains an event that is ready to be dispatched.
     * The bytes considered should be from 0 to the buffer's current position.
     * 
     * @param buffer event as a binary buffer
     * @return true if the buffer contains an event
     */
    virtual bool validate(const std::vector<uint8_t>& buffer) const = 0;
};

#endif // BYTE_DECODER_HPP
