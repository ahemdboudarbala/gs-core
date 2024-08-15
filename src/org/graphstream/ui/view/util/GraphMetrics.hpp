#ifndef GRAPH_METRICS_HPP
#define GRAPH_METRICS_HPP

#include <cmath>
#include <iostream>
#include <sstream>
#include <array>
#include "Point3.hpp"
#include "Vector3.hpp"
#include "StyleConstants.hpp"
#include "Values.hpp"

/**
 * Various geometric informations on the graphic graph.
 * 
 * This class extends the GraphMetrics to provide not only metrics on the
 * graphic graph but also on the rendering canvas, and allow to convert from
 * graph metrics to canvas metrics and the reverse.
 */
class GraphMetrics {
public:
    // Public attributes
    Point3 lo;
    Point3 hi;
    Point3 loVisible;
    Point3 hiVisible;
    Vector3 size;
    double diagonal;
    std::array<double, 4> viewport;
    double ratioPx2Gu;
    double px1;

    // Constructor
    GraphMetrics() {
        setDefaults();
    }

    // Access methods
    double getDiagonal() const {
        return diagonal;
    }

    Vector3 getSize() const {
        return size;
    }

    Point3 getLowPoint() const {
        return lo;
    }

    Point3 getHighPoint() const {
        return hi;
    }

    double graphWidthGU() const {
        return hi.x - lo.x;
    }

    double graphHeightGU() const {
        return hi.y - lo.y;
    }

    double graphDepthGU() const {
        return hi.z - lo.z;
    }

    double lengthToGu(double value, StyleConstants::Units units) const {
        switch (units) {
            case StyleConstants::Units::PX:
                return value / ratioPx2Gu;
            case StyleConstants::Units::PERCENTS:
                return diagonal * value;
            case StyleConstants::Units::GU:
            default:
                return value;
        }
    }

    double lengthToGu(const Value& value) const {
        return lengthToGu(value.value, value.units);
    }

    double lengthToGu(const Values& values, int index) const {
        return lengthToGu(values.get(index), values.units);
    }

    double lengthToPx(double value, StyleConstants::Units units) const {
        switch (units) {
            case StyleConstants::Units::GU:
                return value * ratioPx2Gu;
            case StyleConstants::Units::PERCENTS:
                return (diagonal * value) * ratioPx2Gu;
            case StyleConstants::Units::PX:
            default:
                return value;
        }
    }

    double lengthToPx(const Value& value) const {
        return lengthToPx(value.value, value.units);
    }

    double lengthToPx(const Values& values, int index) const {
        return lengthToPx(values.get(index), values.units);
    }

    double positionPixelToGu(int pixels, int index) const {
        double l = lengthToGu(static_cast<double>(pixels), StyleConstants::Units::PX);

        switch (index) {
            case 0:
                l -= graphWidthGU() / 2.0;
                l = (hi.x + lo.x) / 2.0 + l;
                break;
            case 1:
                l -= graphHeightGU() / 2.0;
                l = (hi.y + lo.y) / 2.0 + l;
                break;
            default:
                throw std::invalid_argument("Invalid index");
        }

        return l;
    }

    // Command methods
    void setViewport(double viewportX, double viewportY, double viewportWidth, double viewportHeight) {
        viewport[0] = viewportX;
        viewport[1] = viewportY;
        viewport[2] = viewportWidth;
        viewport[3] = viewportHeight;
    }

    void setRatioPx2Gu(double ratio) {
        if (ratio > 0) {
            ratioPx2Gu = ratio;
            px1 = 0.95 / ratioPx2Gu;
        } else if (ratio == 0) {
            throw std::runtime_error("ratio PX to GU cannot be zero");
        } else if (ratio < 0) {
            throw std::runtime_error("ratio PX to GU cannot be negative");
        }
    }

    void setBounds(double minx, double miny, double minz, double maxx, double maxy, double maxz) {
        lo.set(minx, miny, minz);
        hi.set(maxx, maxy, maxz);

        size.set(hi.x - lo.x, hi.y - lo.y, hi.z - lo.z);
        diagonal = std::sqrt(size.data[0] * size.data[0] + size.data[1] * size.data[1] + size.data[2] * size.data[2]);
    }

    // String representation
    std::string toString() const {
        std::ostringstream oss;
        oss << "Graph Metrics :" << std::endl;
        oss << "        lo         = " << lo.toString() << std::endl;
        oss << "        hi         = " << hi.toString() << std::endl;
        oss << "        visible lo = " << loVisible.toString() << std::endl;
        oss << "        visible hi = " << hiVisible.toString() << std::endl;
        oss << "        size       = " << size.toString() << std::endl;
        oss << "        diag       = " << diagonal << std::endl;
        oss << "        viewport   = {" << viewport[0] << ", " << viewport[1] << ", " << viewport[2] << ", " << viewport[3] << "}" << std::endl;
        oss << "        ratio      = " << ratioPx2Gu << "px = 1gu" << std::endl;
        return oss.str();
    }

private:
    // Private methods
    void setDefaults() {
        lo.set(-1, -1, -1);
        hi.set(1, 1, 1);
        size.set(2, 2, 2);

        diagonal = 1;
        ratioPx2Gu = 1;
        px1 = 1;
    }
};

#endif // GRAPH_METRICS_HPP
