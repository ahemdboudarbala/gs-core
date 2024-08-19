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

#ifndef GRAPHIC_SPRITE_HPP
#define GRAPHIC_SPRITE_HPP

#include <string>
#include <memory>
#include "GraphicElement.hpp"
#include "GraphicNode.hpp"
#include "GraphicEdge.hpp"
#include "Values.hpp"

class GraphicSprite : public GraphicElement {
public:
    // Constructors
    GraphicSprite(const std::string& id, std::shared_ptr<GraphicGraph> graph);

    // Access methods
    std::shared_ptr<GraphicNode> getNodeAttachment() const;
    std::shared_ptr<GraphicEdge> getEdgeAttachment() const;
    std::shared_ptr<GraphicElement> getAttachment() const;
    bool isAttached() const;
    bool isAttachedToNode() const;
    bool isAttachedToEdge() const;
    virtual Selector::Type getSelectorType() const override;
    virtual double getX() const override;
    virtual double getY() const override;
    virtual double getZ() const override;
    Style::Units getUnits() const;

    // Command methods
    virtual void move(double x, double y, double z) override;
    void attachToNode(std::shared_ptr<GraphicNode> node);
    void attachToEdge(std::shared_ptr<GraphicEdge> edge);
    void detach();
    void setPosition(double value);
    void setPosition(double x, double y, double z, Style::Units units = Style::Units::GU);
    void setPosition(const Values& values);

protected:
    std::shared_ptr<GraphicNode> node;
    std::shared_ptr<GraphicEdge> edge;
    Values position;

    double checkAngle(double angle);

    virtual void attributeChanged(const AttributeChangeEvent& event, const std::string& attribute, 
                                  const std::any& oldValue, const std::any& newValue) override;
    virtual void removed() override;
};

#endif // GRAPHIC_SPRITE_HPP
