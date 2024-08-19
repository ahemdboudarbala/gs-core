#ifndef EDGE_HPP
#define EDGE_HPP

#include "Element.hpp"
#include "Node.hpp"

class Edge : public Element {
public:
    virtual ~Edge() = default;

    /**
     * Is the edge directed?
     *
     * @return True if the edge is directed.
     */
    virtual bool isDirected() const = 0;

    /**
     * Does the source and target of this edge identify the same node?
     *
     * @return True if this edge is a loop.
     */
    virtual bool isLoop() const = 0;

    /**
     * First node of the edge.
     * This is equivalent to the getSourceNode() method, but may be clearer
     * in the source code if the graph you are using is not directed.
     *
     * @return The first node of the edge.
     */
    virtual Node* getNode0() const = 0;

    /**
     * Second node of the edge.
     * This is equivalent to the getTargetNode() method, but may be clearer
     * in the source code if the graph you are using is not directed.
     *
     * @return The second node of the edge.
     */
    virtual Node* getNode1() const = 0;

    /**
     * Start node.
     * When the edge is directed this is the source node, in this case you can get
     * the opposite node using getTargetNode(). This is equivalent to the getNode0()
     * method but may be clearer in the source code if the graph you are using is directed.
     *
     * @return The origin node of the edge.
     */
    virtual Node* getSourceNode() const = 0;

    /**
     * End node.
     * When the edge is directed this is the target node, in this case you can get
     * the opposite node using getSourceNode(). This is equivalent to the getNode1()
     * method but may be clearer in the source code if the graph you are using is directed.
     *
     * @return The destination node of the edge.
     */
    virtual Node* getTargetNode() const = 0;

    /**
     * When knowing one node and one edge of this node, this method returns the node
     * at the other end of the edge.
     * Return null if the given node is not at any end of the edge.
     *
     * @param node The node we search the opposite of.
     * @return the opposite node of the given node.
     */
    virtual Node* getOpposite(Node* node) const = 0;
};

#endif // EDGE_HPP
