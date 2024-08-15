#ifndef BYTE_PROXY_HPP
#define BYTE_PROXY_HPP

#include <atomic>
#include <memory>
#include <vector>
#include <thread>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include "ByteFactory.hpp"
#include "ByteEncoder.hpp"
#include "ByteDecoder.hpp"
#include "SourceBase.hpp"
#include "Pipe.hpp"
#include "Replayable.hpp"

class ByteProxy : public SourceBase, public Pipe, public std::enable_shared_from_this<ByteProxy> {
public:
    enum class Mode {
        SERVER,
        CLIENT
    };

    ByteProxy(std::shared_ptr<ByteFactory> factory, int port);
    ByteProxy(std::shared_ptr<ByteFactory> factory, Mode mode, const std::string& address, int port);
    ~ByteProxy();

    void start();
    void stop();
    void run();
    void poll(bool blocking = true);
    void setReplayable(std::shared_ptr<Replayable> replayable);

protected:
    void init();
    void processSelectedKeys();
    void readDataChunk(int socket);
    void doSend(const ByteBuffer& buffer);
    void replay(int socket);
    void close(int socket);

private:
    std::shared_ptr<ByteFactory> byteFactory;
    std::shared_ptr<ByteEncoder> encoder;
    std::shared_ptr<ByteDecoder> decoder;
    std::atomic<bool> running;
    std::thread workerThread;
    Mode mode;
    std::string address;
    int port;
    int mainSocket;
    std::vector<int> writableSockets;
    std::shared_ptr<Replayable> replayable;
};

#endif // BYTE_PROXY_HPP
