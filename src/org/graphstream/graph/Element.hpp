#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <optional>
#include <type_traits>
#include <stdexcept>

class Element {
public:
    virtual ~Element() = default;

    // Unique identifier of this element.
    virtual std::string getId() const = 0;

    // The current index of this element.
    virtual int getIndex() const = 0;

    // Get the attribute object bound to the given key.
    virtual std::optional<std::shared_ptr<void>> getAttribute(const std::string& key) const = 0;

    // Get the first existing attribute in a list of keys.
    template<typename T>
    std::optional<T> getFirstAttributeOf(const std::vector<std::string>& keys) const {
        for (const auto& key : keys) {
            auto attr = getAttribute<T>(key);
            if (attr) {
                return attr;
            }
        }
        return std::nullopt;
    }

    // Get the attribute object bound to the given key if it is an instance of the given class.
    template<typename T>
    std::optional<T> getAttribute(const std::string& key) const {
        auto attr = getAttribute(key);
        if (attr) {
            return std::static_pointer_cast<T>(attr.value());
        }
        return std::nullopt;
    }

    // Get the label string bound to the given key key.
    virtual std::optional<std::string> getLabel(const std::string& key) const {
        auto attr = getAttribute<std::string>(key);
        return attr ? attr : std::nullopt;
    }

    // Get the number bound to key.
    virtual double getNumber(const std::string& key) const {
        auto attr = getAttribute(key);
        if (attr) {
            if (auto num = std::static_pointer_cast<double>(attr.value())) {
                return *num;
            }
            if (auto str = std::static_pointer_cast<std::string>(attr.value())) {
                try {
                    return std::stod(*str);
                } catch (const std::invalid_argument&) {}
            }
        }
        return std::numeric_limits<double>::quiet_NaN();
    }

    // Get the vector of numbers bound to key.
    virtual std::optional<std::vector<double>> getVector(const std::string& key) const {
        auto attr = getAttribute(key);
        if (attr) {
            if (auto vec = std::static_pointer_cast<std::vector<double>>(attr.value())) {
                return *vec;
            }
        }
        return std::nullopt;
    }

    // Get the array of objects bound to key.
    virtual std::optional<std::vector<std::shared_ptr<void>>> getArray(const std::string& key) const {
        auto attr = getAttribute(key);
        if (attr) {
            if (auto arr = std::static_pointer_cast<std::vector<std::shared_ptr<void>>>(attr.value())) {
                return *arr;
            }
        }
        return std::nullopt;
    }

    // Get the map bound to key.
    virtual std::optional<std::map<std::string, std::shared_ptr<void>>> getMap(const std::string& key) const {
        auto attr = getAttribute(key);
        if (attr) {
            if (auto map = std::static_pointer_cast<std::map<std::string, std::shared_ptr<void>>>(attr.value())) {
                return *map;
            }
        }
        return std::nullopt;
    }

    // Does this element store a value for the given attribute key?
    virtual bool hasAttribute(const std::string& key) const = 0;

    // Does this element store a value for the given attribute key and this value is an instance of the given class?
    template<typename T>
    bool hasAttribute(const std::string& key) const {
        return getAttribute<T>(key).has_value();
    }

    // Does this element store a label value for the given key?
    virtual bool hasLabel(const std::string& key) const {
        return hasAttribute<std::string>(key);
    }

    // Does this element store a number for the given key?
    virtual bool hasNumber(const std::string& key) const {
        return hasAttribute<double>(key) || (getLabel(key).has_value() && !std::isnan(getNumber(key)));
    }

    // Does this element store a vector value for the given key?
    virtual bool hasVector(const std::string& key) const {
        return hasAttribute<std::vector<double>>(key);
    }

    // Does this element store an array value for the given key?
    virtual bool hasArray(const std::string& key) const {
        return hasAttribute<std::vector<std::shared_ptr<void>>>(key);
    }

    // Does this element store a map value for the given key?
    virtual bool hasMap(const std::string& key) const {
        return hasAttribute<std::map<std::string, std::shared_ptr<void>>>(key);
    }

    // Stream over the attribute keys of the element.
    virtual std::vector<std::string> attributeKeys() const = 0;

    // Remove all registered attributes.
    virtual void clearAttributes() = 0;

    // Add or replace the value of an attribute.
    virtual void setAttribute(const std::string& attribute, std::shared_ptr<void> value) = 0;

    // Add or replace each attribute found in attributes.
    virtual void setAttributes(const std::map<std::string, std::shared_ptr<void>>& attributes) {
        for (const auto& [key, value] : attributes) {
            setAttribute(key, value);
        }
    }

    // Remove an attribute.
    virtual void removeAttribute(const std::string& attribute) = 0;

    // Number of attributes stored in this element.
    virtual int getAttributeCount() const = 0;
};

#endif // ELEMENT_HPP
