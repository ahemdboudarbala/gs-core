#ifndef BYTE_FACTORY_HPP
#define BYTE_FACTORY_HPP

#include <memory>
#include "ByteEncoder.hpp"
#include "ByteDecoder.hpp"

class ByteFactory {
public:
    virtual ~ByteFactory() = default;

    /**
     * Create a new binary encoder.
     *
     * @return the new created encoder
     */
    virtual std::shared_ptr<ByteEncoder> createByteEncoder() = 0;

    /**
     * Create a new binary decoder.
     *
     * @return the new created decoder
     */
    virtual std::shared_ptr<ByteDecoder> createByteDecoder() = 0;
};

#endif // BYTE_FACTORY_HPP
