#include "SpringBoxNodeParticle.hpp"
#include "SpringBox.hpp"
#include "GraphCellData.hpp"
#include <cmath>

SpringBoxNodeParticle::SpringBoxNodeParticle(SpringBox* box, const std::string& id)
    : NodeParticle(box, id, box->randomXInsideBounds(), box->randomYInsideBounds(), box->is3D() ? box->randomZInsideBounds() : 0) {
    this->box = box;
}

SpringBoxNodeParticle::SpringBoxNodeParticle(SpringBox* box, const std::string& id, double x, double y, double z)
    : NodeParticle(box, id, x, y, z) {}

void SpringBoxNodeParticle::repulsionN2(Vector3& delta) {
    SpringBox* box = static_cast<SpringBox*>(this->box);
    bool is3D = box->is3D();
    ParticleBox* nodes = box->getSpatialIndex();
    Energies* energies = box->getEnergies();
    
    for (auto& node : nodes->getParticles()) {
        SpringBoxNodeParticle* nodeParticle = static_cast<SpringBoxNodeParticle*>(node);
        
        if (nodeParticle != this) {
            delta.set(nodeParticle->pos.x - pos.x, nodeParticle->pos.y - pos.y, is3D ? nodeParticle->pos.z - pos.z : 0);
            double len = delta.normalize();

            if (len > 0) {
                if (len < box->k)
                    len = box->k;

                double factor = ((box->K2 / (len * len)) * nodeParticle->weight);
                energies->accumulateEnergy(factor);
                delta.scalarMult(-factor);
                disp.add(delta);
            }
        }
    }
}

void SpringBoxNodeParticle::repulsionNLogN(Vector3& delta) {
    recurseRepulsion(box->getSpatialIndex()->getNTree()->getRootCell(), delta);
}

void SpringBoxNodeParticle::recurseRepulsion(Cell* cell, Vector3& delta) {
    SpringBox* box = static_cast<SpringBox*>(this->box);
    bool is3D = box->is3D();
    Energies* energies = box->getEnergies();

    if (intersection(cell)) {
        if (cell->isLeaf()) {
            for (auto& particle : cell->getParticles()) {
                SpringBoxNodeParticle* nodeParticle = static_cast<SpringBoxNodeParticle*>(particle);

                if (nodeParticle != this) {
                    delta.set(nodeParticle->pos.x - pos.x, nodeParticle->pos.y - pos.y, is3D ? nodeParticle->pos.z - pos.z : 0);
                    double len = delta.normalize();

                    if (len > 0) {
                        if (len < box->k)
                            len = box->k;
                        double factor = ((box->K2 / (len * len)) * nodeParticle->weight);
                        energies->accumulateEnergy(factor);
                        repE += factor;
                        delta.scalarMult(-factor);
                        disp.add(delta);
                    }
                }
            }
        } else {
            int div = cell->getSpace()->getDivisions();

            for (int i = 0; i < div; i++)
                recurseRepulsion(cell->getSub(i), delta);
        }
    } else {
        if (cell != this->cell) {
            GraphCellData* bary = static_cast<GraphCellData*>(cell->getData());

            double dist = bary->distanceFrom(pos);
            double size = cell->getSpace()->getSize();

            if (!cell->isLeaf() && (size / dist) > box->getBarnesHutTheta()) {
                int div = cell->getSpace()->getDivisions();

                for (int i = 0; i < div; i++)
                    recurseRepulsion(cell->getSub(i), delta);
            } else {
                if (bary->weight != 0) {
                    delta.set(bary->center.x - pos.x, bary->center.y - pos.y, is3D ? bary->center.z - pos.z : 0);
                    double len = delta.normalize();

                    if (len > 0) {
                        if (len < box->k)
                            len = box->k;
                        double factor = ((box->K2 / (len * len)) * (bary->weight));
                        energies->accumulateEnergy(factor);
                        delta.scalarMult(-factor);
                        repE += factor;
                        disp.add(delta);
                    }
                }
            }
        }
    }
}

void SpringBoxNodeParticle::attraction(Vector3& delta) {
    SpringBox* box = static_cast<SpringBox*>(this->box);
    bool is3D = box->is3D();
    Energies* energies = box->getEnergies();
    int neighbourCount = neighbours.size();

    for (EdgeSpring* edge : neighbours) {
        if (!edge->ignored) {
            NodeParticle* other = edge->getOpposite(this);
            Point3 opos = other->getPosition();

            delta.set(opos.x - pos.x, opos.y - pos.y, is3D ? opos.z - pos.z : 0);
            double len = delta.normalize();
            double k = box->k * edge->weight;
            double factor = box->K1 * (len - k);

            delta.scalarMult(factor * (1.0 / (neighbourCount * 0.1)));
            disp.add(delta);
            attE += factor;
            energies->accumulateEnergy(factor);
        }
    }
}

void SpringBoxNodeParticle::gravity(Vector3& delta) {
    SpringBox* box = static_cast<SpringBox*>(this->box);
    bool is3D = box->is3D();

    delta.set(-pos.x, -pos.y, is3D ? -pos.z : 0);
    delta.normalize();
    delta.scalarMult(box->getGravityFactor());
    disp.add(delta);
}

bool SpringBoxNodeParticle::intersection(Cell* cell) {
    SpringBox* box = static_cast<SpringBox*>(this->box);

    double k = box->k;
    double vz = box->getViewZone();

    Anchor* lo = cell->getSpace()->getLoAnchor();
    Anchor* hi = cell->getSpace()->getHiAnchor();

    double x1 = lo->x;
    double x2 = hi->x;
    double X1 = pos.x - (k * vz);
    double X2 = pos.x + (k * vz);

    if (X2 < x1 || X1 > x2)
        return false;

    double y1 = lo->y;
    double y2 = hi->y;
    double Y1 = pos.y - (k * vz);
    double Y2 = pos.y + (k * vz);

    if (Y2 < y1 || Y1 > y2)
        return false;

    double z1 = lo->z;
    double z2 = hi->z;
    double Z1 = pos.z - (k * vz);
    double Z2 = pos.z + (k * vz);

    if (Z2 < z1 || Z1 > z2)
        return false;

    return true;
}
