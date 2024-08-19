#include "NetStreamUtils.hpp"
#include "NetStreamConstants.hpp"
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>

int NetStreamUtils::getType(const std::any& value) {
    if (!value.has_value()) {
        return NetStreamConstants::TYPE_NULL;
    }

    const std::type_info& type = value.type();

    if (type == typeid(bool)) {
        return NetStreamConstants::TYPE_BOOLEAN;
    } else if (type == typeid(std::vector<bool>)) {
        return NetStreamConstants::TYPE_BOOLEAN_ARRAY;
    } else if (type == typeid(uint8_t)) {
        return NetStreamConstants::TYPE_BYTE;
    } else if (type == typeid(std::vector<uint8_t>)) {
        return NetStreamConstants::TYPE_BYTE_ARRAY;
    } else if (type == typeid(short)) {
        return NetStreamConstants::TYPE_SHORT;
    } else if (type == typeid(std::vector<short>)) {
        return NetStreamConstants::TYPE_SHORT_ARRAY;
    } else if (type == typeid(int)) {
        return NetStreamConstants::TYPE_INT;
    } else if (type == typeid(std::vector<int>)) {
        return NetStreamConstants::TYPE_INT_ARRAY;
    } else if (type == typeid(long)) {
        return NetStreamConstants::TYPE_LONG;
    } else if (type == typeid(std::vector<long>)) {
        return NetStreamConstants::TYPE_LONG_ARRAY;
    } else if (type == typeid(float)) {
        return NetStreamConstants::TYPE_FLOAT;
    } else if (type == typeid(std::vector<float>)) {
        return NetStreamConstants::TYPE_FLOAT_ARRAY;
    } else if (type == typeid(double)) {
        return NetStreamConstants::TYPE_DOUBLE;
    } else if (type == typeid(std::vector<double>)) {
        return NetStreamConstants::TYPE_DOUBLE_ARRAY;
    } else if (type == typeid(std::string)) {
        return NetStreamConstants::TYPE_STRING;
    } else if (type == typeid(std::vector<std::string>)) {
        return NetStreamConstants::TYPE_STRING_ARRAY;
    } else {
        std::cerr << "Cannot determine type of " << type.name() << std::endl;
        return NetStreamConstants::TYPE_UNKNOWN;
    }
}

int NetStreamUtils::getVarintSize(long data) {
    if (data < (1L << 7)) return 1;
    if (data < (1L << 14)) return 2;
    if (data < (1L << 21)) return 3;
    if (data < (1L << 28)) return 4;
    if (data < (1L << 35)) return 5;
    if (data < (1L << 42)) return 6;
    if (data < (1L << 49)) return 7;
    if (data < (1L << 56)) return 8;
    return 9;
}

void NetStreamUtils::putVarint(std::vector<uint8_t>& buffer, long number, int byteSize) {
    for (int i = 0; i < byteSize; i++) {
        int head = (i == byteSize - 1) ? 0 : 128;
        long b = ((number >> (7 * i)) & 127) ^ head;
        buffer.push_back(static_cast<uint8_t>(b & 255));
    }
}

std::vector<uint8_t> NetStreamUtils::encodeValue(const std::any& in, int valueType) {
    if (valueType == NetStreamConstants::TYPE_BOOLEAN) {
        return encodeBoolean(std::any_cast<bool>(in));
    } else if (valueType == NetStreamConstants::TYPE_BOOLEAN_ARRAY) {
        return encodeBooleanArray(std::any_cast<std::vector<bool>>(in));
    } else if (valueType == NetStreamConstants::TYPE_BYTE) {
        return encodeByte(std::any_cast<uint8_t>(in));
    } else if (valueType == NetStreamConstants::TYPE_BYTE_ARRAY) {
        return encodeByteArray(std::any_cast<std::vector<uint8_t>>(in));
    } else if (valueType == NetStreamConstants::TYPE_SHORT) {
        return encodeShort(std::any_cast<short>(in));
    } else if (valueType == NetStreamConstants::TYPE_SHORT_ARRAY) {
        return encodeShortArray(std::any_cast<std::vector<short>>(in));
    } else if (valueType == NetStreamConstants::TYPE_INT) {
        return encodeInt(std::any_cast<int>(in));
    } else if (valueType == NetStreamConstants::TYPE_INT_ARRAY) {
        return encodeIntArray(std::any_cast<std::vector<int>>(in));
    } else if (valueType == NetStreamConstants::TYPE_LONG) {
        return encodeLong(std::any_cast<long>(in));
    } else if (valueType == NetStreamConstants::TYPE_LONG_ARRAY) {
        return encodeLongArray(std::any_cast<std::vector<long>>(in));
    } else if (valueType == NetStreamConstants::TYPE_FLOAT) {
        return encodeFloat(std::any_cast<float>(in));
    } else if (valueType == NetStreamConstants::TYPE_FLOAT_ARRAY) {
        return encodeFloatArray(std::any_cast<std::vector<float>>(in));
    } else if (valueType == NetStreamConstants::TYPE_DOUBLE) {
        return encodeDouble(std::any_cast<double>(in));
    } else if (valueType == NetStreamConstants::TYPE_DOUBLE_ARRAY) {
        return encodeDoubleArray(std::any_cast<std::vector<double>>(in));
    } else if (valueType == NetStreamConstants::TYPE_STRING) {
        return encodeString(std::any_cast<std::string>(in));
    } else if (valueType == NetStreamConstants::TYPE_STRING_ARRAY) {
        return encodeStringArray(std::any_cast<std::vector<std::string>>(in));
    } else if (valueType == NetStreamConstants::TYPE_NULL) {
        return {};
    }

    return {};
}

std::vector<uint8_t> NetStreamUtils::encodeUnsignedVarint(long data) {
    int size = getVarintSize(data);
    std::vector<uint8_t> buff(size);
    putVarint(buff, data, size);
    return buff;
}

std::vector<uint8_t> NetStreamUtils::encodeVarint(long data) {
    long zigzag = data >= 0 ? (data << 1) : ((std::abs(data) << 1) ^ 1);
    return encodeUnsignedVarint(zigzag);
}

std::vector<uint8_t> NetStreamUtils::encodeString(const std::string& str) {
    std::vector<uint8_t> data(str.begin(), str.end());
    std::vector<uint8_t> lenBuff = encodeUnsignedVarint(data.size());
    lenBuff.insert(lenBuff.end(), data.begin(), data.end());
    return lenBuff;
}

// Implement other encoding methods...

int NetStreamUtils::decodeType(const std::vector<uint8_t>& buffer, size_t& offset) {
    if (offset < buffer.size()) {
        return buffer[offset++];
    }
    return NetStreamConstants::TYPE_UNKNOWN;
}

std::any NetStreamUtils::decodeValue(const std::vector<uint8_t>& buffer, size_t& offset, int valueType) {
    if (valueType == NetStreamConstants::TYPE_BOOLEAN) {
        return decodeBoolean(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_BOOLEAN_ARRAY) {
        return decodeBooleanArray(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_BYTE) {
        return decodeByte(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_BYTE_ARRAY) {
        return decodeByteArray(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_SHORT) {
        return decodeShort(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_SHORT_ARRAY) {
        return decodeShortArray(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_INT) {
        return decodeInt(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_INT_ARRAY) {
        return decodeIntArray(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_LONG) {
        return decodeLong(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_LONG_ARRAY) {
        return decodeLongArray(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_FLOAT) {
        return decodeFloat(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_FLOAT_ARRAY) {
        return decodeFloatArray(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_DOUBLE) {
        return decodeDouble(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_DOUBLE_ARRAY) {
        return decodeDoubleArray(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_STRING) {
        return decodeString(buffer, offset);
    } else if (valueType == NetStreamConstants::TYPE_STRING_ARRAY) {
        return decodeStringArray(buffer, offset);
    }
    return {};
}

// Implement other decoding methods...

std::vector<uint8_t> NetStreamUtils::encodeBoolean(bool value) {
    return { static_cast<uint8_t>(value ? 1 : 0) };
}

std::vector<uint8_t> NetStreamUtils::encodeBooleanArray(const std::vector<bool>& value) {
    std::vector<uint8_t> buffer;
    int ssize = getVarintSize(value.size());
    buffer.reserve(ssize + value.size());
    putVarint(buffer, value.size(), ssize);
    for (bool b : value) {
        buffer.push_back(static_cast<uint8_t>(b ? 1 : 0));
    }
    return buffer;
}

std::vector<uint8_t> NetStreamUtils::encodeByte(uint8_t value) {
    return { value };
}

std::vector<uint8_t> NetStreamUtils::encodeByteArray(const std::vector<uint8_t>& value) {
    std::vector<uint8_t> buffer;
    int ssize = getVarintSize(value.size());
    buffer.reserve(ssize + value.size());
    putVarint(buffer, value.size(), ssize);
    buffer.insert(buffer.end(), value.begin(), value.end());
    return buffer;
}

// Implement other specific encoding methods...

std::any NetStreamUtils::decodeBoolean(const std::vector<uint8_t>& buffer, size_t& offset) {
    if (offset < buffer.size()) {
        return static_cast<bool>(buffer[offset++]);
    }
    return false;
}

std::any NetStreamUtils::decodeBooleanArray(const std::vector<uint8_t>& buffer, size_t& offset) {
    int len = static_cast<int>(decodeUnsignedVarint(buffer, offset));
    std::vector<bool> result(len);
    for (int i = 0; i < len; ++i) {
        result[i] = static_cast<bool>(buffer[offset++]);
    }
    return result;
}

std::any NetStreamUtils::decodeByte(const std::vector<uint8_t>& buffer, size_t& offset) {
    if (offset < buffer.size()) {
        return buffer[offset++];
    }
    return uint8_t(0);
}

std::any NetStreamUtils::decodeByteArray(const std::vector<uint8_t>& buffer, size_t& offset) {
    int len = static_cast<int>(decodeUnsignedVarint(buffer, offset));
    std::vector<uint8_t> result(len);
    for (int i = 0; i < len; ++i) {
        result[i] = buffer[offset++];
    }
    return result;
}

// Implement other specific decoding methods...

long NetStreamUtils::decodeUnsignedVarint(const std::vector<uint8_t>& buffer, size_t& offset) {
    long result = 0;
    int shift = 0;
    while (offset < buffer.size()) {
        uint8_t byte = buffer[offset++];
        result |= (long(byte & 0x7F) << shift);
        if ((byte & 0x80) == 0) {
            break;
        }
        shift += 7;
    }
    return result;
}

long NetStreamUtils::decodeVarint(const std::vector<uint8_t>& buffer, size_t& offset) {
    long number = decodeUnsignedVarint(buffer, offset);
    return ((number & 1) == 0) ? (number >> 1) : -(number >> 1);
}

// Implement other utility methods...
