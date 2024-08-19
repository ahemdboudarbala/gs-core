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
 * @author kitskub <kitskub@gmail.com>
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 */

#ifndef NODEPARTICLE_HPP
#define NODEPARTICLE_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Vector3.hpp"
#include "Particle.hpp"
#include "BarnesHutLayout.hpp"
#include "EdgeSpring.hpp"

class NodeParticle : public Particle {
public:
    std::vector<EdgeSpring*> neighbours;
    bool frozen = false;
    Vector3 disp;
    double len;
    double attE;
    double repE;
    std::ofstream out;

protected:
    BarnesHutLayout* box;

public:
    NodeParticle(BarnesHutLayout* box, const std::string& id);
    NodeParticle(BarnesHutLayout* box, const std::string& id, double x, double y, double z);

    virtual ~NodeParticle() = default;

    std::vector<EdgeSpring*> getEdges() const;

    virtual void move(int time) override;
    virtual void nextStep(int time) override;

    void moveOf(double dx, double dy, double dz);
    void moveTo(double x, double y, double z);
    void registerEdge(EdgeSpring* e);
    void unregisterEdge(EdgeSpring* e);
    void removeNeighborEdges();

protected:
    virtual void repulsionN2(Vector3& delta) = 0;
    virtual void repulsionNLogN(Vector3& delta) = 0;
    virtual void attraction(Vector3& delta) = 0;
    virtual void gravity(Vector3& delta) = 0;
    void createDebug();

    virtual void inserted() override;
    virtual void removed() override;
};

#endif // NODEPARTICLE_HPP
