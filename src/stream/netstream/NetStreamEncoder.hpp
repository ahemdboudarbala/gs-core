#ifndef NETSTREAMENCODER_HPP
#define NETSTREAMENCODER_HPP

#include "NetStreamConstants.hpp"
#include "NetStreamUtils.hpp"
#include "ByteEncoder.hpp"
#include "Transport.hpp"

#include <vector>
#include <string>
#include <memory>
#include <iostream>

class NetStreamEncoder : public ByteEncoder {
public:
    NetStreamEncoder(const std::string& stream = "default");
    void addTransport(std::shared_ptr<Transport> transport) override;
    void removeTransport(std::shared_ptr<Transport> transport) override;

    void graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, const std::any& value) override;
    void graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;
    void graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) override;
    void nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& value) override;
    void nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;
    void nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute) override;
    void edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& value) override;
    void edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;
    void edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute) override;
    void nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) override;
    void nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) override;
    void edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) override;
    void edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) override;
    void graphCleared(const std::string& sourceId, long timeId) override;
    void stepBegins(const std::string& sourceId, long timeId, double step) override;

protected:
    std::vector<std::shared_ptr<Transport>> transportList;
    std::string sourceId;
    std::vector<uint8_t> sourceIdBuff;
    std::vector<uint8_t> streamBuffer;

    std::vector<uint8_t> getEncodedValue(const std::any& in, int valueType);
    void doSend(const std::vector<uint8_t>& event);
    std::vector<uint8_t> getAndPrepareBuffer(const std::string& sourceId, long timeId, int eventType, int messageSize);
};

#endif // NETSTREAMENCODER_HPP
