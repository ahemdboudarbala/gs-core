#ifndef GRAPH_RENDERER_HPP
#define GRAPH_RENDERER_HPP

#include "GraphicElement.hpp"
#include "GraphicGraph.hpp"
#include "InteractiveElement.hpp"
#include "View.hpp"
#include "Viewer.hpp"
#include "LayerRenderer.hpp"
#include <vector>
#include <memory>
#include <string>

template <typename S, typename G>
class GraphRenderer {
public:
    virtual ~GraphRenderer() = default;

    // Initialisation
    virtual void open(GraphicGraph* graph, S drawingSurface) = 0;
    virtual void close() = 0;

    // Access
    virtual std::unique_ptr<View> createDefaultView(Viewer* viewer, const std::string& id) = 0;

    /**
     * Get a camera object to provide control commands on the view.
     * 
     * @return a Camera instance
     */
    virtual std::unique_ptr<Camera> getCamera() = 0;

    /**
     * Search for the first GraphicElement among the specified types (precedence:
     * node, edge, sprite) that contains the point at coordinates (x, y).
     *
     * @param types
     *            The types to check
     * @param x
     *            The point abscissa.
     * @param y
     *            The point ordinate.
     * @return The first GraphicElement among the specified types at the given
     *         coordinates or null if nothing found.
     */
    virtual GraphicElement* findGraphicElementAt(const std::vector<InteractiveElement>& types, double x, double y) = 0;

    /**
     * Search for all the graphic elements of the specified types contained inside
     * the rectangle (x1,y1)-(x2,y2).
     *
     * @param types
     *            The types to check
     * @param x1
     *            The rectangle lowest point abscissa.
     * @param y1
     *            The rectangle lowest point ordinate.
     * @param x2
     *            The rectangle highest point abscissa.
     * @param y2
     *            The rectangle highest point ordinate.
     * @return The set of GraphicElements in the given rectangle.
     */
    virtual std::vector<GraphicElement*> allGraphicElementsIn(
        const std::vector<InteractiveElement>& types, double x1, double y1, double x2, double y2) = 0;

    // Command

    /**
     * Redisplay or update the graph.
     */
    virtual void render(G g, int x, int y, int width, int height) = 0;

    /**
     * Called by the mouse manager to specify where a node and sprite selection
     * started.
     * 
     * @param x1
     *            The selection start abscissa.
     * @param y1
     *            The selection start ordinate.
     */
    virtual void beginSelectionAt(double x1, double y1) = 0;

    /**
     * The selection already started grows toward position (x, y).
     * 
     * @param x
     *            The new end selection abscissa.
     * @param y
     *            The new end selection ordinate.
     */
    virtual void selectionGrowsAt(double x, double y) = 0;

    /**
     * Called by the mouse manager to specify where a node and sprite selection
     * stopped.
     * 
     * @param x2
     *            The selection stop abscissa.
     * @param y2
     *            The selection stop ordinate.
     */
    virtual void endSelectionAt(double x2, double y2) = 0;

    /**
     * Force an element to move at the given location in pixels.
     * 
     * @param element
     *            The element.
     * @param x
     *            The requested position abscissa in pixels.
     * @param y
     *            The requested position ordinate in pixels.
     */
    virtual void moveElementAtPx(GraphicElement* element, double x, double y) = 0;

    /**
     * Take a screenshot of the current view.
     * 
     * @param filename
     *            The filename to save the screenshot.
     * @param width
     *            The width of the screenshot.
     * @param height
     *            The height of the screenshot.
     */
    virtual void screenshot(const std::string& filename, int width, int height) = 0;

    /**
     * Set a layer renderer that will be called each time the graph needs to be
     * redrawn before the graph is rendered. Pass "null" to remove the layer
     * renderer.
     * 
     * @param renderer
     *            The renderer (or null to remove it).
     */
    virtual void setBackLayerRenderer(std::unique_ptr<LayerRenderer<G>> renderer) = 0;

    /**
     * Set a layer renderer that will be called each time the graph needs to be
     * redrawn after the graph is rendered. Pass "null" to remove the layer
     * renderer.
     * 
     * @param renderer
     *            The renderer (or null to remove it).
     */
    virtual void setForeLayoutRenderer(std::unique_ptr<LayerRenderer<G>> renderer) = 0;
};

#endif // GRAPH_RENDERER_HPP
