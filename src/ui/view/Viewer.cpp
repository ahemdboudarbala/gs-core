#include "Viewer.hpp"
#include "stream/thread/ThreadProxyPipe.hpp"
#include "ViewerPipe.hpp"
#include <sstream>
#include <iomanip>

std::string Viewer::newGGId() {
    std::ostringstream oss;
    oss << "GraphicGraph_" << std::setw(4) << std::setfill('0') << (rand() % 10000);
    return oss.str();
}

Viewer::CloseFramePolicy Viewer::getCloseFramePolicy() const {
    return closeFramePolicy;
}

std::shared_ptr<ProxyPipe> Viewer::newThreadProxyOnGraphicGraph() {
    auto tpp = std::make_shared<ThreadProxyPipe>();
    tpp->init(graph);
    return tpp;
}

std::shared_ptr<ViewerPipe> Viewer::newViewerPipe() {
    auto tpp = std::make_shared<ThreadProxyPipe>();
    tpp->init(graph, false);

    enableXYZfeedback(true);

    return std::make_shared<ViewerPipe>(newGGId(), tpp);
}

std::shared_ptr<GraphicGraph> Viewer::getGraphicGraph() {
    return graph;
}

std::shared_ptr<View> Viewer::getView(const std::string& id) {
    auto it = views.find(id);
    if (it != views.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<View> Viewer::getDefaultView() {
    return getView(getDefaultID());
}

std::shared_ptr<View> Viewer::addDefaultView(bool openInAFrame) {
    auto renderer = newDefaultGraphRenderer();
    auto view = renderer->createDefaultView(shared_from_this(), getDefaultID());

    addView(view);

    if (openInAFrame) {
        view->openInAFrame(true);
    }

    return view;
}

std::shared_ptr<View> Viewer::addView(std::shared_ptr<View> view) {
    auto old = views[view->getIdView()];
    if (old && old != view) {
        old->close(graph);
    }
    views[view->getIdView()] = view;
    return old;
}

std::shared_ptr<View> Viewer::addView(const std::string& id, std::shared_ptr<GraphRenderer> renderer, bool openInAFrame) {
    auto view = renderer->createDefaultView(shared_from_this(), id);
    addView(view);

    if (openInAFrame) {
        view->openInAFrame(true);
    }

    return view;
}

void Viewer::removeView(const std::string& id) {
    views.erase(id);
}

void Viewer::computeGraphMetrics() {
    graph->computeBounds();

    for (auto& viewEntry : views) {
        auto camera = viewEntry.second->getCamera();
        if (camera) {
            auto lo = graph->getMinPos();
            auto hi = graph->getMaxPos();
            camera->setBounds(lo.x, lo.y, lo.z, hi.x, hi.y, hi.z);
        }
    }
}

void Viewer::setCloseFramePolicy(CloseFramePolicy policy) {
    closeFramePolicy = policy;
}

void Viewer::enableXYZfeedback(bool on) {
    graph->feedbackXYZ(on);
}

void Viewer::enableAutoLayout() {
    enableAutoLayout(Layouts::newLayoutAlgorithm());
}

void Viewer::enableAutoLayout(std::shared_ptr<Layout> layoutAlgorithm) {
    if (!optLayout) {
        optLayout = std::make_shared<LayoutRunner>(graph, layoutAlgorithm, true, false);
        graph->replay();
        layoutPipeIn = optLayout->newLayoutPipe();
        layoutPipeIn->addAttributeSink(graph);
    }
}

void Viewer::disableAutoLayout() {
    if (optLayout) {
        std::dynamic_pointer_cast<ThreadProxyPipe>(layoutPipeIn)->unregisterFromSource();
        layoutPipeIn->removeSink(graph);
        layoutPipeIn = nullptr;
        optLayout->release();
        optLayout = nullptr;
    }
}

void Viewer::replayGraph(std::shared_ptr<Graph> graph) {
    for (const auto& key : graph->attributeKeys()) {
        this->graph->setAttribute(key, graph->getAttribute(key));
    }

    for (const auto& node : graph->nodes()) {
        auto n = this->graph->addNode(node->getId());
        for (const auto& key : node->attributeKeys()) {
            n->setAttribute(key, node->getAttribute(key));
        }
    }

    for (const auto& edge : graph->edges()) {
        auto e = this->graph->addEdge(edge->getId(), edge->getSourceNode()->getId(), edge->getTargetNode()->getId(), edge->isDirected());
        for (const auto& key : edge->attributeKeys()) {
            e->setAttribute(key, edge->getAttribute(key));
        }
    }
}
