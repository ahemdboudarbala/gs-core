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
 * @author kitskub <kitskub@gmail.com>
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 */

#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "Vector2.hpp"
#include "Point3.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <array>

class Vector3 : public Vector2 {
public:
    // Constructors
    Vector3();
    Vector3(double x, double y, double z);
    Vector3(const Vector3& other);
    Vector3(const Point3& point);

    // Predicates
    bool isZero() const override;
    bool validComponent(int i) const override;

    // Access
    Vector3 clone() const override;
    double dotProduct(double ox, double oy, double oz) const;
    double dotProduct(const Vector3& other) const;
    double length() const override;
    double z() const;

    // Commands
    void fill(double value) override;
    void set(int i, double value) override;
    void set(double x, double y, double z);
    void add(const Vector3& other);
    void sub(const Vector3& other);
    void mult(const Vector3& other);
    void scalarAdd(double value) override;
    void scalarSub(double value) override;
    void scalarMult(double value) override;
    void scalarDiv(double value) override;
    void crossProduct(const Vector3& other);
    void crossProduct(const Vector3& A, const Vector3& B);
    double normalize() override;

    // Utility
    void copy(const Vector3& other);
    void copy(const Point3& point);

    // Misc
    std::string toString() const override;
};

#endif // VECTOR3_HPP
