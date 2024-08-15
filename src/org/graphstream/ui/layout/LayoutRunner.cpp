#include "LayoutRunner.hpp"

LayoutRunner::LayoutRunner(std::shared_ptr<Source> source, std::shared_ptr<Layout> layout, bool startImmediately)
    : layout(layout), pumpPipe(std::make_shared<ThreadProxyPipe>()), loop(true), longNap(80), shortNap(10) {
    
    pumpPipe->addSink(layout);

    if (startImmediately) {
        layoutThread = std::thread(&LayoutRunner::run, this);
    }

    pumpPipe->init(source);
}

LayoutRunner::LayoutRunner(std::shared_ptr<Graph> graph, std::shared_ptr<Layout> layout, bool startImmediately, bool replay)
    : layout(layout), pumpPipe(std::make_shared<ThreadProxyPipe>()), loop(true), longNap(80), shortNap(10) {
    
    pumpPipe->addSink(layout);

    if (startImmediately) {
        layoutThread = std::thread(&LayoutRunner::run, this);
    }

    pumpPipe->init(graph, replay);
}

LayoutRunner::~LayoutRunner() {
    release();
}

std::unique_ptr<ProxyPipe> LayoutRunner::newLayoutPipe() {
    auto tpp = std::make_unique<ThreadProxyPipe>();
    tpp->init(layout);
    return tpp;
}

void LayoutRunner::run() {
    while (loop) {
        double limit = layout->getStabilizationLimit();

        pumpPipe->pump();
        if (limit > 0) {
            if (layout->getStabilization() > limit) {
                nap(longNap);
            } else {
                layout->compute();
                nap(shortNap);
            }
        } else {
            layout->compute();
            nap(shortNap);
        }
    }
    std::cout << "Layout '" << layout->getLayoutAlgorithmName() << "' process stopped." << std::endl;
}

void LayoutRunner::release() {
    loop = false;

    if (layoutThread.joinable()) {
        layoutThread.join();
    }

    if (pumpPipe) {
        pumpPipe->unregisterFromSource();
        pumpPipe->removeSink(layout);
        pumpPipe.reset();
    }

    layout.reset();
}

void LayoutRunner::nap(long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void LayoutRunner::setNaps(long longNap, long shortNap) {
    this->longNap = longNap;
    this->shortNap = shortNap;
}
