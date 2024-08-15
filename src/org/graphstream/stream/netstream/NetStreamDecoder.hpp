#ifndef NETSTREAMDECODER_HPP
#define NETSTREAMDECODER_HPP

#include "NetStreamConstants.hpp"
#include "NetStreamUtils.hpp"
#include "SourceBase.hpp"
#include "ByteDecoder.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>

class NetStreamDecoder : public SourceBase, public ByteDecoder {
public:
    NetStreamDecoder();
    bool validate(const std::vector<uint8_t>& buffer) override;
    void decode(std::vector<uint8_t>& bb) override;

protected:
    void serve_EVENT_DEL_EDGE_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_CHG_EDGE_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_ADD_EDGE_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_DEL_NODE_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_CHG_NODE_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_ADD_NODE_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_DEL_GRAPH_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_CHG_GRAPH_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_ADD_GRAPH_ATTR(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_CLEARED(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_STEP(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_DEL_EDGE(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_ADD_EDGE(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_DEL_NODE(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);
    void serve_EVENT_ADD_NODE(std::vector<uint8_t>::iterator& iter, const std::vector<uint8_t>::iterator& end);

private:
    int getInt(std::vector<uint8_t>::iterator& iter);
    int getByte(std::vector<uint8_t>::iterator& iter);
};

#endif // NETSTREAMDECODER_HPP
