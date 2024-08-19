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

#ifndef THREADPROXYPIPE_HPP
#define THREADPROXYPIPE_HPP

#include "SourceBase.hpp"
#include "ProxyPipe.hpp"
#include "Replayable.hpp"
#include "Graph.hpp"
#include "Sink.hpp"
#include <list>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <iostream>

class ThreadProxyPipe : public SourceBase, public ProxyPipe {
public:
    ThreadProxyPipe();
    ThreadProxyPipe(Source* input);
    ThreadProxyPipe(Source* input, bool replay);
    ThreadProxyPipe(Source* input, Sink* initialListener, bool replay);

    void init();
    void init(Source* source);
    void init(Source* source, bool replay);

    void unregisterFromSource();
    void pump();
    void blockingPump() throw(std::exception);
    void blockingPump(long timeout) throw(std::exception);
    bool hasPostRemaining();

    std::string toString() const override;

protected:
    enum class GraphEvents {
        ADD_NODE, DEL_NODE, ADD_EDGE, DEL_EDGE, STEP, CLEARED, ADD_GRAPH_ATTR, CHG_GRAPH_ATTR, DEL_GRAPH_ATTR, ADD_NODE_ATTR, CHG_NODE_ATTR, DEL_NODE_ATTR, ADD_EDGE_ATTR, CHG_EDGE_ATTR, DEL_EDGE_ATTR
    };

    bool maybeUnregister();
    void post(GraphEvents e, std::initializer_list<void*> data);

    void processMessage(GraphEvents e, const std::vector<void*>& data);

    void edgeAttributeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute, void* value);
    void edgeAttributeChanged(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute, void* oldValue, void* newValue);
    void edgeAttributeRemoved(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute);

    void graphAttributeAdded(const std::string& graphId, long timeId, const std::string& attribute, void* value);
    void graphAttributeChanged(const std::string& graphId, long timeId, const std::string& attribute, void* oldValue, void* newValue);
    void graphAttributeRemoved(const std::string& graphId, long timeId, const std::string& attribute);

    void nodeAttributeAdded(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute, void* value);
    void nodeAttributeChanged(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute, void* oldValue, void* newValue);
    void nodeAttributeRemoved(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute);

    void edgeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed);
    void edgeRemoved(const std::string& graphId, long timeId, const std::string& edgeId);

    void graphCleared(const std::string& graphId, long timeId);

    void nodeAdded(const std::string& graphId, long timeId, const std::string& nodeId);
    void nodeRemoved(const std::string& graphId, long timeId, const std::string& nodeId);

    void stepBegins(const std::string& graphId, long timeId, double step);

private:
    std::string id;
    std::string from;
    std::list<GraphEvents> events;
    std::list<std::vector<void*>> eventsData;
    std::mutex lock;
    std::condition_variable notEmpty;
    Source* input;
    bool unregisterWhenPossible = false;
};

#endif // THREADPROXYPIPE_HPP
