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
 * @author Guilhelm Savin <guilhelm.savin@graphstream-project.org>
 * @author Yoann Pigné <yoann.pigne@graphstream-project.org>
 * @author Antoine Dutot <antoine.dutot@graphstream-project.org>
 * @author kitskub <kitskub@gmail.com>
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 */

#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <string>
#include "Pipe.hpp"
#include "Point3.hpp"

class Layout : public Pipe {
public:
    virtual ~Layout() {}

    // Name of the layout algorithm.
    virtual std::string getLayoutAlgorithmName() const = 0;

    // How many nodes moved during the last step? When this method returns zero, the layout stabilized.
    virtual int getNodeMovedCount() const = 0;

    // Estimate of how close to stabilization the layout algorithm is.
    virtual double getStabilization() const = 0;

    // Above which value a correct stabilization is achieved?
    virtual double getStabilizationLimit() const = 0;

    // Smallest point in space of the layout bounding box.
    virtual Point3 getLowPoint() const = 0;

    // Largest point in space of the layout bounding box.
    virtual Point3 getHiPoint() const = 0;

    // Number of calls made to step() so far.
    virtual int getSteps() const = 0;

    // Time in nanoseconds used by the last call to step().
    virtual long getLastStepTime() const = 0;

    // The current layout algorithm quality. A number between 0 and 1 with 1 the highest quality.
    virtual double getQuality() const = 0;

    // The current layout force.
    virtual double getForce() const = 0;

    // Clears the whole nodes and edges structures.
    virtual void clear() = 0;

    // The general "speed" of the algorithm. A number in [0..1].
    virtual void setForce(double value) = 0;

    // Change the stabilization limit for this layout algorithm.
    virtual void setStabilizationLimit(double value) = 0;

    // Set the overall quality level, a number between 0 and 1 with 1 the highest quality available.
    virtual void setQuality(double qualityLevel) = 0;

    // If true, node information messages are sent for every node.
    virtual void setSendNodeInfos(bool send) = 0;

    // Add a random vector whose length is 10% of the size of the graph to all node positions.
    virtual void shake() = 0;

    // Move a node by force to a new location.
    virtual void moveNode(const std::string& id, double x, double y, double z) = 0;

    // Freeze or un-freeze a node.
    virtual void freezeNode(const std::string& id, bool frozen) = 0;

    // Method to call repeatedly to compute the layout.
    virtual void compute() = 0;
};

#endif // LAYOUT_HPP
