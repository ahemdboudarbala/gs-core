#ifndef MOUSE_MANAGER_HPP
#define MOUSE_MANAGER_HPP

#include "GraphicGraph.hpp"
#include "View.hpp"
#include "InteractiveElement.hpp"
#include <set>

/**
 * A global behavior for all mouse events on graphic elements.
 */
class MouseManager {
public:
    virtual ~MouseManager() = default;

    /**
     * Make the manager active on the given graph and view.
     * 
     * @param graph
     *            The graph to control.
     * @param view
     *            The view to control.
     */
    virtual void init(GraphicGraph* graph, View* view) = 0;

    /**
     * Release the links between this manager and the view and the graph.
     */
    virtual void release() = 0;

    /**
     * Returns the set of InteractiveElements managed by the MouseManager.
     *
     * @return the set of InteractiveElements managed by the MouseManager.
     */
    virtual std::set<InteractiveElement> getManagedTypes() const = 0;
};

#endif // MOUSE_MANAGER_HPP
