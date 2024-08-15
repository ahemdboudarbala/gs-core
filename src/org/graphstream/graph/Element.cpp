#include "Element.hpp"
#include <unordered_map>
#include <variant>

class BasicElement : public Element {
private:
    std::string id;
    int index;
    std::unordered_map<std::string, std::shared_ptr<void>> attributes;

public:
    BasicElement(const std::string& id, int index)
        : id(id), index(index) {}

    std::string getId() const override {
        return id;
    }

    int getIndex() const override {
        return index;
    }

    std::optional<std::shared_ptr<void>> getAttribute(const std::string& key) const override {
        auto it = attributes.find(key);
        if (it != attributes.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    bool hasAttribute(const std::string& key) const override {
        return attributes.find(key) != attributes.end();
    }

    std::vector<std::string> attributeKeys() const override {
        std::vector<std::string> keys;
        for (const auto& pair : attributes) {
            keys.push_back(pair.first);
        }
        return keys;
    }

    void clearAttributes() override {
        attributes.clear();
    }

    void setAttribute(const std::string& attribute, std::shared_ptr<void> value) override {
        attributes[attribute] = value;
    }

    void removeAttribute(const std::string& attribute) override {
        attributes.erase(attribute);
    }

    int getAttributeCount() const override {
        return attributes.size();
    }
};
