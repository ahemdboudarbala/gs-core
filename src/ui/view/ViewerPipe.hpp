#ifndef VIEWER_PIPE_HPP
#define VIEWER_PIPE_HPP

#include <string>
#include <set>
#include <memory>
#include <stdexcept>
#include "ProxyPipe.hpp"
#include "SourceBase.hpp"
#include "ViewerListener.hpp"

class ViewerPipe : public SourceBase, public ProxyPipe {
public:
    ViewerPipe(const std::string& id, std::shared_ptr<ProxyPipe> pipeIn);

    std::string getId() const;

    void pump() override;
    void blockingPump() override;
    void blockingPump(long timeout) override;

    void addViewerListener(std::shared_ptr<ViewerListener> listener);
    void removeViewerListener(std::shared_ptr<ViewerListener> listener);

    void edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, void* value) override;
    void edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute, void* oldValue, void* newValue) override;
    void edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& attribute) override;

    void graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, void* value) override;
    void graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, void* oldValue, void* newValue) override;
    void graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) override;

    void nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, void* value) override;
    void nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute, void* oldValue, void* newValue) override;
    void nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, const std::string& attribute) override;

    void edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) override;
    void edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) override;

    void graphCleared(const std::string& sourceId, long timeId) override;

    void nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) override;
    void nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) override;

    void stepBegins(const std::string& sourceId, long timeId, double step) override;

private:
    std::string id;
    std::shared_ptr<ProxyPipe> pipeIn;
    std::set<std::shared_ptr<ViewerListener>> viewerListeners;
};

#endif // VIEWER_PIPE_HPP
