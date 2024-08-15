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
 * @since 2009-07-26
 * 
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 * @author Yoann Pigné <yoann.pigne@graphstream-project.org>
 */

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Point3.hpp"
#include "GraphicElement.hpp"
#include "GraphicGraph.hpp"
#include "GraphMetrics.hpp"
#include "InteractiveElement.hpp"
#include <vector>
#include <set>

class Camera {
public:
    virtual Point3 getViewCenter() = 0;
    virtual void setViewCenter(double x, double y, double z) = 0;
    virtual double getViewPercent() = 0;
    virtual void setViewPercent(double percent) = 0;
    virtual double getViewRotation() = 0;
    virtual void setViewRotation(double theta) = 0;
    virtual double getGraphDimension() = 0;
    virtual void removeGraphViewport() = 0;
    virtual void setGraphViewport(double minx, double miny, double maxx, double maxy) = 0;
    virtual void resetView() = 0;
    virtual void setBounds(double minx, double miny, double minz, double maxx, double maxy, double maxz) = 0;
    virtual GraphMetrics getMetrics() = 0;
    virtual void setAutoFitView(bool on) = 0;
    virtual Point3 transformGuToPx(double x, double y, double z) = 0;
    virtual Point3 transformPxToGu(double x, double y) = 0;
    virtual bool isVisible(GraphicElement* element) = 0;
    virtual GraphicElement* findGraphicElementAt(GraphicGraph* graph, std::set<InteractiveElement> types, double x, double y) = 0;
    virtual std::vector<GraphicElement*> allGraphicElementsIn(GraphicGraph* graph, std::set<InteractiveElement> types, double x1, double y1, double x2, double y2) = 0;

    virtual ~Camera() = default;
};

#endif // CAMERA_HPP
