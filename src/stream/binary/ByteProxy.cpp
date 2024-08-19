#include "ByteProxy.hpp"

ByteProxy::ByteProxy(std::shared_ptr<ByteFactory> factory, int port)
    : ByteProxy(factory, Mode::SERVER, "127.0.0.1", port) {}

ByteProxy::ByteProxy(std::shared_ptr<ByteFactory> factory, Mode mode, const std::string& address, int port)
    : byteFactory(factory), mode(mode), address(address), port(port), running(false), mainSocket(-1) {
    encoder = byteFactory->createByteEncoder();
    decoder = byteFactory->createByteDecoder();
    init();
}

ByteProxy::~ByteProxy() {
    stop();
}

void ByteProxy::start() {
    if (running) {
        throw std::runtime_error("Already started.");
    }
    running = true;
    workerThread = std::thread(&ByteProxy::run, shared_from_this());
}

void ByteProxy::stop() {
    running = false;
    if (workerThread.joinable()) {
        workerThread.join();
    }
    if (mainSocket != -1) {
        close(mainSocket);
    }
    for (int sock : writableSockets) {
        close(sock);
    }
}

void ByteProxy::run() {
    while (running) {
        poll();
    }
}

void ByteProxy::poll(bool blocking) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(mainSocket, &readfds);

    struct timeval tv = {0, 100000};  // 100ms
    int activity = select(mainSocket + 1, &readfds, nullptr, nullptr, blocking ? nullptr : &tv);

    if (activity < 0 && errno != EINTR) {
        std::cerr << "Select error" << std::endl;
    }

    if (FD_ISSET(mainSocket, &readfds)) {
        processSelectedKeys();
    }
}

void ByteProxy::init() {
    mainSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mainSocket < 0) {
        throw std::runtime_error("Socket creation failed.");
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr);

    if (mode == Mode::SERVER) {
        bind(mainSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        listen(mainSocket, 10);
    } else {
        if (connect(mainSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            throw std::runtime_error("Connection failed.");
        }
        writableSockets.push_back(mainSocket);
    }
}

void ByteProxy::processSelectedKeys() {
    if (mode == Mode::SERVER) {
        int clientSocket = accept(mainSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            std::cerr << "Accept failed." << std::endl;
        } else {
            writableSockets.push_back(clientSocket);
            replay(clientSocket);
        }
    } else {
        readDataChunk(mainSocket);
    }
}

void ByteProxy::readDataChunk(int socket) {
    char buffer[1024];
    int bytesRead = read(socket, buffer, sizeof(buffer));
    if (bytesRead <= 0) {
        std::cerr << "Read error or connection closed." << std::endl;
        close(socket);
        writableSockets.erase(std::remove(writableSockets.begin(), writableSockets.end(), socket), writableSockets.end());
        return;
    }

    ByteBuffer byteBuffer(buffer, buffer + bytesRead);
    while (decoder->validate(byteBuffer)) {
        decoder->decode(byteBuffer);
    }
}

void ByteProxy::doSend(const ByteBuffer& buffer) {
    for (int sock : writableSockets) {
        send(sock, buffer.data(), buffer.size(), 0);
    }
}

void ByteProxy::replay(int socket) {
    if (replayable) {
        auto controller = replayable->getReplayController();
        auto replayEncoder = byteFactory->createByteEncoder();
        replayEncoder->addTransport([socket](const ByteBuffer& buffer) {
            send(socket, buffer.data(), buffer.size(), 0);
        });
        controller->addSink(replayEncoder);
        controller->replay();
    }
}

void ByteProxy::close(int socket) {
    ::close(socket);
}

void ByteProxy::setReplayable(std::shared_ptr<Replayable> replayable) {
    this->replayable = replayable;
}
