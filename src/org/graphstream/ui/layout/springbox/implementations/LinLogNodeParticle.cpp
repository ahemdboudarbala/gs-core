#include "LinLogNodeParticle.hpp"
#include "LinLog.hpp"
#include "Vector3.hpp"
#include "EdgeSpring.hpp"
#include "Energies.hpp"
#include "GraphCellData.hpp"
#include "ParticleBox.hpp"

LinLogNodeParticle::LinLogNodeParticle(LinLog* box, const std::string& id)
    : LinLogNodeParticle(box, id, (box->getRandom()->nextDouble() * 2 * box->k) - box->k,
                         (box->getRandom()->nextDouble() * 2 * box->k) - box->k,
                         box->is3D() ? (box->getRandom()->nextDouble() * 2 * box->k) - box->k : 0) {
    this->box = box;
}

LinLogNodeParticle::LinLogNodeParticle(LinLog* box, const std::string& id, double x, double y, double z)
    : NodeParticle(box, id, x, y, z) {
}

void LinLogNodeParticle::repulsionN2(Vector3& delta) {
    LinLog* box = static_cast<LinLog*>(this->box);
    bool is3D = box->is3D();
    ParticleBox* nodes = box->getSpatialIndex();
    Energies* energies = box->getEnergies();
    auto i = nodes->getParticleIdIterator();
    int deg = neighbours.size();

    while (i.hasNext()) {
        LinLogNodeParticle* node = static_cast<LinLogNodeParticle*>(nodes->getParticle(i.next()));

        if (node != this) {
            delta.set(node->pos.x - pos.x, node->pos.y - pos.y, is3D ? node->pos.z - pos.z : 0);

            double len = delta.length();

            if (len > 0) {
                double degFactor = box->edgeBased ? deg * node->neighbours.size() : 1;
                double factor = 1;
                double r = box->r;

                factor = -degFactor * (std::pow(len, r - 2)) * node->weight * weight * box->rFactor;

                if (factor < -box->maxR) {
                    factor = -box->maxR;
                }

                energies->accumulateEnergy(factor);
                delta.scalarMult(factor);
                disp.add(delta);
                repE += factor;
            }
        }
    }
}

void LinLogNodeParticle::repulsionNLogN(Vector3& delta) {
    recurseRepulsion(box->getSpatialIndex()->getNTree()->getRootCell(), delta);
}

void LinLogNodeParticle::recurseRepulsion(Cell* cell, Vector3& delta) {
    LinLog* box = static_cast<LinLog*>(this->box);
    bool is3D = box->is3D();
    Energies* energies = box->getEnergies();
    int deg = neighbours.size();

    if (intersection(cell)) {
        if (cell->isLeaf()) {
            auto i = cell->getParticles();

            while (i.hasNext()) {
                LinLogNodeParticle* node = static_cast<LinLogNodeParticle*>(i.next());

                if (node != this) {
                    delta.set(node->pos.x - pos.x, node->pos.y - pos.y, is3D ? node->pos.z - pos.z : 0);

                    double len = delta.length();

                    if (len > 0) {
                        double degFactor = box->edgeBased ? deg * node->neighbours.size() : 1;
                        double factor = 1;
                        double r = box->r;

                        factor = -degFactor * (std::pow(len, r - 2)) * node->weight * weight * box->rFactor;

                        if (factor < -box->maxR) {
                            factor = -box->maxR;
                        }

                        energies->accumulateEnergy(factor);
                        delta.scalarMult(factor);
                        disp.add(delta);
                        repE += factor;
                    }
                }
            }
        } else {
            int div = cell->getSpace()->getDivisions();

            for (int i = 0; i < div; i++) {
                recurseRepulsion(cell->getSub(i), delta);
            }
        }
    } else {
        if (cell != this->cell) {
            GraphCellData* bary = static_cast<GraphCellData*>(cell->getData());
            double dist = bary->distanceFrom(pos);
            double size = cell->getSpace()->getSize();

            if ((!cell->isLeaf()) && ((size / dist) > box->getBarnesHutTheta())) {
                int div = cell->getSpace()->getDivisions();

                for (int i = 0; i < div; i++) {
                    recurseRepulsion(cell->getSub(i), delta);
                }
            } else {
                if (bary->weight != 0) {
                    delta.set(bary->center.x - pos.x, bary->center.y - pos.y, is3D ? bary->center.z - pos.z : 0);

                    double len = delta.length();

                    if (len > 0) {
                        double degFactor = box->edgeBased ? deg * bary->degree : 1;
                        double factor = 1;
                        double r = box->r;

                        factor = -degFactor * (std::pow(len, r - 2)) * bary->weight * weight * box->rFactor;

                        if (factor < -box->maxR) {
                            factor = -box->maxR;
                        }

                        energies->accumulateEnergy(factor);
                        delta.scalarMult(factor);
                        disp.add(delta);
                        repE += factor;
                    }
                }
            }
        }
    }
}

void LinLogNodeParticle::attraction(Vector3& delta) {
    LinLog* box = static_cast<LinLog*>(this->box);
    bool is3D = box->is3D();
    Energies* energies = box->getEnergies();

    for (EdgeSpring* edge : neighbours) {
        if (!edge->ignored) {
            LinLogNodeParticle* other = static_cast<LinLogNodeParticle*>(edge->getOpposite(this));

            delta.set(other->pos.x - pos.x, other->pos.y - pos.y, is3D ? other->pos.z - pos.z : 0);

            double len = delta.length();

            if (len > 0) {
                double factor = 1;
                double a = box->a;

                factor = (std::pow(len, a - 2)) * edge->weight * box->aFactor;

                energies->accumulateEnergy(factor);
                delta.scalarMult(factor);
                disp.add(delta);
                attE += factor;
            }
        }
    }
}

void LinLogNodeParticle::gravity(Vector3& delta) {
    // No implementation required as per the original Java code
}

bool LinLogNodeParticle::intersection(Cell* cell) {
    LinLog* box = static_cast<LinLog*>(this->box);

    double k = box->k;
    double vz = box->getViewZone();

    double x1 = cell->getSpace()->getLoAnchor().x;
    double y1 = cell->getSpace()->getLoAnchor().y;
    double z1 = cell->getSpace()->getLoAnchor().z;

    double x2 = cell->getSpace()->getHiAnchor().x;
    double y2 = cell->getSpace()->getHiAnchor().y;
    double z2 = cell->getSpace()->getHiAnchor().z;

    double X1 = pos.x - (k * vz);
    double Y1 = pos.y - (k * vz);
    double Z1 = pos.z - (k * vz);
    double X2 = pos.x + (k * vz);
    double Y2 = pos.y + (k * vz);
    double Z2 = pos.z + (k * vz);

    if (X2 < x1 || X1 > x2) return false;
    if (Y2 < y1 || Y1 > y2) return false;
    if (Z2 < z1 || Z1 > z2) return false;

    return true;
}
