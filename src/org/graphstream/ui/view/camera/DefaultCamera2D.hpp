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
 * @since 2018-01-18
 * 
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 * @author Yoann Pigné <yoann.pigne@graphstream-project.org>
 */

#ifndef GRAPHSTREAM_UI_VIEW_CAMERA_DEFAULTCAMERA2D_HPP
#define GRAPHSTREAM_UI_VIEW_CAMERA_DEFAULTCAMERA2D_HPP

#include "Camera.hpp"
#include "Point3.hpp"
#include "GraphMetrics.hpp"
#include "Backend.hpp"
#include "GraphicGraph.hpp"
#include <unordered_set>
#include <vector>
#include <string>

class DefaultCamera2D : public Camera {
public:
    DefaultCamera2D(GraphicGraph* graph);

    Point3 getViewCenter() override;
    void setViewCenter(double x, double y, double z) override;
    void setViewCenter(const Point3& p);
    double getViewPercent() override;
    void setViewPercent(double percent) override;
    double getViewRotation() override;
    void setViewRotation(double theta) override;
    void setViewport(double x, double y, double viewportWidth, double viewportHeight);
    double getGraphDimension() override;
    void setBounds(double minx, double miny, double minz, double maxx, double maxy, double maxz);
    void setBounds(GraphicGraph* graph);
    GraphMetrics* getMetrics() override;
    void setAutoFitView(bool on);
    void setBackend(Backend* backend);
    void removeGraphViewport() override;
    void setGraphViewport(double minx, double miny, double maxx, double maxy) override;
    void resetView() override;
    void pushView(GraphicGraph* graph);
    void popView();
    bool isVisible(GraphicElement* element);

private:
    GraphicGraph* graph;
    GraphMetrics metrics;
    Point3 center;
    double zoom = 1;
    double rotation = 0;
    Values padding = Values(Units::GU, 0, 0, 0);
    Backend* bck = nullptr;
    std::unordered_set<std::string> nodeInvisible;
    std::unordered_set<std::string> spriteInvisible;
    std::vector<double> gviewport;
    bool autoFit = true;

    void setPadding(GraphicGraph* graph);
    void autoFitView();
    void userView();
    double paddingXgu();
    double paddingYgu();
    double paddingXpx();
    double paddingYpx();
};

#endif // GRAPHSTREAM_UI_VIEW_CAMERA_DEFAULTCAMERA2D_HPP
