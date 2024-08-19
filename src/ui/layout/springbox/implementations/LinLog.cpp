#include "LinLog.hpp"
#include "LinLogNodeParticle.hpp"

LinLog::LinLog()
    : LinLog(false) {
}

LinLog::LinLog(bool is3D)
    : LinLog(is3D, std::mt19937(static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()))) {
}

LinLog::LinLog(bool is3D, std::mt19937& randomNumberGenerator)
    : BarnesHutLayout(is3D, randomNumberGenerator) {
    setQuality(1);
    force = 3;
}

void LinLog::configure(double a, double r, bool edgeBased, double force) {
    this->a = a;
    this->r = r;
    this->edgeBased = edgeBased;
    this->force = force;
}

std::string LinLog::getLayoutAlgorithmName() const {
    return "LinLog";
}

void LinLog::setQuality(double qualityLevel) {
    BarnesHutLayout::setQuality(qualityLevel);

    if (quality >= 1) {
        viewZone = -1;
    } else {
        viewZone = k;
    }
}

void LinLog::compute() {
    if (viewZone > 0) {
        viewZone = area / 1.5;
    }
    BarnesHutLayout::compute();
}

NodeParticle* LinLog::newNodeParticle(const std::string& id) {
    return new LinLogNodeParticle(this, id);
}

void LinLog::chooseNodePosition(NodeParticle* n0, NodeParticle* n1) {
    // Implement specific logic if needed.
    // The original Java method is commented out and not active.
}
