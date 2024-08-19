#ifndef GRAPHFACTORY_HPP
#define GRAPHFACTORY_HPP

#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <map>
#include <functional>
#include "Graph.hpp"

class GraphFactory {
public:
    using GraphCreator = std::function<std::shared_ptr<Graph>(const std::string&)>;

    GraphFactory() = default;

    void registerGraph(const std::string& graphClass, GraphCreator creator) {
        creators[graphClass] = creator;
    }

    std::shared_ptr<Graph> newInstance(const std::string& id, const std::string& graphClass) const {
        auto it = creators.find(graphClass);
        if (it != creators.end()) {
            return it->second(id);
        } else {
            std::cerr << "Graph class '" << graphClass << "' not found." << std::endl;
            return nullptr;
        }
    }

private:
    std::map<std::string, GraphCreator> creators;
};

#endif // GRAPHFACTORY_HPP
