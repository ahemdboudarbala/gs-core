#ifndef ABSTRACT_EDGE_HPP
#define ABSTRACT_EDGE_HPP

#include "AbstractElement.hpp"
#include "graph/Edge.hpp"
#include "AbstractNode.hpp"
#include "AbstractGraph.hpp"
//#include "ElementType.hpp"
#include <string>

class AbstractEdge : public AbstractElement, public Edge {
protected:
    // Fields
    AbstractNode* source;
    AbstractNode* target;
    bool directed;
    AbstractGraph* graph;

public:
    // Constructor
    AbstractEdge(const std::string& id, AbstractNode* source, AbstractNode* target, bool directed);

    // Inherited from AbstractElement
    virtual void attributeChanged(const std::string& event, const std::string& attribute, const std::string& oldValue, const std::string& newValue) override;

    virtual std::string toString() const override;

    // Inherited from Edge
    virtual Node* getNode0() const override;
    virtual Node* getNode1() const override;
    virtual Node* getOpposite(Node* node) const override;
    virtual Node* getSourceNode() const override;
    virtual Node* getTargetNode() const override;
    virtual bool isDirected() const override;
    virtual bool isLoop() const override;
};

#endif // ABSTRACT_EDGE_HPP
