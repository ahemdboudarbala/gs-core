/*
 * This file is part of GraphStream <http://graphstream-project.org>.
 *
 * GraphStream is a library whose purpose is to handle static or dynamic
 * graphs, create them from scratch, file, or any source, and display them.
 *
 * This program is free software distributed under the terms of two licenses, the
 * CeCILL-C license that fits European law, and the GNU Lesser General Public
 * License. You can use, modify and/or redistribute the software under the terms
 * of the CeCILL-C license as circulated by CEA, CNRS, and INRIA at the following
 * URL <http://www.cecill.info> or under the terms of the GNU LGPL as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-C and LGPL licenses and that you accept their terms.
 */

#include "BarnesHutLayout.hpp"
#include <iostream>
#include <cmath>
#include <chrono>
#include <algorithm>

BarnesHutLayout::BarnesHutLayout(bool is3D, std::mt19937 randomGenerator)
    : is3D(is3D), random(randomGenerator), lo(0, 0, 0), hi(1, 1, 1), center(0.5, 0.5, 0.5) {
    // Initialization code
    CellSpace* space;

    if (is3D) {
        space = new OctreeCellSpace(Anchor(-1, -1, -1), Anchor(1, 1, 1));
    } else {
        space = new QuadtreeCellSpace(Anchor(-1, -1, -0.01f), Anchor(1, 1, 0.01f));
    }

    nodes = ParticleBox(nodesPerCell, space, new GraphCellData());
    nodes.addParticleBoxListener(this);

    setQuality(quality);
}

Point3 BarnesHutLayout::getLowPoint() {
    auto p = nodes.getNTree()->getLowestPoint();
    lo = Point3(p.x, p.y, p.z);
    return lo;
}

Point3 BarnesHutLayout::getHiPoint() {
    auto p = nodes.getNTree()->getHighestPoint();
    hi = Point3(p.x, p.y, p.z);
    return hi;
}

double BarnesHutLayout::randomXInsideBounds() {
    auto c = dynamic_cast<GraphCellData*>(nodes.getNTree()->getRootCell()->getData())->center;
    return c.x + (random() * 2.0 - 1.0);
}

double BarnesHutLayout::randomYInsideBounds() {
    auto c = dynamic_cast<GraphCellData*>(nodes.getNTree()->getRootCell()->getData())->center;
    return c.y + (random() * 2.0 - 1.0);
}

double BarnesHutLayout::randomZInsideBounds() {
    auto c = dynamic_cast<GraphCellData*>(nodes.getNTree()->getRootCell()->getData())->center;
    return c.z + (random() * 2.0 - 1.0);
}

Point3 BarnesHutLayout::getCenterPoint() {
    return center;
}

double BarnesHutLayout::getGravityFactor() const {
    return gravity;
}

void BarnesHutLayout::setGravityFactor(double value) {
    gravity = value;
}

ParticleBox* BarnesHutLayout::getSpatialIndex() {
    return &nodes;
}

long BarnesHutLayout::getLastStepTime() const {
    return lastStepTime;
}

int BarnesHutLayout::getNodeMovedCount() const {
    return nodeMoveCount;
}

double BarnesHutLayout::getStabilization() const {
    if (lastElementCount == nodes.getParticleCount() + edges.size()) {
        if (time > energies.getBufferSize()) {
            return energies.getStabilization();
        }
    }
    lastElementCount = nodes.getParticleCount() + edges.size();
    return 0;
}

double BarnesHutLayout::getStabilizationLimit() const {
    return stabilizationLimit;
}

int BarnesHutLayout::getSteps() const {
    return time;
}

double BarnesHutLayout::getQuality() const {
    return quality;
}

bool BarnesHutLayout::is3D() const {
    return is3D;
}

double BarnesHutLayout::getForce() const {
    return force;
}

std::mt19937 BarnesHutLayout::getRandom() const {
    return random;
}

Energies* BarnesHutLayout::getEnergies() {
    return &energies;
}

double BarnesHutLayout::getBarnesHutTheta() const {
    return theta;
}

double BarnesHutLayout::getViewZone() const {
    return viewZone;
}

void BarnesHutLayout::setSendNodeInfos(bool on) {
    sendNodeInfos = on;
}

void BarnesHutLayout::setBarnesHutTheta(double theta) {
    if (theta > 0 && theta < 1) {
        this->theta = theta;
    }
}

void BarnesHutLayout::setForce(double value) {
    force = value;
}

void BarnesHutLayout::setStabilizationLimit(double value) {
    stabilizationLimit = value;
}

void BarnesHutLayout::setQuality(double qualityLevel) {
    quality = std::clamp(qualityLevel, 0.0, 1.0);
}

void BarnesHutLayout::clear() {
    energies.clearEnergies();
    nodes.removeAllParticles();
    edges.clear();
    nodeMoveCount = 0;
    lastStepTime = 0;
}

void BarnesHutLayout::compute() {
    auto t1 = std::chrono::high_resolution_clock::now();

    computeArea();

    maxMoveLength = std::numeric_limits<double>::min();
    nodeMoveCount = 0;
    avgLength = 0;

    nodes.step();

    if (nodeMoveCount > 0)
        avgLength /= nodeMoveCount;

    getLowPoint();
    getHiPoint();
    center = Point3(lo.x + (hi.x - lo.x) / 2, lo.y + (hi.y - lo.y) / 2, lo.z + (hi.z - lo.z) / 2);

    energies.storeEnergy();
    printStats();
    time++;

    auto t2 = std::chrono::high_resolution_clock::now();
    lastStepTime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}

void BarnesHutLayout::printStats() {
    if (outputStats) {
        if (!statsOut.is_open()) {
            statsOut.open("springBox.dat");
            statsOut << "# stabilization nodeMoveCount energy energyDiff maxMoveLength avgLength area\n";
            statsOut.flush();
        }

        if (statsOut.is_open()) {
            double energyDiff = energies.getEnergy() - energies.getPreviousEnergyValue(30);
            statsOut << getStabilization() << " " << nodeMoveCount << " " << energies.getEnergy() << " " << energyDiff
                     << " " << maxMoveLength << " " << avgLength << " " << area << "\n";
            statsOut.flush();
        }
    }
}

void BarnesHutLayout::computeArea() {
    area = getHiPoint().distance(getLowPoint());
}

void BarnesHutLayout::shake() {
    energies.clearEnergies();
}

NodeParticle* BarnesHutLayout::addNode(const std::string& sourceId, const std::string& id) {
    NodeParticle* np = newNodeParticle(id);
    nodes.addParticle(np);
    return np;
}

void BarnesHutLayout::moveNode(const std::string& id, double x, double y, double z) {
    NodeParticle* node = static_cast<NodeParticle*>(nodes.getParticle(id));
    if (node != nullptr) {
        node->moveTo(x, y, z);
        energies.clearEnergies();
    }
}

void BarnesHutLayout::freezeNode(const std::string& id, bool on) {
    NodeParticle* node = static_cast<NodeParticle*>(nodes.getParticle(id));
    if (node != nullptr) {
        node->setFrozen(on);
    }
}

void BarnesHutLayout::setNodeWeight(const std::string& id, double weight) {
    NodeParticle* node = static_cast<NodeParticle*>(nodes.getParticle(id));
    if (node != nullptr) {
        node->setWeight(weight);
    }
}

void BarnesHutLayout::removeNode(const std::string& sourceId, const std::string& id) {
    NodeParticle* node = static_cast<NodeParticle*>(nodes.removeParticle(id));
    if (node != nullptr) {
        node->removeNeighborEdges();
    } else {
        std::cerr << "Layout " << getLayoutAlgorithmName() << ": cannot remove non-existing node " << id << std::endl;
    }
}

void BarnesHutLayout::addEdge(const std::string& sourceId, const std::string& id, const std::string& from, const std::string& to, bool directed) {
    NodeParticle* n0 = static_cast<NodeParticle*>(nodes.getParticle(from));
    NodeParticle* n1 = static_cast<NodeParticle*>(nodes.getParticle(to));
    if (n0 != nullptr && n1 != nullptr) {
        EdgeSpring* e = new EdgeSpring(id, n0, n1);
        EdgeSpring* o = edges[id];
        if (o != nullptr) {
            std::cerr << "Layout " << getLayoutAlgorithmName() << ": edge '" << id << "' already exists." << std::endl;
        } else {
            n0->registerEdge(e);
            n1->registerEdge(e);
        }
        chooseNodePosition(n0, n1);
    } else {
        if (n0 == nullptr)
            std::cerr << "Layout " << getLayoutAlgorithmName() << ": node '" << from << "' does not exist, cannot create edge " << id << "." << std::endl;
        if (n1 == nullptr)
            std::cerr << "Layout " << getLayoutAlgorithmName() << ": node '" << to << "' does not exist, cannot create edge " << id << "." << std::endl;
    }
}

void BarnesHutLayout::addEdgeBreakPoint(const std::string& edgeId, int points) {
    std::cerr << "Layout " << getLayoutAlgorithmName() << ": edge break points are not handled yet." << std::endl;
}

void BarnesHutLayout::ignoreEdge(const std::string& edgeId, bool on) {
    EdgeSpring* edge = edges[edgeId];
    if (edge != nullptr) {
        edge->ignored = on;
    }
}

void BarnesHutLayout::setEdgeWeight(const std::string& id, double weight) {
    EdgeSpring* edge = edges[id];
    if (edge != nullptr) {
        edge->weight = weight;
    }
}

void BarnesHutLayout::removeEdge(const std::string& sourceId, const std::string& id) {
    EdgeSpring* e = edges[id];
    if (e != nullptr) {
        e->node0->unregisterEdge(e);
        e->node1->unregisterEdge(e);
        edges.erase(id);
    } else {
        std::cerr << "Layout " << getLayoutAlgorithmName() << ": cannot remove non-existing edge " << id << std::endl;
    }
}

void BarnesHutLayout::particleAdded(void* id, double x, double y, double z) {
    // Implementation
}

void BarnesHutLayout::particleMarked(void* id, void* mark) {
    // Implementation
}

void BarnesHutLayout::particleMoved(void* id, double x, double y, double z) {
    if ((time % sendMoveEventsEvery) == 0) {
        double xyz[] = {x, y, z};
        // sendNodeAttributeChanged(sourceId, (std::string)id, "xyz", xyz, xyz);
    }
}

void BarnesHutLayout::particleRemoved(void* id) {
    // Implementation
}

void BarnesHutLayout::stepFinished(int time) {
    // Implementation
}
