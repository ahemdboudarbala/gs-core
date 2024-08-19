#include "NetStreamDecoder.hpp"

NetStreamDecoder::NetStreamDecoder() {}

bool NetStreamDecoder::validate(const std::vector<uint8_t>& buffer) {
    if (buffer.size() >= 4) {
        int size = *reinterpret_cast<const int*>(buffer.data());
        return buffer.size() >= static_cast<size_t>(size);
    }
    return false;
}

void NetStreamDecoder::decode(std::vector<uint8_t>& bb) {
    try {
        auto iter = bb.begin();
        int size = getInt(iter);
        std::string streamId = NetStreamUtils::decodeString(iter, bb.end());
        int cmd = getByte(iter);

        if (cmd == NetStreamConstants::EVENT_ADD_NODE) {
            serve_EVENT_ADD_NODE(iter, bb.end());
        } else if ((cmd & 0xFF) == (NetStreamConstants::EVENT_DEL_NODE & 0xFF)) {
            serve_DEL_NODE(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_ADD_EDGE) {
            serve_EVENT_ADD_EDGE(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_DEL_EDGE) {
            serve_EVENT_DEL_EDGE(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_STEP) {
            serve_EVENT_STEP(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_CLEARED) {
            serve_EVENT_CLEARED(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_ADD_GRAPH_ATTR) {
            serve_EVENT_ADD_GRAPH_ATTR(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_CHG_GRAPH_ATTR) {
            serve_EVENT_CHG_GRAPH_ATTR(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_DEL_GRAPH_ATTR) {
            serve_EVENT_DEL_GRAPH_ATTR(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_ADD_NODE_ATTR) {
            serve_EVENT_ADD_NODE_ATTR(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_CHG_NODE_ATTR) {
            serve_EVENT_CHG_NODE_ATTR(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_DEL_NODE_ATTR) {
            serve_EVENT_DEL_NODE_ATTR(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_ADD_EDGE_ATTR) {
            serve_EVENT_ADD_EDGE_ATTR(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_CHG_EDGE_ATTR) {
            serve_EVENT_CHG_EDGE_ATTR(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_DEL_EDGE_ATTR) {
            serve_EVENT_DEL_EDGE_ATTR(iter, bb.end());
        } else if (cmd == NetStreamConstants::EVENT_END) {
            std::cout << "NetStreamReceiver : Client properly ended the connection." << std::endl;
        } else {
            std::cout << "NetStreamReceiver: Don't know this command: " << cmd << std::endl;
        }
    } catch (const std::out_of_range& e) {
        std::cout << "bad buffer" << std::endl;
    }
}

void NetStreamDecoder::serve_EVENT_DEL_EDGE_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received DEL_EDGE_ATTR command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string edgeId = NetStreamUtils::decodeString(iter, end);
    std::string attrId = NetStreamUtils::decodeString(iter, end);

    sendEdgeAttributeRemoved(sourceId, timeId, edgeId, attrId);
}

void NetStreamDecoder::serve_EVENT_CHG_EDGE_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received CHG_EDGE_ATTR command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string edgeId = NetStreamUtils::decodeString(iter, end);
    std::string attrId = NetStreamUtils::decodeString(iter, end);
    int oldValueType = NetStreamUtils::decodeType(iter, end);
    auto oldValue = NetStreamUtils::decodeValue(iter, end, oldValueType);
    int newValueType = NetStreamUtils::decodeType(iter, end);
    auto newValue = NetStreamUtils::decodeValue(iter, end, newValueType);

    sendEdgeAttributeChanged(sourceId, timeId, edgeId, attrId, oldValue, newValue);
}

void NetStreamDecoder::serve_EVENT_ADD_EDGE_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received ADD_EDGE_ATTR command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string edgeId = NetStreamUtils::decodeString(iter, end);
    std::string attrId = NetStreamUtils::decodeString(iter, end);
    auto value = NetStreamUtils::decodeValue(iter, end, NetStreamUtils::decodeType(iter, end));

    sendEdgeAttributeAdded(sourceId, timeId, edgeId, attrId, value);
}

void NetStreamDecoder::serve_EVENT_DEL_NODE_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received DEL_NODE_ATTR command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string nodeId = NetStreamUtils::decodeString(iter, end);
    std::string attrId = NetStreamUtils::decodeString(iter, end);

    sendNodeAttributeRemoved(sourceId, timeId, nodeId, attrId);
}

void NetStreamDecoder::serve_EVENT_CHG_NODE_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received CHG_NODE_ATTR command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string nodeId = NetStreamUtils::decodeString(iter, end);
    std::string attrId = NetStreamUtils::decodeString(iter, end);
    int oldValueType = NetStreamUtils::decodeType(iter, end);
    auto oldValue = NetStreamUtils::decodeValue(iter, end, oldValueType);
    int newValueType = NetStreamUtils::decodeType(iter, end);
    auto newValue = NetStreamUtils::decodeValue(iter, end, newValueType);

    sendNodeAttributeChanged(sourceId, timeId, nodeId, attrId, oldValue, newValue);
}

void NetStreamDecoder::serve_EVENT_ADD_NODE_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received ADD_NODE_ATTR command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string nodeId = NetStreamUtils::decodeString(iter, end);
    std::string attrId = NetStreamUtils::decodeString(iter, end);
    auto value = NetStreamUtils::decodeValue(iter, end, NetStreamUtils::decodeType(iter, end));

    sendNodeAttributeAdded(sourceId, timeId, nodeId, attrId, value);
}

void NetStreamDecoder::serve_EVENT_DEL_GRAPH_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received DEL_GRAPH_ATTR command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string attrId = NetStreamUtils::decodeString(iter, end);

    sendGraphAttributeRemoved(sourceId, timeId, attrId);
}

void NetStreamDecoder::serve_EVENT_CHG_GRAPH_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received CHG_GRAPH_ATTR command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string attrId = NetStreamUtils::decodeString(iter, end);
    int oldValueType = NetStreamUtils::decodeType(iter, end);
    auto oldValue = NetStreamUtils::decodeValue(iter, end, oldValueType);
    int newValueType = NetStreamUtils::decodeType(iter, end);
    auto newValue = NetStreamUtils::decodeValue(iter, end, newValueType);

    sendGraphAttributeChanged(sourceId, timeId, attrId, oldValue, newValue);
}

void NetStreamDecoder::serve_EVENT_ADD_GRAPH_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received ADD_GRAPH_ATTR command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string attrId = NetStreamUtils::decodeString(iter, end);
    auto value = NetStreamUtils::decodeValue(iter, end, NetStreamUtils::decodeType(iter, end));

    std::cout << "NetStreamServer | ADD_GRAPH_ATTR | " << attrId << "=" << value << std::endl;

    sendGraphAttributeAdded(sourceId, timeId, attrId, value);
}

void NetStreamDecoder::serve_EVENT_CLEARED(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received EVENT_CLEARED command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);

    sendGraphCleared(sourceId, timeId);
}

void NetStreamDecoder::serve_EVENT_STEP(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received EVENT_STEP command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    double time = NetStreamUtils::decodeDouble(iter, end);

    sendStepBegins(sourceId, timeId, time);
}

void NetStreamDecoder::serve_EVENT_DEL_EDGE(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received DEL_EDGE command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string edgeId = NetStreamUtils::decodeString(iter, end);

    sendEdgeRemoved(sourceId, timeId, edgeId);
}

void NetStreamDecoder::serve_EVENT_ADD_EDGE(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received ADD_EDGE command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string edgeId = NetStreamUtils::decodeString(iter, end);
    std::string from = NetStreamUtils::decodeString(iter, end);
    std::string to = NetStreamUtils::decodeString(iter, end);
    bool directed = NetStreamUtils::decodeBoolean(iter, end);

    sendEdgeAdded(sourceId, timeId, edgeId, from, to, directed);
}

void NetStreamDecoder::serve_DEL_NODE(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received DEL_NODE command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string nodeId = NetStreamUtils::decodeString(iter, end);

    sendNodeRemoved(sourceId, timeId, nodeId);
}

void NetStreamDecoder::serve_EVENT_ADD_NODE(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end) {
    std::cout << "NetStreamServer: Received EVENT_ADD_NODE command." << std::endl;

    std::string sourceId = NetStreamUtils::decodeString(iter, end);
    long timeId = NetStreamUtils::decodeUnsignedVarint(iter, end);
    std::string nodeId = NetStreamUtils::decodeString(iter, end);

    sendNodeAdded(sourceId, timeId, nodeId);
}

int NetStreamDecoder::getInt(std::vector<uint8_t>::iterator& iter) {
    int value = 0;
    std::memcpy(&value, &(*iter), sizeof(int));
    std::advance(iter, sizeof(int));
    return value;
}

int NetStreamDecoder::getByte(std::vector<uint8_t>::iterator& iter) {
    int value = static_cast<int>(*iter);
    std::advance(iter, 1);
    return value;
}
