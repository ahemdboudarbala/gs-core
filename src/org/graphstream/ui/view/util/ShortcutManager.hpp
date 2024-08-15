#ifndef SHORTCUT_MANAGER_HPP
#define SHORTCUT_MANAGER_HPP

#include "GraphicGraph.hpp"
#include "View.hpp"

/**
 * Utility to centralize the shortcuts and actions for all view instances.
 */
class ShortcutManager {
public:
    virtual ~ShortcutManager() = default;

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
};

#endif // SHORTCUT_MANAGER_HPP
