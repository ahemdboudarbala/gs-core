/*
 * This file is part of GraphStream <http://graphstream-project.org>.
 * 
 * GraphStream is a library whose purpose is to handle static or dynamic
 * graphs, create them from scratch, files, or any source, and display them.
 * 
 * This program is free software distributed under the terms of two licenses: the
 * CeCILL-C license that fits European law, and the GNU Lesser General Public
 * License. You can use, modify, and/or redistribute the software under the terms
 * of the CeCILL-C license as circulated by CEA, CNRS, and INRIA at the following
 * URL <http://www.cecill.info> or under the terms of the GNU LGPL as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-C and LGPL licenses and that you accept their terms.
 */

#ifndef LINLOG_HPP
#define LINLOG_HPP

#include "BarnesHutLayout.hpp"
#include "NodeParticle.hpp"
#include <random>

class LinLog : public BarnesHutLayout {
public:
    LinLog();
    LinLog(bool is3D);
    LinLog(bool is3D, std::mt19937& randomNumberGenerator);

    void configure(double a, double r, bool edgeBased, double force);

    std::string getLayoutAlgorithmName() const override;
    void setQuality(double qualityLevel) override;
    void compute() override;

protected:
    NodeParticle* newNodeParticle(const std::string& id) override;
    void chooseNodePosition(NodeParticle* n0, NodeParticle* n1) override;

private:
    double k = 1.0;
    double aFactor = 1.0f;
    double rFactor = 1.0f;
    bool edgeBased = true;
    double maxR = 0.5;
    double a = 0.0;
    double r = -1.2;
};

#endif // LINLOG_HPP
