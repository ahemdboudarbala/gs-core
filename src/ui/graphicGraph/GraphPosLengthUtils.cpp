#include "GraphPosLengthUtils.hpp"
#include <stdexcept>
#include <cmath>
#include <typeinfo>

std::vector<double> GraphPosLengthUtils::nodePosition(const Graph& graph, const std::string& id) {
    const Node* node = graph.getNode(id);
    if (!node) {
        throw std::runtime_error("Node '" + id + "' does not exist");
    }
    return nodePosition(*node);
}

Point3 GraphPosLengthUtils::nodePointPosition(const Graph& graph, const std::string& id) {
    const Node* node = graph.getNode(id);
    if (!node) {
        throw std::runtime_error("Node '" + id + "' does not exist");
    }
    return nodePointPosition(*node);
}

std::vector<double> GraphPosLengthUtils::nodePosition(const Node& node) {
    std::vector<double> xyz(3, 0.0);
    nodePosition(node, xyz);
    return xyz;
}

Point3 GraphPosLengthUtils::nodePointPosition(const Node& node) {
    return nodePosition(node, Point3());
}

void GraphPosLengthUtils::nodePosition(const Graph& graph, const std::string& id, std::vector<double>& xyz) {
    const Node* node = graph.getNode(id);
    if (!node) {
        throw std::runtime_error("Node '" + id + "' does not exist");
    }
    nodePosition(*node, xyz);
}

Point3 GraphPosLengthUtils::nodePosition(const Graph& graph, const std::string& id, const Point3& pos) {
    const Node* node = graph.getNode(id);
    if (!node) {
        throw std::runtime_error("Node '" + id + "' does not exist");
    }
    return nodePosition(*node, pos);
}

void GraphPosLengthUtils::nodePosition(const Node& node, std::vector<double>& xyz) {
    if (xyz.size() < 3) return;

    if (node.hasAttribute("xyz") || node.hasAttribute("xy")) {
        auto attr = node.getAttribute("xyz");
        if (!attr.has_value()) {
            attr = node.getAttribute("xy");
        }
        if (attr.has_value()) {
            positionFromObject(attr, xyz);
        }
    } else if (node.hasAttribute("x")) {
        xyz[0] = std::any_cast<double>(node.getAttribute("x"));
        if (node.hasAttribute("y")) {
            xyz[1] = std::any_cast<double>(node.getAttribute("y"));
        }
        if (node.hasAttribute("z")) {
            xyz[2] = std::any_cast<double>(node.getAttribute("z"));
        }
    }
}

Point3 GraphPosLengthUtils::nodePosition(const Node& node, const Point3& pos) {
    if (node.hasAttribute("xyz") || node.hasAttribute("xy")) {
        auto attr = node.getAttribute("xyz");
        if (!attr.has_value()) {
            attr = node.getAttribute("xy");
        }
        if (attr.has_value()) {
            return positionFromObject(attr, pos);
        }
    } else if (node.hasAttribute("x")) {
        double x = std::any_cast<double>(node.getAttribute("x"));
        double y = pos.y;
        double z = pos.z;

        if (node.hasAttribute("y")) {
            y = std::any_cast<double>(node.getAttribute("y"));
        }
        if (node.hasAttribute("z")) {
            z = std::any_cast<double>(node.getAttribute("z"));
        }

        return Point3(x, y, z);
    }
    return pos;
}

void GraphPosLengthUtils::positionFromObject(const std::any& obj, std::vector<double>& xyz) {
    if (obj.type() == typeid(std::vector<std::any>)) {
        auto values = std::any_cast<std::vector<std::any>>(obj);
        xyz = extractXYZ(values);
    } else if (obj.type() == typeid(Point3)) {
        auto point = std::any_cast<Point3>(obj);
        xyz[0] = point.x;
        xyz[1] = point.y;
        xyz[2] = point.z;
    } else if (obj.type() == typeid(Point2)) {
        auto point = std::any_cast<Point2>(obj);
        xyz[0] = point.x;
        xyz[1] = point.y;
        xyz[2] = 0.0;
    } else {
        std::cerr << "Unknown type for position: " << obj.type().name() << std::endl;
    }
}

Point3 GraphPosLengthUtils::positionFromObject(const std::any& obj, const Point3& pos) {
    std::vector<double> xyz = {pos.x, pos.y, pos.z};
    positionFromObject(obj, xyz);
    return Point3(xyz[0], xyz[1], xyz[2]);
}

double GraphPosLengthUtils::edgeLength(const Graph& graph, const std::string& id) {
    const Edge* edge = graph.getEdge(id);
    if (!edge) {
        throw std::runtime_error("Edge '" + id + "' cannot be found");
    }
    return edgeLength(*edge);
}

double GraphPosLengthUtils::edgeLength(const Edge& edge) {
    auto xyz0 = nodePosition(*edge.getNode0());
    auto xyz1 = nodePosition(*edge.getNode1());

    if (xyz0.empty() || xyz1.empty()) return -1.0;

    xyz0[0] = xyz1[0] - xyz0[0];
    xyz0[1] = xyz1[1] - xyz0[1];
    xyz0[2] = xyz1[2] - xyz0[2];

    return std::sqrt(xyz0[0] * xyz0[0] + xyz0[1] * xyz0[1] + xyz0[2] * xyz0[2]);
}

std::vector<double> GraphPosLengthUtils::extractXYZ(const std::vector<std::any>& values) {
    std::vector<double> xyz(3, 0.0);
    if (values.size() > 0 && values[0].type() == typeid(double)) {
        xyz[0] = std::any_cast<double>(values[0]);
        if (values.size() > 1) xyz[1] = std::any_cast<double>(values[1]);
        if (values.size() > 2) xyz[2] = std::any_cast<double>(values[2]);
    }
    return xyz;
}
