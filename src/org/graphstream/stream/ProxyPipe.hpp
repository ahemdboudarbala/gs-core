#ifndef PROXY_PIPE_HPP
#define PROXY_PIPE_HPP

#include "Pipe.hpp"
#include <stdexcept>

/**
 * Interface for a ProxyPipe.
 * 
 * A proxy is a kind of event buffer that allows passing some kind of barrier,
 * such as a thread or a network. It decouples the source from the sink by
 * buffering the source events and sending them to the listeners at the sink
 * when possible. This makes the process indirect, non-synchronized, and
 * non-blocking.
 */
class ProxyPipe : public Pipe {
public:
    virtual ~ProxyPipe() = default;

    /**
     * Check if some events are pending and dispatch them to the registered outputs.
     */
    virtual void pump() = 0;

    /**
     * Same as pump() but tries to block until new events are available.
     * Note that this feature might not be available in all proxy pipe implementations
     * and may throw std::runtime_error. It can throw std::runtime_error if the current
     * thread is interrupted while the proxy is waiting for events.
     *
     * @throws std::runtime_error if the operation is not supported.
     * @throws std::runtime_error if interrupted while waiting.
     */
    virtual void blockingPump() {
        throw std::runtime_error("Unsupported operation: blockingPump");
    }

    /**
     * Same as blockingPump() but with a timeout delay.
     *
     * @param timeout The maximum time to wait in milliseconds.
     * @throws std::runtime_error if the operation is not supported.
     * @throws std::runtime_error if interrupted while waiting.
     */
    virtual void blockingPump(long timeout) {
        throw std::runtime_error("Unsupported operation: blockingPump with timeout");
    }
};

#endif // PROXY_PIPE_HPP
