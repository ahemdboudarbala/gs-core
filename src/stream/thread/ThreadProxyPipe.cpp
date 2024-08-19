/*
 * This file is part of GraphStream <http://graphstream-project.org>.
 * 
 * GraphStream is a library whose purpose is to handle static or dynamic
 * graph, create them from scratch, file or any source and display them.
 * 
 * This program is free software distributed under the terms of two licenses, the
 * CeCILL-C license that fits European law, and the GNU Lesser General Public
 * License. You can use, modify and/or redistribute the software under the terms
 * of the CeCILL-C license as circulated by CEA, CNRS and INRIA at the following
 * URL <http://www.cecill.info> or under the terms of the GNU LGPL as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-C and LGPL licenses and that you accept their terms.
 */

#include "ThreadProxyPipe.hpp"
#include <iostream>
#include <stdexcept>

// Constructors
ThreadProxyPipe::ThreadProxyPipe() : input(nullptr) {
    from = "<in>";
}

ThreadProxyPipe::ThreadProxyPipe(Source* input) : ThreadProxyPipe() {
    init(input);
}

ThreadProxyPipe::ThreadProxyPipe(Source* input, bool replay) : ThreadProxyPipe() {
    init(input, replay);
}

ThreadProxyPipe::ThreadProxyPipe(Source* input, Sink* initialListener, bool replay) : ThreadProxyPipe() {
    if (initialListener != nullptr)
        addSink(initialListener);
    init(input, replay);
}

// Methods
void ThreadProxyPipe::init() {
    init(nullptr, false);
}

void ThreadProxyPipe::init(Source* source) {
    init(source, dynamic_cast<Replayable*>(source) != nullptr);
}

void ThreadProxyPipe::init(Source* source, bool replay) {
    std::lock_guard<std::mutex> guard(lock);

    if (this->input != nullptr)
        this->input->removeSink(this);

    this->input = source;
    events.clear();
    eventsData.clear();

    if (source != nullptr) {
        if (auto graph = dynamic_cast<Graph*>(source))
            this->from = graph->getId();

        this->input->addSink(this);

        if (replay) {
            if (auto r = dynamic_cast<Replayable*>(source)) {
                auto rc = r->getReplayController();
                rc->addSink(this);
                rc->replay();
            }
        }
    }
}

void ThreadProxyPipe::unregisterFromSource() {
    unregisterWhenPossible = true;
}

void ThreadProxyPipe::pump() {
    GraphEvents e;
    std::vector<void*> data;

    do {
        std::unique_lock<std::mutex> lock(this->lock);
        if (events.empty())
            return;

        e = events.front();
        data = eventsData.front();
        events.pop_front();
        eventsData.pop_front();
        lock.unlock();

        processMessage(e, data);
    } while (!events.empty());
}

void ThreadProxyPipe::blockingPump() throw(std::exception) {
    blockingPump(0);
}

void ThreadProxyPipe::blockingPump(long timeout) throw(std::exception) {
    GraphEvents e;
    std::vector<void*> data;

    std::unique_lock<std::mutex> lock(this->lock);

    if (timeout > 0)
        notEmpty.wait_for(lock, std::chrono::milliseconds(timeout), [this]() { return !events.empty(); });
    else
        notEmpty.wait(lock, [this]() { return !events.empty(); });

    if (events.empty())
        return;

    e = events.front();
    data = eventsData.front();
    events.pop_front();
    eventsData.pop_front();
    lock.unlock();

    processMessage(e, data);
}

bool ThreadProxyPipe::hasPostRemaining() {
    std::lock_guard<std::mutex> guard(lock);
    return !events.empty();
}

std::string ThreadProxyPipe::toString() const {
    std::string dest = "nil";
    if (!attrSinks.empty())
        dest = attrSinks.front()->toString();

    return "thread-proxy(from " + from + " to " + dest + ")";
}

// Protected Methods
bool ThreadProxyPipe::maybeUnregister() {
    if (unregisterWhenPossible) {
        if (input != nullptr)
            input->removeSink(this);
        return true;
    }
    return false;
}

void ThreadProxyPipe::post(GraphEvents e, std::initializer_list<void*> data) {
    std::lock_guard<std::mutex> guard(lock);
    events.push_back(e);
    eventsData.push_back(data);
    notEmpty.notify_one();
}

// Event handling methods (example shown for edgeAttributeAdded)
void ThreadProxyPipe::edgeAttributeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute, void* value) {
    if (maybeUnregister())
        return;
    post(GraphEvents::ADD_EDGE_ATTR, { (void*)&graphId, (void*)&timeId, (void*)&edgeId, (void*)&attribute, value });
}

// Other event handling methods (like edgeAttributeChanged, graphAttributeAdded, etc.) would follow the same pattern as edgeAttributeAdded

void ThreadProxyPipe::processMessage(GraphEvents e, const std::vector<void*>& data) {
    std::string* graphId;
    std::string* elementId;
    std::string* attribute;
    long* timeId;
    void* newValue;
    void* oldValue;

    switch (e) {
        case GraphEvents::ADD_NODE:
            graphId = static_cast<std::string*>(data[0]);
            timeId = static_cast<long*>(data[1]);
            elementId = static_cast<std::string*>(data[2]);
            sendNodeAdded(*graphId, *timeId, *elementId);
            break;
        // Other cases to be handled similarly
        default:
            std::cerr << "Unknown message " << static_cast<int>(e) << std::endl;
            break;
    }
}
