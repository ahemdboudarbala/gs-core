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

#include "DefaultCamera2D.hpp"

DefaultCamera2D::DefaultCamera2D(GraphicGraph* graph) : graph(graph) {}

Point3 DefaultCamera2D::getViewCenter() {
    return center;
}

void DefaultCamera2D::setViewCenter(double x, double y, double z) {
    setAutoFitView(false);
    center.set(x, y, z);
    graph->graphChanged = true;
}

void DefaultCamera2D::setViewCenter(const Point3& p) {
    setViewCenter(p.x, p.y, p.z);
}

double DefaultCamera2D::getViewPercent() {
    return zoom;
}

void DefaultCamera2D::setViewPercent(double percent) {
    setAutoFitView(false);
    setZoom(percent);
    graph->graphChanged = true;
}

void DefaultCamera2D::setZoom(double z) {
    zoom = z;
    graph->graphChanged = true;
}

double DefaultCamera2D::getViewRotation() {
    return rotation;
}

void DefaultCamera2D::setViewRotation(double theta) {
    rotation = theta;
    graph->graphChanged = true;
}

void DefaultCamera2D::setViewport(double x, double y, double viewportWidth, double viewportHeight) {
    metrics.setViewport(x, y, viewportWidth, viewportHeight);
}

double DefaultCamera2D::getGraphDimension() {
    return metrics.getDiagonal();
}

void DefaultCamera2D::setBounds(double minx, double miny, double minz, double maxx, double maxy, double maxz) {
    metrics.setBounds(minx, miny, minz, maxx, maxy, maxz);
}

void DefaultCamera2D::setBounds(GraphicGraph* graph) {
    setBounds(graph->getMinPos().x, graph->getMinPos().y, 0, graph->getMaxPos().x, graph->getMaxPos().y, 0);
}

GraphMetrics* DefaultCamera2D::getMetrics() {
    return &metrics;
}

void DefaultCamera2D::setAutoFitView(bool on) {
    if (autoFit && !on) {
        zoom = 1;
        center.set(metrics.lo.x + (metrics.size.data[0] / 2), metrics.lo.y + (metrics.size.data[1] / 2), 0);
    }
    autoFit = on;
}

void DefaultCamera2D::setBackend(Backend* backend) {
    this->bck = backend;
}

void DefaultCamera2D::removeGraphViewport() {
    gviewport.clear();
}

void DefaultCamera2D::setGraphViewport(double minx, double miny, double maxx, double maxy) {
    gviewport = { minx, miny, maxx, maxy };
}

void DefaultCamera2D::resetView() {
    setAutoFitView(true);
    setViewRotation(0);
}

void DefaultCamera2D::pushView(GraphicGraph* graph) {
    bck->pushTransform();
    setPadding(graph);

    if (autoFit)
        autoFitView();
    else
        userView();

    checkVisibility(graph);
}

void DefaultCamera2D::popView() {
    bck->popTransform();
}

bool DefaultCamera2D::isVisible(GraphicElement* element) {
    if (autoFit) {
        return !element->hidden && element->style.getVisibilityMode() != StyleConstants::VisibilityMode::HIDDEN;
    } else {
        switch (element->getSelectorType()) {
        case SelectorType::NODE:
            return nodeInvisible.find(element->getId()) == nodeInvisible.end();
        case SelectorType::EDGE:
            return isEdgeVisible(static_cast<GraphicEdge*>(element));
        case SelectorType::SPRITE:
            return spriteInvisible.find(element->getId()) == spriteInvisible.end();
        default:
            return false;
        }
    }
}

double DefaultCamera2D::paddingXgu() {
    if (padding.units == Units::GU && padding.size() > 0)
        return padding.get(0);
    else
        return 0;
}

double DefaultCamera2D::paddingYgu() {
    if (padding.units == Units::GU && padding.size() > 1)
        return padding.get(1);
    else
        return paddingXgu();
}

double DefaultCamera2D::paddingXpx() {
    if (padding.units == Units::PX && padding.size() > 0)
        return padding.get(0);
    else
        return 0;
}

double DefaultCamera2D::paddingYpx() {
    if (padding.units == Units::PX && padding.size() > 1)
        return padding.get(1);
    else
        return paddingXpx();
}

// Other private methods omitted for brevity.
