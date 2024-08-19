#ifndef VIEWER_HPP
#define VIEWER_HPP

#include <string>
#include <map>
#include <memory>
#include <random>

#include "ui/graphicGraph/GraphicGraph.hpp"
#include "View.hpp"
#include "stream/ProxyPipe.hpp"
#include "ui/layout/Layout.hpp"
#include "ui/layout/LayoutRunner.hpp"
#include "ViewerPipe.hpp"

class Viewer {
public:
    enum class ThreadingModel {
        GRAPH_IN_GUI_THREAD,
        GRAPH_IN_ANOTHER_THREAD,
        GRAPH_ON_NETWORK
    };

    enum class CloseFramePolicy {
        CLOSE_VIEWER,
        HIDE_ONLY,
        EXIT
    };

    virtual ~Viewer() = default;

    virtual std::string getDefaultID() const = 0;
    virtual void init(std::shared_ptr<GraphicGraph> graph, std::shared_ptr<ProxyPipe> ppipe, std::shared_ptr<Source> source) = 0;
    virtual void close() = 0;
    virtual std::shared_ptr<GraphRenderer> newDefaultGraphRenderer() = 0;

    CloseFramePolicy getCloseFramePolicy() const;
    std::shared_ptr<ProxyPipe> newThreadProxyOnGraphicGraph();
    std::shared_ptr<ViewerPipe> newViewerPipe();
    std::shared_ptr<GraphicGraph> getGraphicGraph();
    std::shared_ptr<View> getView(const std::string& id);
    std::shared_ptr<View> getDefaultView();
    std::shared_ptr<View> addDefaultView(bool openInAFrame);
    std::shared_ptr<View> addView(std::shared_ptr<View> view);
    std::shared_ptr<View> addView(const std::string& id, std::shared_ptr<GraphRenderer> renderer, bool openInAFrame = true);
    void removeView(const std::string& id);
    void computeGraphMetrics();
    void setCloseFramePolicy(CloseFramePolicy policy);

    void enableXYZfeedback(bool on);
    void enableAutoLayout();
    void enableAutoLayout(std::shared_ptr<Layout> layoutAlgorithm);
    void disableAutoLayout();
    void replayGraph(std::shared_ptr<Graph> graph);

protected:
    std::string newGGId();

    bool graphInAnotherThread = true;
    std::shared_ptr<GraphicGraph> graph;
    std::shared_ptr<ProxyPipe> pumpPipe;
    std::shared_ptr<Source> sourceInSameThread;
    std::map<std::string, std::shared_ptr<View>> views;
    CloseFramePolicy closeFramePolicy = CloseFramePolicy::EXIT;
    std::shared_ptr<LayoutRunner> optLayout = nullptr;
    std::shared_ptr<ProxyPipe> layoutPipeIn = nullptr;
};

#endif // VIEWER_HPP
