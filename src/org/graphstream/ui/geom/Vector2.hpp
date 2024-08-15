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

/**
 * @since 2009-12-22
 * 
 * @author Antoine Dutot <antoine.dutot@graphstream-project.org>
 * @author Guilhelm Savin <guilhelm.savin@graphstream-project.org>
 * @author Richard O. Legendi <richard.legendi@gmail.com>
 * @author Alex Bowen <bowen.a@gmail.com>
 * @author kitskub <kitskub@gmail.com>
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 */

#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "Point2.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <array>

class Vector2 {
public:
    std::array<double, 2> data;

    // Constructors
    Vector2();
    Vector2(double x, double y);
    Vector2(const Vector2& other);
    Vector2(const Point2& point);
    Vector2(const Point2& from, const Point2& to);

    // Predicates
    bool isZero() const;
    bool validComponent(int i) const;

    // Accessors
    double at(int i) const;
    double x() const;
    double y() const;
    double dotProduct(double ox, double oy) const;
    double dotProduct(const Vector2& other) const;
    double length() const;
    void fill(double value);
    void set(int i, double value);
    void set(double x, double y);

    // Operations
    void add(const Vector2& other);
    void sub(const Vector2& other);
    void mult(const Vector2& other);
    void scalarAdd(double value);
    void scalarSub(double value);
    void scalarMult(double value);
    void scalarDiv(double value);
    double normalize();

    // Utility
    void copy(const Vector2& other);
    void copy(const Point2& point);

    // Misc
    std::string toString() const;
    std::size_t hashCode() const;
    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;
    Vector2 clone() const;

    friend std::ostream& operator<<(std::ostream& os, const Vector2& vector);
};

#endif // VECTOR2_HPP
