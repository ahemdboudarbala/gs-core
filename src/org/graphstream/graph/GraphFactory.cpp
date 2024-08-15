#include "GraphFactory.hpp"
#include "SimpleGraph.hpp"

GraphFactory::GraphFactory() {
    // Register built-in graph types here if necessary
    registerGraph("SimpleGraph", [](const std::string& id) {
        return std::make_shared<SimpleGraph>(id);
    });
}

std::shared_ptr<Graph> GraphFactory::newInstance(const std::string& id, const std::string& graphClass) const {
    auto it = creators.find(graphClass);
    if (it != creators.end()) {
        return it->second(id);
    } else {
        std::cerr << "Graph class '" << graphClass << "' not found." << std::endl;
        return nullptr;
    }
}
