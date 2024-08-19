#include "SpringBox.hpp"
#include "SpringBoxNodeParticle.hpp"
#include <chrono>

SpringBox::SpringBox() : SpringBox(false) {}

SpringBox::SpringBox(bool is3D) 
    : SpringBox(is3D, std::mt19937(std::chrono::system_clock::now().time_since_epoch().count())) {}

SpringBox::SpringBox(bool is3D, std::mt19937 randomNumberGenerator) 
    : BarnesHutLayout(is3D, randomNumberGenerator) {
    setQuality(0.1);
}

std::string SpringBox::getLayoutAlgorithmName() {
    return "SpringBox";
}

void SpringBox::setQuality(double qualityLevel) {
    BarnesHutLayout::setQuality(qualityLevel);

    if (quality >= 1) {
        viewZone = -1;
    } else if (quality <= 0) {
        viewZone = k;
    } else {
        viewZone = k + (k * 10 * quality);
    }
}

void SpringBox::chooseNodePosition(NodeParticle* n0, NodeParticle* n1) {
    if (n0->isFrozen() || n1->isFrozen()) {
        return;
    }

    double delta = random.nextDouble();
    if (n0->getEdges().size() == 1 && n1->getEdges().size() > 1) {
        auto pos = n1->getPosition();
        n0->moveTo(pos.x + delta, pos.y + delta, pos.z + delta);
    } else if (n1->getEdges().size() == 1 && n0->getEdges().size() > 1) {
        auto pos = n0->getPosition();
        n1->moveTo(pos.x + delta, pos.y + delta, pos.z + delta);
    }
}

NodeParticle* SpringBox::newNodeParticle(const std::string& id) {
    return new SpringBoxNodeParticle(this, id);
}
