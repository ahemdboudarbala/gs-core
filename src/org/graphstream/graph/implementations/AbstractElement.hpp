#ifndef ABSTRACT_ELEMENT_HPP
#define ABSTRACT_ELEMENT_HPP

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <typeinfo>
#include <iostream>

class Element {
public:
    virtual ~Element() = default;

    virtual std::string getId() const = 0;
    virtual int getIndex() const = 0;
    virtual void setAttribute(const std::string& attribute, std::initializer_list<std::shared_ptr<void>> values) = 0;
    virtual void removeAttribute(const std::string& attribute) = 0;
    virtual bool hasAttribute(const std::string& key) const = 0;
    virtual int getAttributeCount() const = 0;
    virtual std::string toString() const = 0;

    // Other virtual methods from Element interface would go here...
};

class AbstractElement : public Element {
public:
    enum class AttributeChangeEvent {
        ADD,
        CHANGE,
        REMOVE
    };

protected:
    // Fields
    std::string id;
    int index;
    std::map<std::string, std::shared_ptr<void>> attributes;
    std::vector<std::string> attributesBeingRemoved;

public:
    // Constructor
    explicit AbstractElement(const std::string& id);

    // Access methods
    std::string getId() const override;
    int getIndex() const override;
    void setIndex(int newIndex);

    // Attribute methods
    std::shared_ptr<void> getAttribute(const std::string& key) const;
    void setAttribute(const std::string& attribute, std::initializer_list<std::shared_ptr<void>> values) override;
    void removeAttribute(const std::string& attribute) override;
    bool hasAttribute(const std::string& key) const override;
    int getAttributeCount() const override;
    void clearAttributes();
    void clearAttributesWithNoEvent();

    // Abstract method to handle attribute changes
    virtual void attributeChanged(AttributeChangeEvent event, const std::string& attribute,
                                  std::shared_ptr<void> oldValue, std::shared_ptr<void> newValue) = 0;

    // Utility method
    std::string toString() const override;
};

#endif // ABSTRACT_ELEMENT_HPP
