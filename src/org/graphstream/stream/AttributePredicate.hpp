#ifndef ATTRIBUTE_PREDICATE_HPP
#define ATTRIBUTE_PREDICATE_HPP

#include <string>
#include <any>

/**
 * Interface to determine if an attribute is recognized.
 */
class AttributePredicate {
public:
    virtual ~AttributePredicate() = default;

    /**
     * Determine if an attribute is recognized or not. The predicate can work on the
     * name of the attribute, on its value, or on both.
     *
     * @param attributeName The name of the attribute.
     * @param attributeValue The value of the attribute.
     * @return True if the attribute must be removed from the stream of graph events.
     */
    virtual bool matches(const std::string& attributeName, const std::any& attributeValue) const = 0;
};

#endif // ATTRIBUTE_PREDICATE_HPP
