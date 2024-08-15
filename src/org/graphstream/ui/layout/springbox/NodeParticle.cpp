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

#include "NodeParticle.hpp"

NodeParticle::NodeParticle(BarnesHutLayout* box, const std::string& id)
    : Particle(id, box->randomXInsideBounds(), box->randomYInsideBounds(), box->is3D ? box->randomZInsideBounds() : 0), box(box), disp() {
    createDebug();
}

NodeParticle::NodeParticle(BarnesHutLayout* box, const std::string& id, double x, double y, double z)
    : Particle(id, x, y, box->is3D ? z : 0), box(box), disp() {
    createDebug();
}

std::vector<EdgeSpring*> NodeParticle::getEdges() const {
    return neighbours;
}

void NodeParticle::move(int time) {
    if (!frozen) {
        disp.fill(0);

        Vector3 delta;

        repE = 0;
        attE = 0;

        if (box->viewZone < 0)
            repulsionN2(delta);
        else
            repulsionNLogN(delta);

        attraction(delta);

        if (box->gravity != 0)
            gravity(delta);

        disp.scalarMult(box->force);

        len = disp.length();

        if (len > (box->area / 2)) {
            disp.scalarMult((box->area / 2) / len);
            len = box->area / 2;
        }

        box->avgLength += len;

        if (len > box->maxMoveLength)
            box->maxMoveLength = len;
    }
}

void NodeParticle::nextStep(int time) {
    if (!frozen) {
        nextPos.x = pos.x + disp.data[0];
        nextPos.y = pos.y + disp.data[1];

        if (box->is3D)
            nextPos.z = pos.z + disp.data[2];

        box->nodeMoveCount++;
        moved = true;
    } else {
        nextPos.x = pos.x;
        nextPos.y = pos.y;
        if (box->is3D)
            nextPos.z = pos.z;
    }

    if (out.is_open()) {
        out << getId() << " " << len << " " << attE << " " << repE << std::endl;
    }

    Particle::nextStep(time);
}

void NodeParticle::moveOf(double dx, double dy, double dz) {
    pos.set(pos.x + dx, pos.y + dy, pos.z + dz);
}

void NodeParticle::moveTo(double x, double y, double z) {
    pos.set(x, y, z);
    moved = true;
}

void NodeParticle::registerEdge(EdgeSpring* e) {
    neighbours.push_back(e);
}

void NodeParticle::unregisterEdge(EdgeSpring* e) {
    auto it = std::find(neighbours.begin(), neighbours.end(), e);
    if (it != neighbours.end()) {
        neighbours.erase(it);
    }
}

void NodeParticle::removeNeighborEdges() {
    std::vector<EdgeSpring*> edges = neighbours;
    for (EdgeSpring* edge : edges)
        box->removeEdge(box->getLayoutAlgorithmName(), edge->id);

    neighbours.clear();
}

void NodeParticle::createDebug() {
    if (box->outputNodeStats) {
        try {
            out.open("out" + getId() + ".data");
        } catch (const std::exception& e) {
            std::cerr << "Error creating debug file: " << e.what() << std::endl;
            std::exit(1);
        }
    }
}

void NodeParticle::inserted() {
    // Implementation for inserted action
}

void NodeParticle::removed() {
    // Implementation for removed action
}
