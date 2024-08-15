#ifndef GRAPH_RENDERER_BASE_HPP
#define GRAPH_RENDERER_BASE_HPP

#include "GraphicGraph.hpp"
#include "StyleGroupListener.hpp"
#include "Selection.hpp"
#include "GraphRenderer.hpp"
#include <stdexcept>

template <typename S, typename G>
class GraphRendererBase : public GraphRenderer<S, G>, public StyleGroupListener {
protected:
    /**
     * The graph to draw.
     */
    GraphicGraph* graph;

    /**
     * Current selection or null.
     */
    Selection* selection;

    /**
     * The surface we are rendering on.
     */
    S renderingSurface;

public:
    GraphRendererBase() : graph(nullptr), selection(nullptr) {}

    virtual ~GraphRendererBase() {
        close();
        delete selection;
    }

    /**
     * Initialise the renderer with the graph and the rendering surface.
     * 
     * @param graph The graphic graph to render.
     * @param renderingSurface The rendering surface.
     */
    void open(GraphicGraph* graph, S renderingSurface) {
        if (this->graph != nullptr)
            throw std::runtime_error("Renderer already open, cannot open twice");

        this->graph = graph;
        this->renderingSurface = renderingSurface;

        this->graph->getStyleGroups().addListener(this);
    }

    /**
     * Close the renderer and clean up resources.
     */
    void close() {
        if (graph != nullptr) {
            graph->getStyleGroups().removeListener(this);
            graph = nullptr;
        }
    }

    /**
     * Get the rendering surface.
     * 
     * @return The rendering surface.
     */
    S getRenderingSurface() const {
        return renderingSurface;
    }

    /**
     * Begin selection at the specified coordinates.
     * 
     * @param x1 The x-coordinate of the start of the selection.
     * @param y1 The y-coordinate of the start of the selection.
     */
    void beginSelectionAt(double x1, double y1) {
        if (selection == nullptr)
            selection = new Selection();

        selection->x1 = x1;
        selection->y1 = y1;
        selection->x2 = x1;
        selection->y2 = y1;
    }

    /**
     * Update the selection as it grows.
     * 
     * @param x The new x-coordinate.
     * @param y The new y-coordinate.
     */
    void selectionGrowsAt(double x, double y) {
        if (selection != nullptr) {
            selection->x2 = x;
            selection->y2 = y;
        }
    }

    /**
     * End the selection.
     * 
     * @param x2 The x-coordinate where the selection ends.
     * @param y2 The y-coordinate where the selection ends.
     */
    void endSelectionAt(double x2, double y2) {
        delete selection;
        selection = nullptr;
    }
};

#endif // GRAPH_RENDERER_BASE_HPP
