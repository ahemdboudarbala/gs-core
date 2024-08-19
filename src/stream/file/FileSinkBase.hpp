#ifndef FILE_SINK_BASE_HPP
#define FILE_SINK_BASE_HPP

#include "Graph.hpp"
#include <fstream>
#include <iostream>
#include <atomic>

class FileSinkBase {
public:
    virtual ~FileSinkBase() = default;

    // Write the current graph state in one big non-interruptible operation.
    void writeAll(const Graph& graph, const std::string& fileName) {
        begin(fileName);
        exportGraph(graph);
        end();
    }

    // Write the current graph state to an output stream.
    void writeAll(const Graph& graph, std::ostream& stream) {
        begin(stream);
        exportGraph(graph);
        end();
    }

    // Write the current graph state to a writer.
    void writeAll(const Graph& graph, std::ostream& writer) {
        begin(writer);
        exportGraph(graph);
        end();
    }

    // Begin writing graph events to a file.
    void begin(const std::string& fileName) {
        if (output.is_open())
            throw std::runtime_error("Cannot call begin() twice without calling end() before.");

        output.open(fileName);
        if (!output.is_open())
            throw std::runtime_error("Failed to open file: " + fileName);

        outputHeader();
    }

    // Begin writing graph events to an output stream.
    void begin(std::ostream& stream) {
        if (outputStream != nullptr)
            throw std::runtime_error("Cannot call begin() twice without calling end() before.");

        outputStream = &stream;
        outputHeader();
    }

    // Flush the output.
    void flush() {
        if (output.is_open()) {
            output.flush();
        }
        if (outputStream != nullptr) {
            outputStream->flush();
        }
    }

    // End the writing process.
    void end() {
        outputEndOfFile();
        flush();
        if (output.is_open()) {
            output.close();
        }
        outputStream = nullptr;
    }

protected:
    // The output stream for file output.
    std::ofstream output;

    // The output stream for stream output.
    std::ostream* outputStream = nullptr;

    // Export the entire graph.
    void exportGraph(const Graph& graph) {
        const std::string& graphId = graph.getId();
        std::atomic<long> timeId(0);

        for (const auto& key : graph.getAttributeKeys()) {
            graphAttributeAdded(graphId, timeId++, key, graph.getAttribute(key));
        }

        for (const auto& node : graph.getNodes()) {
            const std::string& nodeId = node.getId();
            nodeAdded(graphId, timeId++, nodeId);

            for (const auto& key : node.getAttributeKeys()) {
                nodeAttributeAdded(graphId, timeId++, nodeId, key, node.getAttribute(key));
            }
        }

        for (const auto& edge : graph.getEdges()) {
            const std::string& edgeId = edge.getId();
            edgeAdded(graphId, timeId++, edgeId, edge.getNode0().getId(), edge.getNode1().getId(), edge.isDirected());

            for (const auto& key : edge.getAttributeKeys()) {
                edgeAttributeAdded(graphId, timeId++, edgeId, key, edge.getAttribute(key));
            }
        }
    }

    // Method to be overridden by subclasses to write the header.
    virtual void outputHeader() = 0;

    // Method to be overridden by subclasses to write the end of the file.
    virtual void outputEndOfFile() = 0;

    // Abstract methods that subclasses should implement to handle graph events.
    virtual void graphAttributeAdded(const std::string& graphId, long timeId, const std::string& key, const std::string& value) = 0;
    virtual void nodeAdded(const std::string& graphId, long timeId, const std::string& nodeId) = 0;
    virtual void nodeAttributeAdded(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& key, const std::string& value) = 0;
    virtual void edgeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& node0Id, const std::string& node1Id, bool directed) = 0;
    virtual void edgeAttributeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& key, const std::string& value) = 0;
};

#endif // FILE_SINK_BASE_HPP
