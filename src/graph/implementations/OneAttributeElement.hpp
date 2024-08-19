#ifndef ONEATTRIBUTELEMENT_HPP
#define ONEATTRIBUTELEMENT_HPP

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <iterator>
#include <typeinfo>
#include <cassert>
#include <iostream>

class OneAttributeElement {
public:
    enum class AttributeChangeEvent {
        ADD,
        CHANGE,
        REMOVE
    };

    explicit OneAttributeElement(const std::string& id);

    // Accessors
    std::string getId() const;
    std::shared_ptr<void> getAttribute(const std::string& key) const;
    
    template<typename T>
    std::shared_ptr<T> getAttribute(const std::string& key) const;

    template<typename T>
    std::shared_ptr<T> getFirstAttributeOf(const std::vector<std::string>& keys) const;

    std::shared_ptr<std::string> getLabel(const std::string& key) const;
    double getNumber(const std::string& key) const;
    std::shared_ptr<std::vector<double>> getVector(const std::string& key) const;

    bool hasAttribute(const std::string& key) const;
    bool hasAttribute(const std::string& key, const std::type_info& type) const;
    bool hasLabel(const std::string& key) const;
    bool hasNumber(const std::string& key) const;
    bool hasVector(const std::string& key) const;

    virtual void clearAttributes();
    virtual void addAttribute(const std::string& attribute, const std::shared_ptr<void>& value);
    virtual void changeAttribute(const std::string& attribute, const std::shared_ptr<void>& value);
    virtual void setAttributes(const std::map<std::string, std::shared_ptr<void>>& attributes);
    virtual void removeAttribute(const std::string& attribute);

    virtual std::string toString() const;

protected:
    std::string id;
    std::shared_ptr<void> attribute;

    virtual void attributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, AttributeChangeEvent event, const std::shared_ptr<void>& oldValue, const std::shared_ptr<void>& newValue) = 0;
};

#endif // ONEATTRIBUTELEMENT_HPP
