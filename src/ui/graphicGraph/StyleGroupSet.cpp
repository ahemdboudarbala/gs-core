#include "StyleGroupSet.hpp"
#include "StyleGroup.hpp"
#include "Element.hpp"
#include "Node.hpp"
#include "Edge.hpp"
#include "GraphicSprite.hpp"
#include "Graph.hpp"
#include <algorithm>
#include <iterator>

StyleGroupSet::StyleGroupSet(StyleSheet* stylesheet) 
    : stylesheet(stylesheet),
      nodeSet(std::make_unique<NodeSet>()),
      edgeSet(std::make_unique<EdgeSet>()),
      spriteSet(std::make_unique<SpriteSet>()),
      graphSet(std::make_unique<GraphSet>()),
      eventSet(std::make_unique<EventSet>()),
      zIndex(std::make_unique<ZIndex>()),
      shadow(std::make_unique<ShadowSet>()) {
    stylesheet->addListener(this);
}

StyleGroupSet::~StyleGroupSet() {
    release();
}

int StyleGroupSet::getGroupCount() const {
    return groups.size();
}

StyleGroup* StyleGroupSet::getGroup(const std::string& groupId) {
    auto it = groups.find(groupId);
    if (it != groups.end()) {
        return it->second;
    }
    return nullptr;
}

std::map<std::string, StyleGroup*>::iterator StyleGroupSet::getGroupIterator() {
    return groups.begin();
}

std::map<std::string, StyleGroup*>::iterator StyleGroupSet::getZIterator() {
    return zIndex->getIterator();
}

std::map<std::string, StyleGroup*>::iterator StyleGroupSet::getShadowIterator() {
    return shadow->getIterator();
}

bool StyleGroupSet::containsNode(const std::string& id) const {
    return byNodeIdGroups.find(id) != byNodeIdGroups.end();
}

bool StyleGroupSet::containsEdge(const std::string& id) const {
    return byEdgeIdGroups.find(id) != byEdgeIdGroups.end();
}

bool StyleGroupSet::containsSprite(const std::string& id) const {
    return bySpriteIdGroups.find(id) != bySpriteIdGroups.end();
}

bool StyleGroupSet::containsGraph(const std::string& id) const {
    return byGraphIdGroups.find(id) != byGraphIdGroups.end();
}

Node* StyleGroupSet::getNode(const std::string& id) {
    return dynamic_cast<Node*>(getElement(id, byNodeIdGroups));
}

Edge* StyleGroupSet::getEdge(const std::string& id) {
    return dynamic_cast<Edge*>(getElement(id, byEdgeIdGroups));
}

GraphicSprite* StyleGroupSet::getSprite(const std::string& id) {
    return dynamic_cast<GraphicSprite*>(getElement(id, bySpriteIdGroups));
}

Graph* StyleGroupSet::getGraph(const std::string& id) {
    return dynamic_cast<Graph*>(getElement(id, byGraphIdGroups));
}

int StyleGroupSet::getNodeCount() const {
    return byNodeIdGroups.size();
}

int StyleGroupSet::getEdgeCount() const {
    return byEdgeIdGroups.size();
}

int StyleGroupSet::getSpriteCount() const {
    return bySpriteIdGroups.size();
}

void StyleGroupSet::release() {
    stylesheet->removeListener(this);
}

void StyleGroupSet::clear() {
    byEdgeIdGroups.clear();
    byNodeIdGroups.clear();
    bySpriteIdGroups.clear();
    byGraphIdGroups.clear();
    groups.clear();
    zIndex->clear();
    shadow->clear();
}

void StyleGroupSet::setRemoveEmptyGroups(bool on) {
    if (!removeEmptyGroups && on) {
        auto it = groups.begin();
        while (it != groups.end()) {
            if (it->second->isEmpty()) {
                it = groups.erase(it);
            } else {
                ++it;
            }
        }
    }
    removeEmptyGroups = on;
}

StyleGroup* StyleGroupSet::addElement(Element* element) {
    StyleGroup* group = addElement_(element);
    for (auto listener : listeners) {
        listener->elementStyleChanged(element, nullptr, group);
    }
    return group;
}

StyleGroup* StyleGroupSet::addElement_(Element* element) {
    auto rules = stylesheet->getRulesFor(element);
    auto gid = stylesheet->getStyleGroupIdFor(element, rules);
    StyleGroup* group = getGroup(gid);
    if (!group) {
        group = addGroup(gid, rules, element);
    } else {
        group->addElement(element);
    }
    addElementToReverseSearch(element, gid);
    return group;
}

void StyleGroupSet::removeElement(Element* element) {
    std::string gid = getElementGroup(element);
    StyleGroup* group = getGroup(gid);
    if (group) {
        group->removeElement(element);
        removeElementFromReverseSearch(element);
        if (removeEmptyGroups && group->isEmpty()) {
            removeGroup(group);
        }
    }
}

void StyleGroupSet::checkElementStyleGroup(Element* element) {
    StyleGroup* oldGroup = getGroup(getElementGroup(element));
    bool isDyn = false;
    StyleGroup::ElementEvents* events = nullptr;

    if (oldGroup) {
        isDyn = oldGroup->isElementDynamic(element);
        events = oldGroup->getEventsFor(element);
    }

    removeElement(element);
    addElement_(element);

    StyleGroup* newGroup = getGroup(getElementGroup(element));

    if (newGroup && events) {
        for (const auto& event : events->events) {
            pushEventFor(element, event);
        }
    }

    for (auto listener : listeners) {
        listener->elementStyleChanged(element, oldGroup, newGroup);
    }

    if (newGroup && isDyn) {
        newGroup->pushElementAsDynamic(element);
    }
}

void StyleGroupSet::pushEvent(const std::string& event) {
    eventSet->pushEvent(event);
}

void StyleGroupSet::pushEventFor(Element* element, const std::string& event) {
    StyleGroup* group = getGroup(getElementGroup(element));
    if (group) {
        group->pushEventFor(element, event);
    }
}

void StyleGroupSet::popEvent(const std::string& event) {
    eventSet->popEvent(event);
}

void StyleGroupSet::popEventFor(Element* element, const std::string& event) {
    StyleGroup* group = getGroup(getElementGroup(element));
    if (group) {
        group->popEventFor(element, event);
    }
}

void StyleGroupSet::pushElementAsDynamic(Element* element) {
    StyleGroup* group = getGroup(getElementGroup(element));
    if (group) {
        group->pushElementAsDynamic(element);
    }
}

void StyleGroupSet::popElementAsDynamic(Element* element) {
    StyleGroup* group = getGroup(getElementGroup(element));
    if (group) {
        group->popElementAsDynamic(element);
    }
}

void StyleGroupSet::addListener(StyleGroupListener* listener) {
    listeners.push_back(listener);
}

void StyleGroupSet::removeListener(StyleGroupListener* listener) {
    auto it = std::find(listeners.begin(), listeners.end(), listener);
    if (it != listeners.end()) {
        listeners.erase(it);
    }
}

std::string StyleGroupSet::toString() const {
    std::string result = "Style groups (" + std::to_string(groups.size()) + "):\n";
    for (const auto& group : groups) {
        result += group.second->toString(1) + "\n";
    }
    return result;
}

void StyleGroupSet::styleAdded(Rule* oldRule, Rule* newRule) {
    if (!oldRule) {
        checkForNewStyle(newRule);
    } else {
        checkZIndexAndShadow(oldRule, newRule);
    }
}

void StyleGroupSet::styleSheetCleared() {
    std::vector<Element*> elements;

    for (auto& graph : *graphSet) {
        elements.push_back(graph);
    }

    for (auto node : nodeSet->iterator()) {
        elements.push_back(node);
    }

    for (auto edge : edgeSet->iterator()) {
        elements.push_back(edge);
    }

    for (auto sprite : spriteSet->iterator()) {
        elements.push_back(sprite);
    }

    clear();

    for (auto element : elements) {
        removeElement(element);
        addElement(element);
    }
}

StyleGroup* StyleGroupSet::addGroup(const std::string& id, const std::vector<Rule*>& rules, Element* firstElement) {
    auto group = new StyleGroup(id, rules, firstElement, eventSet.get());
    groups[id] = group;
    zIndex->groupAdded(group);
    shadow->groupAdded(group);
    return group;
}

void StyleGroupSet::removeGroup(StyleGroup* group) {
    zIndex->groupRemoved(group);
    shadow->groupRemoved(group);
    groups.erase(group->getId());
    group->release();
}

void StyleGroupSet::addElementToReverseSearch(Element* element, const std::string& groupId) {
    if (dynamic_cast<Node*>(element)) {
        byNodeIdGroups[element->getId()] = groupId;
    } else if (dynamic_cast<Edge*>(element)) {
        byEdgeIdGroups[element->getId()] = groupId;
    } else if (dynamic_cast<GraphicSprite*>(element)) {
        bySpriteIdGroups[element->getId()] = groupId;
    } else if (dynamic_cast<Graph*>(element)) {
        byGraphIdGroups[element->getId()] = groupId;
    }
}

void StyleGroupSet::removeElementFromReverseSearch(Element* element) {
    if (dynamic_cast<Node*>(element)) {
        byNodeIdGroups.erase(element->getId());
    } else if (dynamic_cast<Edge*>(element)) {
        byEdgeIdGroups.erase(element->getId());
    } else if (dynamic_cast<GraphicSprite*>(element)) {
        bySpriteIdGroups.erase(element->getId());
    } else if (dynamic_cast<Graph*>(element)) {
        byGraphIdGroups.erase(element->getId());
    }
}

void StyleGroupSet::checkZIndexAndShadow(Rule* oldRule, Rule* newRule) {
    if (oldRule) {
        if (oldRule->selector.getId() || oldRule->selector.getClazz()) {
            if (oldRule->getGroups()) {
                for (const auto& s : oldRule->getGroups()) {
                    StyleGroup* group = groups[s];
                    if (group) {
                        zIndex->groupChanged(group);
                        shadow->groupChanged(group);
                    }
                }
            }
        } else {
            Selector::Type type = oldRule->selector.type;

            for (const auto& group : groups) {
                if (group.second->getType() == type) {
                    zIndex->groupChanged(group.second);
                    shadow->groupChanged(group.second);
                }
            }
        }
    }
}

void StyleGroupSet::checkForNewStyle(Rule* newRule) {
    switch (newRule->selector.type) {
        case Selector::Type::GRAPH:
            if (newRule->selector.getId()) {
                checkForNewIdStyle(newRule, byGraphIdGroups);
            } else {
                checkForNewStyle(newRule, byGraphIdGroups);
            }
            break;
        case Selector::Type::NODE:
            if (newRule->selector.getId()) {
                checkForNewIdStyle(newRule, byNodeIdGroups);
            } else {
                checkForNewStyle(newRule, byNodeIdGroups);
            }
            break;
        case Selector::Type::EDGE:
            if (newRule->selector.getId()) {
                checkForNewIdStyle(newRule, byEdgeIdGroups);
            } else {
                checkForNewStyle(newRule, byEdgeIdGroups);
            }
            break;
        case Selector::Type::SPRITE:
            if (newRule->selector.getId()) {
                checkForNewIdStyle(newRule, bySpriteIdGroups);
            } else {
                checkForNewStyle(newRule, bySpriteIdGroups);
            }
            break;
        default:
            throw std::runtime_error("Unexpected type");
    }
}

void StyleGroupSet::checkForNewIdStyle(Rule* newRule, std::map<std::string, std::string>& elt2grp) {
    Element* element = getElement(newRule->selector.getId(), elt2grp);
    if (element) {
        checkElementStyleGroup(element);
    }
}

void StyleGroupSet::checkForNewStyle(Rule* newRule, std::map<std::string, std::string>& elt2grp) {
    for (const auto& entry : elt2grp) {
        Element* element = getElement(entry.first, elt2grp);
        checkElementStyleGroup(element);
    }
}
