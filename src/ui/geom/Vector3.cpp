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

#include "Vector3.hpp"

// Constructors
Vector3::Vector3() {
    data = std::array<double, 3>{0, 0, 0};
}

Vector3::Vector3(double x, double y, double z) {
    data = std::array<double, 3>{x, y, z};
}

Vector3::Vector3(const Vector3& other) {
    copy(other);
}

Vector3::Vector3(const Point3& point) {
    data = std::array<double, 3>{point.x, point.y, point.z};
}

// Predicates
bool Vector3::isZero() const {
    return (data[0] == 0 && data[1] == 0 && data[2] == 0);
}

bool Vector3::validComponent(int i) const {
    return (i >= 0 && i < 3);
}

// Access
Vector3 Vector3::clone() const {
    return Vector3(*this);
}

double Vector3::dotProduct(double ox, double oy, double oz) const {
    return (data[0] * ox) + (data[1] * oy) + (data[2] * oz);
}

double Vector3::dotProduct(const Vector3& other) const {
    return (data[0] * other.data[0]) + (data[1] * other.data[1]) + (data[2] * other.data[2]);
}

double Vector3::length() const {
    return std::sqrt((data[0] * data[0]) + (data[1] * data[1]) + (data[2] * data[2]));
}

double Vector3::z() const {
    return data[2];
}

// Commands
void Vector3::fill(double value) {
    data[0] = data[1] = data[2] = value;
}

void Vector3::set(int i, double value) {
    data[i] = value;
}

void Vector3::set(double x, double y, double z) {
    data[0] = x;
    data[1] = y;
    data[2] = z;
}

void Vector3::add(const Vector3& other) {
    data[0] += other.data[0];
    data[1] += other.data[1];
    data[2] += other.data[2];
}

void Vector3::sub(const Vector3& other) {
    data[0] -= other.data[0];
    data[1] -= other.data[1];
    data[2] -= other.data[2];
}

void Vector3::mult(const Vector3& other) {
    data[0] *= other.data[0];
    data[1] *= other.data[1];
    data[2] *= other.data[2];
}

void Vector3::scalarAdd(double value) {
    data[0] += value;
    data[1] += value;
    data[2] += value;
}

void Vector3::scalarSub(double value) {
    data[0] -= value;
    data[1] -= value;
    data[2] -= value;
}

void Vector3::scalarMult(double value) {
    data[0] *= value;
    data[1] *= value;
    data[2] *= value;
}

void Vector3::scalarDiv(double value) {
    data[0] /= value;
    data[1] /= value;
    data[2] /= value;
}

void Vector3::crossProduct(const Vector3& other) {
    double x = (data[1] * other.data[2]) - (data[2] * other.data[1]);
    double y = (data[2] * other.data[0]) - (data[0] * other.data[2]);
    data[2] = (data[0] * other.data[1]) - (data[1] * other.data[0]);
    data[0] = x;
    data[1] = y;
}

void Vector3::crossProduct(const Vector3& A, const Vector3& B) {
    data[0] = (A.data[1] * B.data[2]) - (A.data[2] * B.data[1]);
    data[1] = (A.data[2] * B.data[0]) - (A.data[0] * B.data[2]);
    data[2] = (A.data[0] * B.data[1]) - (A.data[1] * B.data[0]);
}

double Vector3::normalize() {
    double len = length();
    if (len != 0) {
        data[0] /= len;
        data[1] /= len;
        data[2] /= len;
    }
    return len;
}

// Utility
void Vector3::copy(const Vector3& other) {
    data[0] = other.data[0];
    data[1] = other.data[1];
    data[2] = other.data[2];
}

void Vector3::copy(const Point3& point) {
    data[0] = point.x;
    data[1] = point.y;
    data[2] = point.z;
}

// Misc
std::string Vector3::toString() const {
    return "[" + std::to_string(data[0]) + "|" + std::to_string(data[1]) + "|" + std::to_string(data[2]) + "]";
}
