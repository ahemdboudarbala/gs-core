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

#ifndef GRAPHIC_ELEMENT_HPP
#define GRAPHIC_ELEMENT_HPP

#include <string>
#include <memory>
#include <regex>
#include "AbstractElement.hpp"
#include "StyleGroup.hpp"
#include "GraphicGraph.hpp"

class GraphicElement : public AbstractElement {
public:
    class SwingElementRenderer {};

    GraphicElement(const std::string& id, std::shared_ptr<GraphicGraph> graph);

    virtual ~GraphicElement() = default;

    std::shared_ptr<GraphicGraph> getGraph() const;

    virtual Selector::Type getSelectorType() const = 0;

    std::shared_ptr<StyleGroup> getStyle() const;

    std::string getLabel() const;

    virtual double getX() const = 0;
    virtual double getY() const = 0;
    virtual double getZ() const = 0;

    void* getComponent() const;
    void setComponent(void* component);

    virtual void removed() = 0;
    virtual void move(double x, double y, double z) = 0;

    void attributeChanged(AttributeChangeEvent event, const std::string& attribute, 
                          const std::any& oldValue, const std::any& newValue) override;

    void setAttribute(const std::string& attribute, const std::any& value) override;

protected:
    std::shared_ptr<GraphicGraph> myGraph;
    std::string label;
    std::shared_ptr<StyleGroup> style;
    void* component = nullptr;
    bool hidden = false;

private:
    static std::regex acceptedAttribute;
};

#endif // GRAPHIC_ELEMENT_HPP
