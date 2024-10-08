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

#include "GraphCellData.hpp"
#include <cassert>

GraphCellData::GraphCellData() : degree(0) {}

double GraphCellData::getDegree() const {
    return degree;
}

CellData* GraphCellData::newCellData() {
    return new GraphCellData();
}

void GraphCellData::recompute() {
    double x = 0;
    double y = 0;
    double z = 0;
    double n = 0;

    weight = 0;
    degree = 0;

    if (cell->isLeaf()) {
        for (auto& particle : cell->getParticles()) {
            NodeParticle* nodeParticle = static_cast<NodeParticle*>(particle);

            x += nodeParticle->getPosition().x;
            y += nodeParticle->getPosition().y;
            z += nodeParticle->getPosition().z;

            weight += nodeParticle->getWeight();
            degree += nodeParticle->getEdges().size();

            n++;
        }

        if (n > 0) {
            x /= n;
            y /= n;
            z /= n;
        }

        center.set(x, y, z);
    } else {
        double subcnt = cell->getSpace()->getDivisions();
        double totpop = cell->getPopulation();
        int verif = 0;

        if (totpop > 0) {
            for (int i = 0; i < subcnt; ++i) {
                Cell* subcell = cell->getSub(i);
                GraphCellData* data = static_cast<GraphCellData*>(subcell->getData());
                double pop = subcell->getPopulation();

                verif += pop;

                x += data->center.x * pop;
                y += data->center.y * pop;
                z += data->center.z * pop;

                weight += data->weight;
                degree += data->degree;
            }

            assert(verif == totpop && "Discrepancy in population counts?");

            x /= totpop;
            y /= totpop;
            z /= totpop;
        }

        center.set(x, y, z);
    }

    for (auto& listener : cell->getTree()->getListeners()) {
        listener->cellData(cell->getId(), "barycenter", this);
    }
}
