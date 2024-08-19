#ifndef MULTINODE_HPP
#define MULTINODE_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "AdjacencyListNode.hpp"
#include "AbstractEdge.hpp"
#include "AbstractNode.hpp"

class MultiNode : public AdjacencyListNode {
public:
    MultiNode(std::shared_ptr<AbstractGraph> graph, const std::string& id);

    template<typename T = AbstractEdge>
    std::vector<std::shared_ptr<T>> getEdgeSetBetween(const std::shared_ptr<AbstractNode>& node) const;

    template<typename T = AbstractEdge>
    std::vector<std::shared_ptr<T>> getEdgeSetBetween(const std::string& id) const;

    template<typename T = AbstractEdge>
    std::vector<std::shared_ptr<T>> getEdgeSetBetween(int index) const;

protected:
    virtual bool addEdgeCallback(std::shared_ptr<AbstractEdge> edge) override;
    virtual void removeEdge(int i) override;
    virtual void clearCallback() override;

    std::unordered_map<std::shared_ptr<AbstractNode>, std::vector<std::shared_ptr<AbstractEdge>>> neighborMap;
};

#endif // MULTINODE_HPP
