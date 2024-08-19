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

#include "Vector2.hpp"
#include <algorithm>

// Constructors
Vector2::Vector2() {
    data.fill(0);
}

Vector2::Vector2(double x, double y) {
    data[0] = x;
    data[1] = y;
}

Vector2::Vector2(const Vector2& other) {
    copy(other);
}

Vector2::Vector2(const Point2& point) {
    copy(point);
}

Vector2::Vector2(const Point2& from, const Point2& to) {
    data[0] = to.x - from.x;
    data[1] = to.y - from.y;
}

// Predicates
bool Vector2::isZero() const {
    return (data[0] == 0 && data[1] == 0);
}

bool Vector2::validComponent(int i) const {
    return (i >= 0 && i < 2);
}

// Accessors
double Vector2::at(int i) const {
    return data[i];
}

double Vector2::x() const {
    return data[0];
}

double Vector2::y() const {
    return data[1];
}

double Vector2::dotProduct(double ox, double oy) const {
    return ((data[0] * ox) + (data[1] * oy));
}

double Vector2::dotProduct(const Vector2& other) const {
    return ((data[0] * other.data[0]) + (data[1] * other.data[1]));
}

double Vector2::length() const {
    return std::sqrt((data[0] * data[0]) + (data[1] * data[1]));
}

// Operations
void Vector2::fill(double value) {
    data[0] = data[1] = value;
}

void Vector2::set(int i, double value) {
    data[i] = value;
}

void Vector2::set(double x, double y) {
    data[0] = x;
    data[1] = y;
}

void Vector2::add(const Vector2& other) {
    data[0] += other.data[0];
    data[1] += other.data[1];
}

void Vector2::sub(const Vector2& other) {
    data[0] -= other.data[0];
    data[1] -= other.data[1];
}

void Vector2::mult(const Vector2& other) {
    data[0] *= other.data[0];
    data[1] *= other.data[1];
}

void Vector2::scalarAdd(double value) {
    data[0] += value;
    data[1] += value;
}

void Vector2::scalarSub(double value) {
    data[0] -= value;
    data[1] -= value;
}

void Vector2::scalarMult(double value) {
    data[0] *= value;
    data[1] *= value;
}

void Vector2::scalarDiv(double value) {
    data[0] /= value;
    data[1] /= value;
}

double Vector2::normalize() {
    double len = length();
    if (len != 0) {
        data[0] /= len;
        data[1] /= len;
    }
    return len;
}

// Utility
void Vector2::copy(const Vector2& other) {
    data[0] = other.data[0];
    data[1] = other.data[1];
}

void Vector2::copy(const Point2& point) {
    data[0] = point.x;
    data[1] = point.y;
}

// Misc
std::string Vector2::toString() const {
    return "[" + std::to_string(data[0]) + "|" + std::to_string(data[1]) + "]";
}

std::size_t Vector2::hashCode() const {
    return std::hash<double>{}(data[0]) ^ std::hash<double>{}(data[1]);
}

bool Vector2::operator==(const Vector2& other) const {
    return data[0] == other.data[0] && data[1] == other.data[1];
}

bool Vector2::operator!=(const Vector2& other) const {
    return !(*this == other);
}

Vector2 Vector2::clone() const {
    return Vector2(*this);
}

std::ostream& operator<<(std::ostream& os, const Vector2& vector) {
    os << vector.toString();
    return os;
}
