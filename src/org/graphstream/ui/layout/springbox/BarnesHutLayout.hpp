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

/**
 * @since 2009-02-19
 *
 * @author Antoine Dutot <antoine.dutot@graphstream-project.org>
 * @author Guilhelm Savin <guilhelm.savin@graphstream-project.org>
 * @author Alex Bowen <bowen.a@gmail.com>
 * @author kitskub <kitskub@gmail.com>
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 */

#ifndef BARNESHUTLAYOUT_HPP
#define BARNESHUTLAYOUT_HPP

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include "Point3.hpp"
#include "NodeParticle.hpp"
#include "EdgeSpring.hpp"
#include "Energies.hpp"
#include "ParticleBox.hpp"
#include "ParticleBoxListener.hpp"

class BarnesHutLayout : public ParticleBoxListener {
public:
    BarnesHutLayout(bool is3D = false, std::mt19937 randomGenerator = std::mt19937(std::random_device()()));
    virtual ~BarnesHutLayout() = default;

    Point3 getLowPoint();
    Point3 getHiPoint();
    double randomXInsideBounds();
    double randomYInsideBounds();
    double randomZInsideBounds();
    Point3 getCenterPoint();
    double getGravityFactor() const;
    void setGravityFactor(double value);
    ParticleBox* getSpatialIndex();
    long getLastStepTime() const;
    virtual std::string getLayoutAlgorithmName() const = 0;
    int getNodeMovedCount() const;
    double getStabilization() const;
    double getStabilizationLimit() const;
    int getSteps() const;
    double getQuality() const;
    bool is3D() const;
    double getForce() const;
    std::mt19937 getRandom() const;
    Energies* getEnergies();
    double getBarnesHutTheta() const;
    double getViewZone() const;
    void setSendNodeInfos(bool on);
    void setBarnesHutTheta(double theta);
    void setForce(double value);
    void setStabilizationLimit(double value);
    void setQuality(double qualityLevel);
    void clear();
    void compute();
    void shake();
    virtual NodeParticle* newNodeParticle(const std::string& id) = 0;

protected:
    virtual void chooseNodePosition(NodeParticle* n0, NodeParticle* n1) = 0;

private:
    void printStats();
    void computeArea();
    NodeParticle* addNode(const std::string& sourceId, const std::string& id);
    void moveNode(const std::string& id, double x, double y, double z);
    void freezeNode(const std::string& id, bool on);
    void setNodeWeight(const std::string& id, double weight);
    void removeNode(const std::string& sourceId, const std::string& id);
    void addEdge(const std::string& sourceId, const std::string& id, const std::string& from, const std::string& to, bool directed);
    void addEdgeBreakPoint(const std::string& edgeId, int points);
    void ignoreEdge(const std::string& edgeId, bool on);
    void setEdgeWeight(const std::string& id, double weight);
    void removeEdge(const std::string& sourceId, const std::string& id);

    // ParticleBoxListener methods
    void particleAdded(void* id, double x, double y, double z) override;
    void particleMarked(void* id, void* mark) override;
    void particleMoved(void* id, double x, double y, double z) override;
    void particleRemoved(void* id) override;
    void stepFinished(int time) override;

protected:
    ParticleBox nodes;
    std::unordered_map<std::string, EdgeSpring*> edges;
    int lastElementCount = 0;
    std::mt19937 random;
    Point3 lo, hi, center;
    std::ofstream statsOut;
    Energies energies;
    double force = 1.0;
    double viewZone = 5.0;
    double theta = 0.7;
    double quality = 1.0;
    int nodesPerCell = 10;
    double area = 1.0;
    double stabilizationLimit = 0.9;
    int time = 0;
    long lastStepTime = 0;
    double maxMoveLength = 0.0;
    double avgLength = 0.0;
    int nodeMoveCount = 0;
    bool is3D = false;
    double gravity = 0.0;
    bool sendNodeInfos = false;
    bool outputStats = false;
    bool outputNodeStats = false;
    int sendMoveEventsEvery = 1;
};

#endif // BARNESHUTLAYOUT_HPP
