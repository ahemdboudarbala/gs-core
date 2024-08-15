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

#ifndef GRAPHIC_NODE_HPP
#define GRAPHIC_NODE_HPP

#include <string>
#include <map>
#include <vector>
#include <memory>
#include "GraphicElement.hpp"
#include "GraphicEdge.hpp"
#include "Point3.hpp"

class GraphicNode : public GraphicElement {
public:
    // Constructors
    GraphicNode(std::shared_ptr<GraphicGraph> graph, const std::string& id, 
                const std::map<std::string, std::any>& attributes = {});

    // Access methods
    virtual Selector::Type getSelectorType() const override;
    virtual double getX() const override;
    virtual double getY() const override;
    virtual double getZ() const override;

    std::shared_ptr<Point3> getPosition() const;
    void moveFromEvent(double x, double y, double z);
    virtual void move(double x, double y, double z) override;

protected:
    double x, y, z;
    bool positioned;

    virtual void attributeChanged(const AttributeChangeEvent& event, const std::string& attribute, 
                                  const std::any& oldValue, const std::any& newValue) override;
    virtual void removed() override;

private:
    double numberAttribute(const std::any& value) const;

    // Node interface methods
public:
    int getDegree() const;
    std::shared_ptr<GraphicEdge> getEdge(int i) const;
    std::shared_ptr<GraphicEdge> getEdgeBetween(const std::string& id) const;
    std::shared_ptr<GraphicEdge> getEdgeFrom(const std::string& id) const;
    std::shared_ptr<GraphicEdge> getEdgeToward(const std::string& id) const;
    std::shared_ptr<Graph> getGraph() const;
    
    int getInDegree() const;
    int getOutDegree() const;
    
    bool hasEdgeBetween(const std::string& id) const;
    bool hasEdgeFrom(const std::string& id) const;
    bool hasEdgeToward(const std::string& id) const;

    // Additional methods (stubs)
    std::shared_ptr<GraphicEdge> getEdgeBetween(std::shared_ptr<GraphicNode> node);
    std::shared_ptr<GraphicEdge> getEdgeBetween(int index);
    std::shared_ptr<GraphicEdge> getEdgeFrom(std::shared_ptr<GraphicNode> node);
    std::shared_ptr<GraphicEdge> getEdgeFrom(int index);
    std::shared_ptr<GraphicEdge> getEdgeToward(std::shared_ptr<GraphicNode> node);
    std::shared_ptr<GraphicEdge> getEdgeToward(int index);
    std::shared_ptr<GraphicEdge> getEnteringEdge(int i);
    std::shared_ptr<GraphicEdge> getLeavingEdge(int i);
};

#endif // GRAPHIC_NODE_HPP
