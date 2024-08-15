#include "NetStreamEncoder.hpp"

NetStreamEncoder::NetStreamEncoder(const std::string& stream) {
    streamBuffer = NetStreamUtils::encodeString(stream);
}

void NetStreamEncoder::addTransport(std::shared_ptr<Transport> transport) {
    transportList.push_back(transport);
}

void NetStreamEncoder::removeTransport(std::shared_ptr<Transport> transport) {
    transportList.erase(std::remove(transportList.begin(), transportList.end(), transport), transportList.end());
}

std::vector<uint8_t> NetStreamEncoder::getEncodedValue(const std::any& in, int valueType) {
    auto value = NetStreamUtils::encodeValue(in, valueType);
    if (value.empty()) {
        std::cerr << "Unknown value type: " << valueType << std::endl;
    }
    return value;
}

void NetStreamEncoder::doSend(const std::vector<uint8_t>& event) {
    for (const auto& transport : transportList) {
        transport->send(event);
    }
}

std::vector<uint8_t> NetStreamEncoder::getAndPrepareBuffer(const std::string& sourceId, long timeId, int eventType, int messageSize) {
    if (sourceId != this->sourceId) {
        this->sourceId = sourceId;
        sourceIdBuff = NetStreamUtils::encodeString(sourceId);
    }

    std::vector<uint8_t> buffer;
    buffer.reserve(4 + streamBuffer.size() + 1 + sourceIdBuff.size() + NetStreamUtils::getVarintSize(timeId) + messageSize);

    buffer.insert(buffer.end(), streamBuffer.begin(), streamBuffer.end());
    buffer.push_back(static_cast<uint8_t>(eventType));
    buffer.insert(buffer.end(), sourceIdBuff.begin(), sourceIdBuff.end());
    auto timeIdEncoded = NetStreamUtils::encodeUnsignedVarint(timeId);
    buffer.insert(buffer.end(), timeIdEncoded.begin(), timeIdEncoded.end());

    return buffer;
}

void NetStreamEncoder::graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, const std::any& value) {
    auto attrBuff = NetStreamUtils::encodeString(attribute);
    int valueType = NetStreamUtils::getType(value);
    auto valueBuff = getEncodedValue(value, valueType);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_ADD_GRAPH_ATTR, attrBuff.size() + 1 + valueBuff.size());

    buff.insert(buff.end(), attrBuff.begin(), attrBuff.end());
    buff.push_back(static_cast<uint8_t>(valueType));
    buff.insert(buff.end(), valueBuff.begin(), valueBuff.end());

    doSend(buff);
}

void NetStreamEncoder::graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    auto attrBuff = NetStreamUtils::encodeString(attribute);
    int oldValueType = NetStreamUtils::getType(oldValue);
    int newValueType = NetStreamUtils::getType(newValue);

    auto oldValueBuff = getEncodedValue(oldValue, oldValueType);
    auto newValueBuff = getEncodedValue(newValue, newValueType);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_CHG_GRAPH_ATTR, attrBuff.size() + 1 + oldValueBuff.size() + 1 + newValueBuff.size());

    buff.insert(buff.end(), attrBuff.begin(), attrBuff.end());
    buff.push_back(static_cast<uint8_t>(oldValueType));
    buff.insert(buff.end(), oldValueBuff.begin(), oldValueBuff.end());
    buff.push_back(static_cast<uint8_t>(newValueType));
    buff.insert(buff.end(), newValueBuff.begin(), newValueBuff.end());

    doSend(buff);
}

void NetStreamEncoder::graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) {
    auto attrBuff = NetStreamUtils::encodeString(attribute);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_DEL_GRAPH_ATTR, attrBuff.size());

    buff.insert(buff.end(), attrBuff.begin(), attrBuff.end());

    doSend(buff);
}

void NetStreamEncoder::nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& value) {
    auto nodeBuff = NetStreamUtils::encodeString(nodeId);
    auto attrBuff = NetStreamUtils::encodeString(attribute);
    int valueType = NetStreamUtils::getType(value);
    auto valueBuff = getEncodedValue(value, valueType);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_ADD_NODE_ATTR, nodeBuff.size() + attrBuff.size() + 1 + valueBuff.size());

    buff.insert(buff.end(), nodeBuff.begin(), nodeBuff.end());
    buff.insert(buff.end(), attrBuff.begin(), attrBuff.end());
    buff.push_back(static_cast<uint8_t>(valueType));
    buff.insert(buff.end(), valueBuff.begin(), valueBuff.end());

    doSend(buff);
}

void NetStreamEncoder::nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    auto nodeBuff = NetStreamUtils::encodeString(nodeId);
    auto attrBuff = NetStreamUtils::encodeString(attribute);
    int oldValueType = NetStreamUtils::getType(oldValue);
    int newValueType = NetStreamUtils::getType(newValue);

    auto oldValueBuff = getEncodedValue(oldValue, oldValueType);
    auto newValueBuff = getEncodedValue(newValue, newValueType);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_CHG_NODE_ATTR, nodeBuff.size() + attrBuff.size() + 1 + oldValueBuff.size() + 1 + newValueBuff.size());

    buff.insert(buff.end(), nodeBuff.begin(), nodeBuff.end());
    buff.insert(buff.end(), attrBuff.begin(), attrBuff.end());
    buff.push_back(static_cast<uint8_t>(oldValueType));
    buff.insert(buff.end(), oldValueBuff.begin(), oldValueBuff.end());
    buff.push_back(static_cast<uint8_t>(newValueType));
    buff.insert(buff.end(), newValueBuff.begin(), newValueBuff.end());

    doSend(buff);
}

void NetStreamEncoder::nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute) {
    auto nodeBuff = NetStreamUtils::encodeString(nodeId);
    auto attrBuff = NetStreamUtils::encodeString(attribute);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_DEL_NODE_ATTR, nodeBuff.size() + attrBuff.size());

    buff.insert(buff.end(), nodeBuff.begin(), nodeBuff.end());
    buff.insert(buff.end(), attrBuff.begin(), attrBuff.end());

    doSend(buff);
}

void NetStreamEncoder::edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& value) {
    auto edgeBuff = NetStreamUtils::encodeString(edgeId);
    auto attrBuff = NetStreamUtils::encodeString(attribute);
    int valueType = NetStreamUtils::getType(value);
    auto valueBuff = getEncodedValue(value, valueType);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_ADD_EDGE_ATTR, edgeBuff.size() + attrBuff.size() + 1 + valueBuff.size());

    buff.insert(buff.end(), edgeBuff.begin(), edgeBuff.end());
    buff.insert(buff.end(), attrBuff.begin(), attrBuff.end());
    buff.push_back(static_cast<uint8_t>(valueType));
    buff.insert(buff.end(), valueBuff.begin(), valueBuff.end());

    doSend(buff);
}

void NetStreamEncoder::edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    auto edgeBuff = NetStreamUtils::encodeString(edgeId);
    auto attrBuff = NetStreamUtils::encodeString(attribute);
    int oldValueType = NetStreamUtils::getType(oldValue);
    int newValueType = NetStreamUtils::getType(newValue);

    auto oldValueBuff = getEncodedValue(oldValue, oldValueType);
    auto newValueBuff = getEncodedValue(newValue, newValueType);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_CHG_EDGE_ATTR, edgeBuff.size() + attrBuff.size() + 1 + oldValueBuff.size() + 1 + newValueBuff.size());

    buff.insert(buff.end(), edgeBuff.begin(), edgeBuff.end());
    buff.insert(buff.end(), attrBuff.begin(), attrBuff.end());
    buff.push_back(static_cast<uint8_t>(oldValueType));
    buff.insert(buff.end(), oldValueBuff.begin(), oldValueBuff.end());
    buff.push_back(static_cast<uint8_t>(newValueType));
    buff.insert(buff.end(), newValueBuff.begin(), newValueBuff.end());

    doSend(buff);
}

void NetStreamEncoder::edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute) {
    auto edgeBuff = NetStreamUtils::encodeString(edgeId);
    auto attrBuff = NetStreamUtils::encodeString(attribute);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_DEL_EDGE_ATTR, edgeBuff.size() + attrBuff.size());

    buff.insert(buff.end(), edgeBuff.begin(), edgeBuff.end());
    buff.insert(buff.end(), attrBuff.begin(), attrBuff.end());

    doSend(buff);
}

void NetStreamEncoder::nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) {
    auto nodeBuff = NetStreamUtils::encodeString(nodeId);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_ADD_NODE, nodeBuff.size());
    buff.insert(buff.end(), nodeBuff.begin(), nodeBuff.end());

    doSend(buff);
}

void NetStreamEncoder::nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) {
    auto nodeBuff = NetStreamUtils::encodeString(nodeId);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_DEL_NODE, nodeBuff.size());
    buff.insert(buff.end(), nodeBuff.begin(), nodeBuff.end());

    doSend(buff);
}

void NetStreamEncoder::edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) {
    auto edgeBuff = NetStreamUtils::encodeString(edgeId);
    auto fromNodeBuff = NetStreamUtils::encodeString(fromNodeId);
    auto toNodeBuff = NetStreamUtils::encodeString(toNodeId);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_ADD_EDGE, edgeBuff.size() + fromNodeBuff.size() + toNodeBuff.size() + 1);

    buff.insert(buff.end(), edgeBuff.begin(), edgeBuff.end());
    buff.insert(buff.end(), fromNodeBuff.begin(), fromNodeBuff.end());
    buff.insert(buff.end(), toNodeBuff.begin(), toNodeBuff.end());
    buff.push_back(static_cast<uint8_t>(!directed ? 0 : 1));

    doSend(buff);
}

void NetStreamEncoder::edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) {
    auto edgeBuff = NetStreamUtils::encodeString(edgeId);

    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_DEL_EDGE, edgeBuff.size());
    buff.insert(buff.end(), edgeBuff.begin(), edgeBuff.end());

    doSend(buff);
}

void NetStreamEncoder::graphCleared(const std::string& sourceId, long timeId) {
    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_CLEARED, 0);
    doSend(buff);
}

void NetStreamEncoder::stepBegins(const std::string& sourceId, long timeId, double step) {
    auto buff = getAndPrepareBuffer(sourceId, timeId, NetStreamConstants::EVENT_STEP, sizeof(double));
    NetStreamUtils::encodeDouble(buff, step);

    doSend(buff);
}
