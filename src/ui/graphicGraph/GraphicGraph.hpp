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

#ifndef GRAPHIC_GRAPH_HPP
#define GRAPHIC_GRAPH_HPP

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <any>
#include <stdexcept>
#include <atomic>

#include "AbstractElement.hpp"
#include "StyleSheet.hpp"
#include "StyleGroup.hpp"
#include "StyleGroupSet.hpp"
#include "GraphicNode.hpp"
#include "GraphicEdge.hpp"
#include "GraphicSprite.hpp"
#include "Point3.hpp"
#include "GraphListeners.hpp"

class GraphicGraph : public AbstractElement, public StyleGroupListener {
public:
    // Constructor
    GraphicGraph(const std::string& id);

    // Accessors
    bool graphChangedFlag() const;
    void resetGraphChangedFlag();

    std::shared_ptr<StyleSheet> getStyleSheet() const;
    std::shared_ptr<StyleGroup> getStyle() const;
    std::shared_ptr<StyleGroupSet> getStyleGroups() const;

    double getStep() const;
    std::shared_ptr<Point3> getMaxPos() const;
    std::shared_ptr<Point3> getMinPos() const;
    bool feedbackXYZ() const;

    // Commands
    void feedbackXYZ(bool on);
    void computeBounds();
    void moveNode(const std::string& id, double x, double y, double z);

    std::shared_ptr<GraphicNode> getNode(const std::string& id) const;
    std::shared_ptr<GraphicEdge> getEdge(const std::string& id) const;
    std::shared_ptr<GraphicSprite> getSprite(const std::string& id) const;

    void addSink(std::shared_ptr<Sink> listener);
    void removeSink(std::shared_ptr<Sink> listener);

    void addAttributeSink(std::shared_ptr<AttributeSink> listener);
    void removeAttributeSink(std::shared_ptr<AttributeSink> listener);

    void addElementSink(std::shared_ptr<ElementSink> listener);
    void removeElementSink(std::shared_ptr<ElementSink> listener);

    void clearAttributeSinks();
    void clearElementSinks();
    void clearSinks();

    // Sprite management
    std::shared_ptr<GraphicSprite> addSprite(const std::string& id);
    void removeSprite(const std::string& id);

    // Style group listener interface
    void elementStyleChanged(std::shared_ptr<Element> element, std::shared_ptr<StyleGroup> oldStyle, std::shared_ptr<StyleGroup> style) override;
    void styleChanged(std::shared_ptr<StyleGroup> style) override;

    // Graph interface
    int getEdgeCount() const;
    int getNodeCount() const;
    int getSpriteCount() const;

    void replay();

protected:
    virtual void attributeChanged(const AttributeChangeEvent& event, const std::string& attribute, 
                                  const std::any& oldValue, const std::any& newValue) override;

    // Additional sprite management
    void spriteAttribute(const AttributeChangeEvent& event, std::shared_ptr<Element> element, 
                         const std::string& attribute, const std::any& value);
    void addOrChangeSprite(const AttributeChangeEvent& event, std::shared_ptr<Element> element, 
                           const std::string& spriteId, const std::any& value);
    void positionSprite(std::shared_ptr<GraphicSprite> sprite, const std::any& value);

private:
    std::shared_ptr<StyleSheet> styleSheet;
    std::shared_ptr<StyleGroupSet> styleGroups;
    std::map<std::shared_ptr<GraphicNode>, std::vector<std::shared_ptr<GraphicEdge>>> connectivity;
    std::shared_ptr<StyleGroup> style;

    double step;
    bool graphChanged;
    bool boundsChanged;
    std::shared_ptr<Point3> hi;
    std::shared_ptr<Point3> lo;

    std::shared_ptr<GraphListeners> listeners;
    bool feedbackXYZEnabled;

    // Utility methods
    std::shared_ptr<GraphicSprite> addSprite_(const std::string& id);
    std::shared_ptr<GraphicSprite> removeSprite_(const std::string& id);
};

#endif // GRAPHIC_GRAPH_HPP
