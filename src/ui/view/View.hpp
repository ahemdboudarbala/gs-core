#ifndef VIEW_HPP
#define VIEW_HPP

#include <string>
#include <memory>
#include <set>
#include <map>
#include "ui/graphicGraph/GraphicElement.hpp"
#include "ui/graphicGraph/GraphicGraph.hpp"
#include "ui/view/camera/Camera.hpp"
#include "ui/view/util/InteractiveElement.hpp"
#include "ui/view/util/MouseManager.hpp"
#include "ui/view/util/ShortcutManager.hpp"

class View {
public:
    virtual ~View() = default;

    /**
     * Get the unique view id.
     *
     * @return a view id
     */
    virtual std::string getIdView() const = 0;

    /**
     * Get a camera object to provide control commands on the view.
     *
     * @return a Camera instance
     */
    virtual std::shared_ptr<Camera> getCamera() const = 0;

    /**
     * Search for the first GraphicElement among the specified types (precedence: node, edge, sprite) that contains the point at coordinates (x, y).
     *
     * @param types The types to check
     * @param x The point abscissa.
     * @param y The point ordinate.
     * @return The first GraphicElement among the specified types at the given coordinates or null if nothing found.
     */
    virtual std::shared_ptr<GraphicElement> findGraphicElementAt(const std::set<InteractiveElement>& types, double x, double y) const = 0;

    /**
     * Search for all the graphic elements contained inside the rectangle (x1,y1)-(x2,y2).
     *
     * @param types The set of types to check
     * @param x1 The rectangle lowest point abscissa.
     * @param y1 The rectangle lowest point ordinate.
     * @param x2 The rectangle highest point abscissa.
     * @param y2 The rectangle highest point ordinate.
     * @return The set of sprites, nodes, and edges in the given rectangle.
     */
    virtual std::set<std::shared_ptr<GraphicElement>> allGraphicElementsIn(const std::set<InteractiveElement>& types, double x1, double y1, double x2, double y2) const = 0;

    /**
     * Redisplay or update the view contents. Called by the Viewer.
     *
     * @param graph The graphic graph to represent.
     * @param graphChanged True if the graph changed since the last call to this method.
     */
    virtual void display(const std::shared_ptr<GraphicGraph>& graph, bool graphChanged) = 0;

    /**
     * Open this view in a frame. The argument allows to put the view in a new frame or to remove it from the frame (if it already exists).
     * Called by the Viewer.
     *
     * @param on Add the view in its own frame or remove it if it already was in its own frame.
     */
    virtual void openInAFrame(bool on) = 0;

    /**
     * Close definitively this view. Called by the Viewer.
     *
     * @param graph The graphic graph.
     */
    virtual void close(const std::shared_ptr<GraphicGraph>& graph) = 0;

    /**
     * Called by the mouse manager to specify where a node and sprite selection started.
     *
     * @param x1 The selection start abscissa.
     * @param y1 The selection start ordinate.
     */
    virtual void beginSelectionAt(double x1, double y1) = 0;

    /**
     * The selection already started grows toward position (x, y).
     *
     * @param x The new end selection abscissa.
     * @param y The new end selection ordinate.
     */
    virtual void selectionGrowsAt(double x, double y) = 0;

    /**
     * Called by the mouse manager to specify where a node and sprite selection stopped.
     *
     * @param x2 The selection stop abscissa.
     * @param y2 The selection stop ordinate.
     */
    virtual void endSelectionAt(double x2, double y2) = 0;

    /**
     * Freeze an element so that the optional layout cannot move it.
     *
     * @param element The element.
     * @param frozen If true the element cannot be moved automatically.
     */
    virtual void freezeElement(const std::shared_ptr<GraphicElement>& element, bool frozen) = 0;

    /**
     * Force an element to move at the given location in pixels.
     *
     * @param element The element.
     * @param x The requested position abscissa in pixels.
     * @param y The requested position ordinate in pixels.
     */
    virtual void moveElementAtPx(const std::shared_ptr<GraphicElement>& element, double x, double y) = 0;

    /**
     * Change the manager for mouse events on this view. If the value for the new manager is null, a default manager is installed.
     * The MouseManager::init method must not yet have been called.
     *
     * @param manager The new manager, or null to set the default manager.
     */
    virtual void setMouseManager(std::shared_ptr<MouseManager> manager) = 0;

    /**
     * Change the manager for key and shortcuts events on this view. If the value for the new manager is null, a default manager is installed.
     * The ShortcutManager::init method must not yet have been called.
     *
     * @param manager The new manager, or null to set the default manager.
     */
    virtual void setShortcutManager(std::shared_ptr<ShortcutManager> manager) = 0;

    /**
     * This is a shortcut to a call setShortcutManager with a MouseOverMouseManager instance and with
     * (InteractiveElement.EDGE, InteractiveElement.NODE, InteractiveElement.SPRITE).
     */
    virtual void enableMouseOptions() = 0;

    /**
     * Request ui focus.
     *
     * @return optional object used to confirm the request
     */
    virtual void* requireFocus() = 0;

    /**
     * Generic method for add a new Listener.
     *
     * @param T Describe the listener
     * @param U Listener
     */
    template <typename T, typename U>
    void addListener(T descriptor, U listener) {
        listeners[descriptor] = listener;
    }

    /**
     * Generic method for remove a Listener.
     *
     * @param T Describe the listener
     * @param U Listener
     */
    template <typename T, typename U>
    void removeListener(T descriptor, U listener) {
        listeners.erase(descriptor);
    }

protected:
    std::map<void*, void*> listeners;
};

#endif // VIEW_HPP
