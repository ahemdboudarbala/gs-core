#ifndef NETSTREAMUTILS_HPP
#define NETSTREAMUTILS_HPP

#include "NetStreamEncoder.hpp"
#include "NetStreamDecoder.hpp"
#include "ByteFactory.hpp"
#include "NetStreamConstants.hpp"

#include <any>
#include <string>
#include <vector>
#include <memory>
#include <typeinfo>
#include <iostream>
#include <algorithm>
#include <cstring>

class NetStreamUtils {
public:
    static std::unique_ptr<ByteFactory> getDefaultNetStreamFactory() {
        return std::make_unique<ByteFactory>([]() {
            return std::make_unique<NetStreamEncoder>();
        }, []() {
            return std::make_unique<NetStreamDecoder>();
        });
    }

    static int getType(const std::any& value);
    static int getVarintSize(long data);
    static void putVarint(std::vector<uint8_t>& buffer, long number, int byteSize);

    static std::vector<uint8_t> encodeValue(const std::any& in, int valueType);
    static std::vector<uint8_t> encodeUnsignedVarint(long data);
    static std::vector<uint8_t> encodeVarint(long data);
    static std::vector<uint8_t> encodeString(const std::string& str);
    static std::vector<uint8_t> encodeDoubleArray(const std::vector<double>& data);
    static std::vector<uint8_t> encodeStringArray(const std::vector<std::string>& data);
    static std::vector<uint8_t> encodeDouble(double value);
    static std::vector<uint8_t> encodeFloatArray(const std::vector<float>& data);
    static std::vector<uint8_t> encodeFloat(float value);
    static std::vector<uint8_t> encodeLongArray(const std::vector<long>& data);
    static std::vector<uint8_t> encodeLong(long value);
    static std::vector<uint8_t> encodeIntArray(const std::vector<int>& data);
    static std::vector<uint8_t> encodeInt(int value);
    static std::vector<uint8_t> encodeShortArray(const std::vector<short>& data);
    static std::vector<uint8_t> encodeShort(short value);
    static std::vector<uint8_t> encodeByteArray(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> encodeByte(uint8_t value);
    static std::vector<uint8_t> encodeBooleanArray(const std::vector<bool>& data);
    static std::vector<uint8_t> encodeBoolean(bool value);
    static std::vector<uint8_t> encodeVarintArray(const std::vector<long>& data);

    static int decodeType(const std::vector<uint8_t>& buffer, size_t& offset);
    static std::any decodeValue(const std::vector<uint8_t>& buffer, size_t& offset, int valueType);
    static std::string decodeString(const std::vector<uint8_t>& buffer, size_t& offset);
    static std::vector<std::string> decodeStringArray(const std::vector<uint8_t>& buffer, size_t& offset);
    static bool decodeBoolean(const std::vector<uint8_t>& buffer, size_t& offset);
    static uint8_t decodeByte(const std::vector<uint8_t>& buffer, size_t& offset);
    static long decodeUnsignedVarint(const std::vector<uint8_t>& buffer, size_t& offset);
    static long decodeVarint(const std::vector<uint8_t>& buffer, size_t& offset);
    static short decodeShort(const std::vector<uint8_t>& buffer, size_t& offset);
    static int decodeInt(const std::vector<uint8_t>& buffer, size_t& offset);
    static long decodeLong(const std::vector<uint8_t>& buffer, size_t& offset);
    static float decodeFloat(const std::vector<uint8_t>& buffer, size_t& offset);
    static double decodeDouble(const std::vector<uint8_t>& buffer, size_t& offset);
    static std::vector<int> decodeIntArray(const std::vector<uint8_t>& buffer, size_t& offset);
    static std::vector<bool> decodeBooleanArray(const std::vector<uint8_t>& buffer, size_t& offset);
    static std::vector<uint8_t> decodeByteArray(const std::vector<uint8_t>& buffer, size_t& offset);
    static std::vector<double> decodeDoubleArray(const std::vector<uint8_t>& buffer, size_t& offset);
    static std::vector<float> decodeFloatArray(const std::vector<uint8_t>& buffer, size_t& offset);
    static std::vector<long> decodeLongArray(const std::vector<uint8_t>& buffer, size_t& offset);
    static std::vector<short> decodeShortArray(const std::vector<uint8_t>& buffer, size_t& offset);
};

#endif // NETSTREAMUTILS_HPP
