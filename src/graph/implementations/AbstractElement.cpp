#include "AbstractElement.hpp"

// Constructor implementation
AbstractElement::AbstractElement(const std::string& id)
    : id(id), index(-1) {
    assert(!id.empty() && "Graph elements cannot have a null identifier");
}

// Access methods implementation
std::string AbstractElement::getId() const {
    return id;
}

int AbstractElement::getIndex() const {
    return index;
}

void AbstractElement::setIndex(int newIndex) {
    index = newIndex;
}

// Attribute methods implementation
std::shared_ptr<void> AbstractElement::getAttribute(const std::string& key) const {
    auto it = attributes.find(key);
    if (it != attributes.end()) {
        return it->second;
    }
    return nullptr;
}

void AbstractElement::setAttribute(const std::string& attribute, std::initializer_list<std::shared_ptr<void>> values) {
    std::shared_ptr<void> value;

    if (values.size() == 0) {
        value = std::make_shared<bool>(true);
    } else if (values.size() == 1) {
        value = *values.begin();
    } else {
        value = std::make_shared<std::vector<std::shared_ptr<void>>>(values);
    }

    auto it = attributes.find(attribute);
    AttributeChangeEvent event = it != attributes.end() ? AttributeChangeEvent::CHANGE : AttributeChangeEvent::ADD;

    std::shared_ptr<void> oldValue = it != attributes.end() ? it->second : nullptr;
    attributes[attribute] = value;

    attributeChanged(event, attribute, oldValue, value);
}

void AbstractElement::removeAttribute(const std::string& attribute) {
    if (attributes.find(attribute) != attributes.end() && 
        std::find(attributesBeingRemoved.begin(), attributesBeingRemoved.end(), attribute) == attributesBeingRemoved.end()) {

        attributesBeingRemoved.push_back(attribute);

        attributeChanged(AttributeChangeEvent::REMOVE, attribute, attributes[attribute], nullptr);

        attributesBeingRemoved.pop_back();
        attributes.erase(attribute);
    }
}

bool AbstractElement::hasAttribute(const std::string& key) const {
    return attributes.find(key) != attributes.end();
}

int AbstractElement::getAttributeCount() const {
    return attributes.size();
}

void AbstractElement::clearAttributes() {
    for (auto& entry : attributes) {
        attributeChanged(AttributeChangeEvent::REMOVE, entry.first, entry.second, nullptr);
    }
    attributes.clear();
}

void AbstractElement::clearAttributesWithNoEvent() {
    attributes.clear();
}

// Utility method
std::string AbstractElement::toString() const {
    return id;
}
