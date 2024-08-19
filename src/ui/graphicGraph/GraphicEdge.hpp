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

#ifndef GRAPHIC_EDGE_HPP
#define GRAPHIC_EDGE_HPP

#include "GraphicElement.hpp"
#include "GraphicNode.hpp"
#include <vector>
#include <memory>

class GraphicEdge : public GraphicElement {
public:
    class EdgeGroup;

    GraphicEdge(const std::string& id, std::shared_ptr<GraphicNode> from, std::shared_ptr<GraphicNode> to, 
                bool directed, const std::unordered_map<std::string, std::any>& attributes = {});

    virtual ~GraphicEdge() = default;

    Selector::Type getSelectorType() const override;

    std::shared_ptr<GraphicNode> otherNode(const std::shared_ptr<GraphicNode>& node) const;

    double getX() const override;
    double getY() const override;
    double getZ() const override;

    const std::vector<double>& getControlPoints() const;
    void setControlPoints(const std::vector<double>& points);

    bool isCurve() const;
    int getMultiIndex() const;

    void move(double x, double y, double z) override;
    void removed() override;

    std::shared_ptr<GraphicNode> getNode0() const;
    std::shared_ptr<GraphicNode> getNode1() const;
    std::shared_ptr<EdgeGroup> getGroup() const;

    std::shared_ptr<GraphicNode> getOpposite(const std::shared_ptr<Node>& node) const;
    std::shared_ptr<GraphicNode> getSourceNode() const;
    std::shared_ptr<GraphicNode> getTargetNode() const;

    bool isDirected() const;
    bool isLoop() const;
    void setDirected(bool on);
    void switchDirection();

protected:
    void attributeChanged(AttributeChangeEvent event, const std::string& attribute, 
                          const std::any& oldValue, const std::any& newValue) override;

private:
    std::shared_ptr<GraphicNode> from;
    std::shared_ptr<GraphicNode> to;
    bool directed;
    int multi;
    std::shared_ptr<EdgeGroup> group;
    std::vector<double> ctrl;
};

class GraphicEdge::EdgeGroup {
public:
    EdgeGroup(const std::shared_ptr<GraphicEdge>& first, const std::shared_ptr<GraphicEdge>& second);

    std::shared_ptr<GraphicEdge> getEdge(int i) const;
    int getCount() const;
    void increment(const std::shared_ptr<GraphicEdge>& edge);
    void decrement(const std::shared_ptr<GraphicEdge>& edge);

private:
    std::vector<std::shared_ptr<GraphicEdge>> edges;
};

#endif // GRAPHIC_EDGE_HPP
