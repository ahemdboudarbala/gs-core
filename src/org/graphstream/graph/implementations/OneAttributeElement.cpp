#include "OneAttributeElement.hpp"

// Constructor
OneAttributeElement::OneAttributeElement(const std::string& id) : id(id), attribute(nullptr) {
    assert(!id.empty() && "Graph elements cannot have a null identifier");
}

// Accessors
std::string OneAttributeElement::getId() const {
    return id;
}

std::shared_ptr<void> OneAttributeElement::getAttribute(const std::string& key) const {
    return attribute;
}

template<typename T>
std::shared_ptr<T> OneAttributeElement::getAttribute(const std::string& key) const {
    return std::static_pointer_cast<T>(attribute);
}

template<typename T>
std::shared_ptr<T> OneAttributeElement::getFirstAttributeOf(const std::vector<std::string>& keys) const {
    return std::static_pointer_cast<T>(attribute);
}

std::shared_ptr<std::string> OneAttributeElement::getLabel(const std::string& key) const {
    return std::dynamic_pointer_cast<std::string>(attribute);
}

double OneAttributeElement::getNumber(const std::string& key) const {
    if (attribute) {
        if (auto numPtr = std::dynamic_pointer_cast<double>(attribute)) {
            return *numPtr;
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}

std::shared_ptr<std::vector<double>> OneAttributeElement::getVector(const std::string& key) const {
    return std::dynamic_pointer_cast<std::vector<double>>(attribute);
}

bool OneAttributeElement::hasAttribute(const std::string& key) const {
    return attribute != nullptr;
}

bool OneAttributeElement::hasAttribute(const std::string& key, const std::type_info& type) const {
    return attribute && typeid(*attribute) == type;
}

bool OneAttributeElement::hasLabel(const std::string& key) const {
    return attribute && std::dynamic_pointer_cast<std::string>(attribute) != nullptr;
}

bool OneAttributeElement::hasNumber(const std::string& key) const {
    return attribute && std::dynamic_pointer_cast<double>(attribute) != nullptr;
}

bool OneAttributeElement::hasVector(const std::string& key) const {
    return attribute && std::dynamic_pointer_cast<std::vector<double>>(attribute) != nullptr;
}

// Commands
void OneAttributeElement::clearAttributes() {
    attribute = nullptr;
}

void OneAttributeElement::addAttribute(const std::string& attribute, const std::shared_ptr<void>& value) {
    this->attribute = value;
    attributeChanged("", 0, attribute, AttributeChangeEvent::ADD, nullptr, value);
}

void OneAttributeElement::changeAttribute(const std::string& attribute, const std::shared_ptr<void>& value) {
    auto oldValue = this->attribute;
    this->attribute = value;
    attributeChanged("", 0, attribute, AttributeChangeEvent::CHANGE, oldValue, value);
}

void OneAttributeElement::setAttributes(const std::map<std::string, std::shared_ptr<void>>& attributes) {
    if (!attributes.empty()) {
        addAttribute("", attributes.begin()->second);
    }
}

void OneAttributeElement::removeAttribute(const std::string& attribute) {
    auto oldValue = this->attribute;
    this->attribute = nullptr;
    attributeChanged("", 0, attribute, AttributeChangeEvent::REMOVE, oldValue, nullptr);
}

// Convert element to string
std::string OneAttributeElement::toString() const {
    return id;
}
