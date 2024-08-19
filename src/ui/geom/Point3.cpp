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

#include "Point3.hpp"

// Initialize static constant
const Point3 Point3::NULL_POINT3(0, 0, 0);

// Constructors
Point3::Point3() : Point2(), z(0) {}

Point3::Point3(double x, double y) : Point2(x, y), z(0) {}

Point3::Point3(double x, double y, double z) : Point2(x, y), z(z) {}

Point3::Point3(const Point3& other) {
    copy(other);
}

Point3::Point3(const Vector3& vec) {
    copy(vec);
}

Point3::Point3(int start, const float data[]) {
    if (data) {
        if (data[start] != 0) x = data[start];
        if (data[start + 1] != 0) y = data[start + 1];
        if (data[start + 2] != 0) z = data[start + 2];
    }
}

Point3::Point3(int start, const double data[]) {
    if (data) {
        if (data[start] != 0) x = data[start];
        if (data[start + 1] != 0) y = data[start + 1];
        if (data[start + 2] != 0) z = data[start + 2];
    }
}

// Methods
void Point3::set(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

bool Point3::isZero() const {
    return (x == 0 && y == 0 && z == 0);
}

Point3 Point3::interpolate(const Point3& other, double factor) const {
    return Point3(x + ((other.x - x) * factor), y + ((other.y - y) * factor), z + ((other.z - z) * factor));
}

double Point3::distance(const Point3& other) const {
    double xx = other.x - x;
    double yy = other.y - y;
    double zz = other.z - z;
    return std::abs(std::sqrt((xx * xx) + (yy * yy) + (zz * zz)));
}

double Point3::distance(double x, double y, double z) const {
    double xx = x - this->x;
    double yy = y - this->y;
    double zz = z - this->z;
    return std::abs(std::sqrt((xx * xx) + (yy * yy) + (zz * zz)));
}

void Point3::copy(const Point3& other) {
    x = other.x;
    y = other.y;
    z = other.z;
}

void Point3::copy(const Vector3& vec) {
    x = vec.data[0];
    y = vec.data[1];
    z = vec.data[2];
}

void Point3::moveTo(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Point3::move(double dx, double dy, double dz) {
    this->x += dx;
    this->y += dy;
    this->z += dz;
}

void Point3::move(const Point3& p) {
    this->x += p.x;
    this->y += p.y;
    this->z += p.z;
}

void Point3::move(const Vector3& d) {
    this->x += d.data[0];
    this->y += d.data[1];
    this->z += d.data[2];
}

void Point3::moveZ(double dz) {
    z += dz;
}

void Point3::scale(double sx, double sy, double sz) {
    x *= sx;
    y *= sy;
    z *= sz;
}

void Point3::scale(const Point3& s) {
    x *= s.x;
    y *= s.y;
    z *= s.z;
}

void Point3::scale(const Vector3& s) {
    x *= s.data[0];
    y *= s.data[1];
    z *= s.data[2];
}

void Point3::scale(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
}

void Point3::setZ(double z) {
    this->z = z;
}

void Point3::swap(Point3& other) {
    if (&other != this) {
        std::swap(this->x, other.x);
        std::swap(this->y, other.y);
        std::swap(this->z, other.z);
    }
}

// Misc
std::string Point3::toString() const {
    return "Point3[" + std::to_string(x) + "|" + std::to_string(y) + "|" + std::to_string(z) + "]";
}

bool Point3::operator==(const Point3& other) const {
    return x == other.x && y == other.y && z == other.z;
}

bool Point3::operator!=(const Point3& other) const {
    return !(*this == other);
}

std::size_t Point3::hashCode() const {
    std::size_t result = Point2::hashCode();
    std::size_t temp = std::hash<double>{}(z);
    result = 31 * result + temp;
    return result;
}

std::ostream& operator<<(std::ostream& os, const Point3& point) {
    os << point.toString();
    return os;
}
