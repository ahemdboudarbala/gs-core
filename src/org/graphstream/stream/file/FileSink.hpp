#ifndef FILE_SINK_HPP
#define FILE_SINK_HPP

#include <string>
#include <fstream>
#include "Graph.hpp"

class FileSink {
public:
    virtual ~FileSink() = default;

    // Write the current graph state in one big non-interruptible operation.
    virtual void writeAll(const Graph& graph, const std::string& fileName) = 0;

    // Write the current graph state to an output stream.
    virtual void writeAll(const Graph& graph, std::ostream& stream) = 0;

    // Begin writing graph events to a file.
    virtual void begin(const std::string& fileName) = 0;

    // Begin writing graph events to an output stream.
    virtual void begin(std::ostream& stream) = 0;

    // Ensure all data sent to the output are correctly written.
    virtual void flush() = 0;

    // End the writing process.
    virtual void end() = 0;
};

#endif // FILE_SINK_HPP
