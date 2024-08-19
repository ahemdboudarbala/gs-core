#include "StyleGroup.hpp"

// Constructor
StyleGroup::StyleGroup(const std::string& identifier, const std::vector<Rule>& rules, 
                       std::shared_ptr<Element> firstElement, std::shared_ptr<StyleGroupSet::EventSet> eventSet)
    : id(identifier), rules(rules), eventSet(eventSet) {
    elements[firstElement->getId()] = firstElement;
    curEvents = {};
    bulkElements = std::make_shared<BulkElements>();
}

// Access methods implementation
std::string StyleGroup::getId() const {
    return id;
}

Selector::Type StyleGroup::getType() const {
    return rules[0].selector.type;
}

bool StyleGroup::hasDynamicElements() const {
    return !dynamicOnes.empty();
}

bool StyleGroup::hasEventElements() const {
    return !eventsFor.empty();
}

bool StyleGroup::elementHasEvents(const std::shared_ptr<Element>& element) const {
    return eventsFor.find(element) != eventsFor.end();
}

bool StyleGroup::elementIsDynamic(const std::shared_ptr<Element>& element) const {
    return dynamicOnes.find(element) != dynamicOnes.end();
}

std::shared_ptr<Element> StyleGroup::getElement(const std::string& id) const {
    auto it = elements.find(id);
    if (it != elements.end()) {
        return it->second;
    }
    return nullptr;
}

int StyleGroup::getElementCount() const {
    return elements.size();
}

bool StyleGroup::isEmpty() const {
    return elements.empty();
}

bool StyleGroup::contains(const std::string& elementId) const {
    return elements.find(elementId) != elements.end();
}

bool StyleGroup::contains(const std::shared_ptr<Element>& element) const {
    return contains(element->getId());
}

std::shared_ptr<SwingElementRenderer> StyleGroup::getRenderer(const std::string& id) const {
    auto it = renderers.find(id);
    if (it != renderers.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<StyleGroup::ElementEvents> StyleGroup::getEventsFor(const std::shared_ptr<Element>& element) const {
    auto it = eventsFor.find(element);
    if (it != eventsFor.end()) {
        return it->second;
    }
    return nullptr;
}

// Command methods implementation
void StyleGroup::addElement(const std::shared_ptr<Element>& element) {
    elements[element->getId()] = element;
}

std::shared_ptr<Element> StyleGroup::removeElement(const std::shared_ptr<Element>& element) {
    if (elementHasEvents(element)) {
        eventsFor.erase(element);
    }
    dynamicOnes.erase(element);
    return elements.erase(element->getId());
}

void StyleGroup::pushEventFor(const std::shared_ptr<Element>& element, const std::string& event) {
    if (contains(element)) {
        if (eventsFor.find(element) == eventsFor.end()) {
            eventsFor[element] = std::make_shared<ElementEvents>(element, shared_from_this(), event);
        } else {
            eventsFor[element]->pushEvent(event);
        }
    }
}

void StyleGroup::popEventFor(const std::shared_ptr<Element>& element, const std::string& event) {
    if (contains(element)) {
        auto evs = getEventsFor(element);
        if (evs) {
            evs->popEvent(event);
            if (evs->eventCount() == 0) {
                eventsFor.erase(element);
            }
        }
    }
}

void StyleGroup::activateEventsFor(const std::shared_ptr<Element>& element) {
    auto evs = getEventsFor(element);
    if (evs && curEvents.empty()) {
        curEvents = evs->events();
    }
}

void StyleGroup::deactivateEvents() {
    curEvents.clear();
}

void StyleGroup::pushElementAsDynamic(const std::shared_ptr<Element>& element) {
    dynamicOnes.insert(element);
}

void StyleGroup::popElementAsDynamic(const std::shared_ptr<Element>& element) {
    dynamicOnes.erase(element);
}

void StyleGroup::release() {
    elements.clear();
    eventsFor.clear();
    dynamicOnes.clear();
}

void StyleGroup::setValue(const std::string& property, const std::any& value) {
    throw std::runtime_error("You cannot change the values of a style group.");
}

// Renderers management
void StyleGroup::addRenderer(const std::string& id, const std::shared_ptr<SwingElementRenderer>& renderer) {
    renderers[id] = renderer;
}

std::shared_ptr<SwingElementRenderer> StyleGroup::removeRenderer(const std::string& id) {
    auto it = renderers.find(id);
    if (it != renderers.end()) {
        auto renderer = it->second;
        renderers.erase(it);
        return renderer;
    }
    return nullptr;
}

// ToString methods
std::string StyleGroup::toString() const {
    return toString(-1);
}

std::string StyleGroup::toString(int level) const {
    std::string prefix(level * 4, ' ');
    std::string result = prefix + id + "\n";
    result += prefix + "Contains: ";
    for (const auto& element : elements) {
        result += element.second->getId() + " ";
    }
    result += "\n" + prefix + "Style: ";
    for (const auto& rule : rules) {
        result += rule.selector.toString() + " ";
    }
    result += "\n";
    return result;
}

// ElementEvents implementation
StyleGroup::ElementEvents::ElementEvents(const std::shared_ptr<Element>& element, const std::shared_ptr<StyleGroup>& group, const std::string& event)
    : element(element), group(group) {
    events.push_back(event);
}

std::shared_ptr<Element> StyleGroup::ElementEvents::getElement() const {
    return element;
}

int StyleGroup::ElementEvents::eventCount() const {
    return events.size();
}

std::vector<std::string> StyleGroup::ElementEvents::events() const {
    return events;
}

void StyleGroup::ElementEvents::activate() {
    group->activateEventsFor(element);
}

void StyleGroup::ElementEvents::deactivate() {
    group->deactivateEvents();
}

void StyleGroup::ElementEvents::pushEvent(const std::string& event) {
    events.push_back(event);
}

void StyleGroup::ElementEvents::popEvent(const std::string& event) {
    auto it = std::remove(events.begin(), events.end(), event);
    events.erase(it, events.end());
}

// BulkElements implementation
std::shared_ptr<StyleGroup::BulkIterator> StyleGroup::BulkElements::iterator() {
    return std::make_shared<BulkIterator>(elements);
}

// BulkIterator implementation
StyleGroup::BulkIterator::BulkIterator(const std::vector<std::shared_ptr<Element>>& elements)
    : iterator(elements.begin()) {
    nextElement = *iterator;
}

bool StyleGroup::BulkIterator::hasNext() {
    return iterator != elements.end();
}

std::shared_ptr<Element> StyleGroup::BulkIterator::next() {
    auto currentElement = nextElement;
    ++iterator;
    if (iterator != elements.end()) {
        nextElement = *iterator;
    }
    return currentElement;
}
