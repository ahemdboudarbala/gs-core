#ifndef ATTRIBUTE_PIPE_HPP
#define ATTRIBUTE_PIPE_HPP

#include "PipeBase.hpp"
#include "AttributePredicate.hpp"

/**
 * Allows filtering of the attribute event stream.
 * 
 * Filtering is based on attribute predicates. An attribute predicate is an object
 * that defines the method matches(). If the matches() method returns false, the
 * attribute is discarded from the event stream; otherwise, it is passed to the
 * listeners of this filter.
 */
class AttributePipe : public PipeBase {
public:
    AttributePipe();

    void setGlobalAttributeFilter(std::shared_ptr<AttributePredicate> filter);
    void setGraphAttributeFilter(std::shared_ptr<AttributePredicate> filter);
    void setNodeAttributeFilter(std::shared_ptr<AttributePredicate> filter);
    void setEdgeAttributeFilter(std::shared_ptr<AttributePredicate> filter);

    std::shared_ptr<AttributePredicate> getGlobalAttributeFilter() const;
    std::shared_ptr<AttributePredicate> getGraphAttributeFilter() const;
    std::shared_ptr<AttributePredicate> getNodeAttributeFilter() const;
    std::shared_ptr<AttributePredicate> getEdgeAttributeFilter() const;

    // GraphListener overrides
    void edgeAttributeAdded(const std::string& graphId, long timeId, const std::string& edgeId,
                            const std::string& attribute, const std::any& value) override;

    void edgeAttributeChanged(const std::string& graphId, long timeId, const std::string& edgeId,
                              const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;

    void edgeAttributeRemoved(const std::string& graphId, long timeId, const std::string& edgeId,
                              const std::string& attribute) override;

    void graphAttributeAdded(const std::string& graphId, long timeId, const std::string& attribute,
                             const std::any& value) override;

    void graphAttributeChanged(const std::string& graphId, long timeId, const std::string& attribute,
                               const std::any& oldValue, const std::any& newValue) override;

    void graphAttributeRemoved(const std::string& graphId, long timeId, const std::string& attribute) override;

    void nodeAttributeAdded(const std::string& graphId, long timeId, const std::string& nodeId,
                            const std::string& attribute, const std::any& value) override;

    void nodeAttributeChanged(const std::string& graphId, long timeId, const std::string& nodeId,
                              const std::string& attribute, const std::any& oldValue, const std::any& newValue) override;

    void nodeAttributeRemoved(const std::string& graphId, long timeId, const std::string& nodeId,
                              const std::string& attribute) override;

private:
    std::shared_ptr<AttributePredicate> globalPredicate;
    std::shared_ptr<AttributePredicate> graphPredicate;
    std::shared_ptr<AttributePredicate> nodePredicate;
    std::shared_ptr<AttributePredicate> edgePredicate;

    class FalsePredicate : public AttributePredicate {
    public:
        bool matches(const std::string& attributeName, const std::any& attributeValue) const override {
            return false;
        }
    };
};

#endif // ATTRIBUTE_PIPE_HPP
