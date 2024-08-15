/*
 * This file is part of GraphStream <http://graphstream-project.org>.
 * 
 * GraphStream is a library whose purpose is to handle static or dynamic
 * graph, create them from scratch, file or any source and display them.
 * 
 * This program is free software distributed under the terms of two licenses, the
 * CeCILL-C license that fits European law, and the GNU Lesser General Public
 * License. You can use, modify and/or redistribute the software under the terms
 * of the CeCILL-C license as circulated by CEA, CNRS and INRIA at the following
 * URL <http://www.cecill.info> or under the terms of the GNU LGPL as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-C and LGPL licenses and that you accept their terms.
 */

#include "Point2.hpp"

// Initialize static constant
const Point2 Point2::NULL_POINT2(0, 0);

// Constructors
Point2::Point2() : x(0), y(0) {}

Point2::Point2(double x, double y) : x(x), y(y) {}

Point2::Point2(const Point2& other) {
    copy(other);
}

// Methods
void Point2::set(double x, double y) {
    this->x = x;
    this->y = y;
}

void Point2::make(double x, double y) {
    set(x, y);
}

bool Point2::isZero() const {
    return (x == 0 && y == 0);
}

Point2 Point2::interpolate(const Point2& other, double factor) const {
    return Point2(x + ((other.x - x) * factor), y + ((other.y - y) * factor));
}

double Point2::distance(const Point2& other) const {
    double xx = other.x - x;
    double yy = other.y - y;
    return std::abs(std::sqrt((xx * xx) + (yy * yy)));
}

void Point2::copy(const Point2& other) {
    x = other.x;
    y = other.y;
}

void Point2::moveTo(double x, double y) {
    this->x = x;
    this->y = y;
}

void Point2::move(double dx, double dy) {
    this->x += dx;
    this->y += dy;
}

void Point2::move(const Point2& p) {
    this->x += p.x;
    this->y += p.y;
}

void Point2::moveX(double dx) {
    x += dx;
}

void Point2::moveY(double dy) {
    y += dy;
}

void Point2::scale(double sx, double sy) {
    x *= sx;
    y *= sy;
}

void Point2::scale(const Point2& s) {
    x *= s.x;
    y *= s.y;
}

void Point2::setX(double x) {
    this->x = x;
}

void Point2::setY(double y) {
    this->y = y;
}

void Point2::swap(Point2& other) {
    if (&other != this) {
        std::swap(this->x, other.x);
        std::swap(this->y, other.y);
    }
}

// Misc
std::string Point2::toString() const {
    return "Point2[" + std::to_string(x) + "|" + std::to_string(y) + "]";
}

bool Point2::operator==(const Point2& other) const {
    return x == other.x && y == other.y;
}

bool Point2::operator!=(const Point2& other) const {
    return !(*this == other);
}

std::size_t Point2::hashCode() const {
    std::size_t result = std::hash<double>{}(x);
    result = 31 * result + std::hash<double>{}(y);
    return result;
}

std::ostream& operator<<(std::ostream& os, const Point2& point) {
    os << point.toString();
    return os;
}
