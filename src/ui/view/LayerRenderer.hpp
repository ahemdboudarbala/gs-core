#ifndef LAYER_RENDERER_HPP
#define LAYER_RENDERER_HPP

#include "ui/graphicGraph/GraphicGraph.hpp"  // Assuming you have a corresponding C++ class for GraphicGraph

template <typename G>
class LayerRenderer {
public:
    virtual ~LayerRenderer() = default;

    /**
     * Render something under or above the graph.
     * 
     * @param graphics The graphics module used to draw (for example, a custom graphics class).
     * @param graph The graphic representation of the graph.
     * @param px2Gu The ratio to pass from pixels to graph units.
     * @param widthPx The width in pixels of the view port.
     * @param heightPx The height in pixels of the view port.
     * @param minXGu The minimum visible point abscissa of the graph in graph units.
     * @param minYGu The minimum visible point ordinate of the graph in graph units.
     * @param maxXGu The maximum visible point abscissa of the graph in graph units.
     * @param maxYGu The maximum visible point ordinate of the graph in graph units.
     */
    virtual void render(G& graphics, GraphicGraph& graph, double px2Gu, int widthPx, int heightPx,
                        double minXGu, double minYGu, double maxXGu, double maxYGu) = 0;
};

#endif // LAYER_RENDERER_HPP
