#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <memory>
#include <stdexcept>
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>
#include "Element.hpp"
#include "Node.hpp"
#include "Edge.hpp"
#include "stream/Pipe.hpp"
#include "Structure.hpp"
#include "stream/file/FileSource.hpp"
#include "stream/file/FileSink.hpp"
#include "stream/GraphParseException.hpp"
#include "ElementNotFoundException.hpp"
#include "IdAlreadyInUseException.hpp"
#include "EdgeRejectedException.hpp"
#include "NodeFactory.hpp"
#include "EdgeFactory.hpp"
#include "ui/view/Viewer.hpp"

class Graph : public Element, public Pipe, public Structure {
public:
    virtual ~Graph() = default;

    // Access
    virtual std::shared_ptr<Node> getNode(const std::string& id) = 0;
    virtual std::shared_ptr<Edge> getEdge(const std::string& id) = 0;

    virtual std::shared_ptr<NodeFactory<Node>> nodeFactory() = 0;
    virtual std::shared_ptr<EdgeFactory<Edge>> edgeFactory() = 0;

    virtual bool isStrict() const = 0;
    virtual bool isAutoCreationEnabled() const = 0;
    virtual double getStep() const = 0;

    // Command
    virtual void setNodeFactory(std::shared_ptr<NodeFactory<Node>> nf) = 0;
    virtual void setEdgeFactory(std::shared_ptr<EdgeFactory<Edge>> ef) = 0;
    virtual void setStrict(bool on) = 0;
    virtual void setAutoCreate(bool on) = 0;

    // Graph construction
    virtual void clear() = 0;
    virtual std::shared_ptr<Node> addNode(const std::string& id) = 0;
    virtual std::shared_ptr<Edge> addEdge(const std::string& id, const std::string& node1, const std::string& node2) = 0;
    virtual std::shared_ptr<Edge> addEdge(const std::string& id, const std::string& from, const std::string& to, bool directed) = 0;
    virtual void stepBegins(double time) = 0;

    // Source
    virtual std::vector<std::shared_ptr<AttributeSink>> attributeSinks() = 0;
    virtual std::vector<std::shared_ptr<ElementSink>> elementSinks() = 0;

    // Utility shortcuts
    virtual void read(const std::string& filename) = 0;
    virtual void read(std::shared_ptr<FileSource> input, const std::string& filename) = 0;
    virtual void write(const std::string& filename) = 0;
    virtual void write(std::shared_ptr<FileSink> output, const std::string& filename) = 0;

    virtual std::shared_ptr<Viewer> display() = 0;
    virtual std::shared_ptr<Viewer> display(bool autoLayout) = 0;

    // New methods
    virtual std::shared_ptr<Node> getNode(int index) = 0;
    virtual std::shared_ptr<Edge> getEdge(int index) = 0;

    virtual std::shared_ptr<Edge> addEdge(const std::string& id, int index1, int index2) = 0;
    virtual std::shared_ptr<Edge> addEdge(const std::string& id, int fromIndex, int toIndex, bool directed) = 0;
    virtual std::shared_ptr<Edge> addEdge(const std::string& id, std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) = 0;
    virtual std::shared_ptr<Edge> addEdge(const std::string& id, std::shared_ptr<Node> from, std::shared_ptr<Node> to, bool directed) = 0;

    virtual std::shared_ptr<Edge> removeEdge(int index) = 0;
    virtual std::shared_ptr<Edge> removeEdge(int fromIndex, int toIndex) = 0;
    virtual std::shared_ptr<Edge> removeEdge(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) = 0;
    virtual std::shared_ptr<Edge> removeEdge(const std::string& from, const std::string& to) = 0;
    virtual std::shared_ptr<Edge> removeEdge(const std::string& id) = 0;
    virtual std::shared_ptr<Edge> removeEdge(std::shared_ptr<Edge> edge) = 0;

    virtual std::shared_ptr<Node> removeNode(int index) = 0;
    virtual std::shared_ptr<Node> removeNode(const std::string& id) = 0;
    virtual std::shared_ptr<Node> removeNode(std::shared_ptr<Node> node) = 0;
};

#endif // GRAPH_HPP
