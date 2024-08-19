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

#ifndef STYLE_GROUP_SET_HPP
#define STYLE_GROUP_SET_HPP

#include "StyleGroup.hpp"
#include "StyleSheet.hpp"
#include "StyleGroupListener.hpp"
#include <map>
#include <set>
#include <vector>
#include <string>
#include <memory>
#include <iterator>

class StyleGroupSet : public StyleSheetListener {
public:
    explicit StyleGroupSet(StyleSheet* stylesheet);
    ~StyleGroupSet();

    int getGroupCount() const;
    StyleGroup* getGroup(const std::string& groupId);
    std::map<std::string, StyleGroup*>::iterator getGroupIterator();
    std::map<std::string, StyleGroup*>::iterator getZIterator();
    std::map<std::string, StyleGroup*>::iterator getShadowIterator();
    bool containsNode(const std::string& id) const;
    bool containsEdge(const std::string& id) const;
    bool containsSprite(const std::string& id) const;
    bool containsGraph(const std::string& id) const;
    Node* getNode(const std::string& id);
    Edge* getEdge(const std::string& id);
    GraphicSprite* getSprite(const std::string& id);
    Graph* getGraph(const std::string& id);
    int getNodeCount() const;
    int getEdgeCount() const;
    int getSpriteCount() const;
    void release();
    void clear();
    void setRemoveEmptyGroups(bool on);
    StyleGroup* addElement(Element* element);
    void removeElement(Element* element);
    void checkElementStyleGroup(Element* element);
    void pushEvent(const std::string& event);
    void pushEventFor(Element* element, const std::string& event);
    void popEvent(const std::string& event);
    void popEventFor(Element* element, const std::string& event);
    void pushElementAsDynamic(Element* element);
    void popElementAsDynamic(Element* element);
    void addListener(StyleGroupListener* listener);
    void removeListener(StyleGroupListener* listener);
    std::string toString() const;

    // StyleSheetListener interface
    void styleAdded(Rule* oldRule, Rule* newRule) override;
    void styleSheetCleared() override;

private:
    StyleGroup* addGroup(const std::string& id, const std::vector<Rule*>& rules, Element* firstElement);
    void removeGroup(StyleGroup* group);
    StyleGroup* addElement_(Element* element);
    void addElementToReverseSearch(Element* element, const std::string& groupId);
    void removeElementFromReverseSearch(Element* element);
    void checkZIndexAndShadow(Rule* oldRule, Rule* newRule);
    void checkForNewStyle(Rule* newRule);
    void checkForNewIdStyle(Rule* newRule, std::map<std::string, std::string>& elt2grp);
    void checkForNewStyle(Rule* newRule, std::map<std::string, std::string>& elt2grp);

    StyleSheet* stylesheet;
    std::map<std::string, StyleGroup*> groups;
    std::map<std::string, std::string> byNodeIdGroups;
    std::map<std::string, std::string> byEdgeIdGroups;
    std::map<std::string, std::string> bySpriteIdGroups;
    std::map<std::string, std::string> byGraphIdGroups;

    class NodeSet;
    class EdgeSet;
    class SpriteSet;
    class GraphSet;
    class EventSet;
    class ZIndex;
    class ShadowSet;

    std::unique_ptr<NodeSet> nodeSet;
    std::unique_ptr<EdgeSet> edgeSet;
    std::unique_ptr<SpriteSet> spriteSet;
    std::unique_ptr<GraphSet> graphSet;
    std::unique_ptr<EventSet> eventSet;
    std::unique_ptr<ZIndex> zIndex;
    std::unique_ptr<ShadowSet> shadow;

    bool removeEmptyGroups = true;
    std::vector<StyleGroupListener*> listeners;
};

#endif // STYLE_GROUP_SET_HPP
