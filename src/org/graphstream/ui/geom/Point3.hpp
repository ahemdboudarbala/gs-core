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
 * @author Alex Bowen <bowen.a@gmail.com>
 * @author kitskub <kitskub@gmail.com>
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 */

#ifndef POINT3_HPP
#define POINT3_HPP

#include "Point2.hpp"
#include "Vector3.hpp"
#include <cmath>
#include <iostream>
#include <string>

class Point3 : public Point2 {
public:
    // Attributes
    double z;

    // Shared Attributes
    static const Point3 NULL_POINT3;

    // Constructors
    Point3();
    Point3(double x, double y);
    Point3(double x, double y, double z);
    Point3(const Point3& other);
    Point3(const Vector3& vec);
    Point3(int start, const float data[]);
    Point3(int start, const double data[]);

    // Methods
    void set(double x, double y, double z);
    bool isZero() const override;
    Point3 interpolate(const Point3& other, double factor) const;
    double distance(const Point3& other) const;
    double distance(double x, double y, double z) const;
    void copy(const Point3& other);
    void copy(const Vector3& vec);
    void moveTo(double x, double y, double z);
    void move(double dx, double dy, double dz);
    void move(const Point3& p);
    void move(const Vector3& d);
    void moveZ(double dz);
    void scale(double sx, double sy, double sz);
    void scale(const Point3& s);
    void scale(const Vector3& s);
    void scale(double scalar);
    void setZ(double z);
    void swap(Point3& other);

    // Misc
    std::string toString() const override;
    bool operator==(const Point3& other) const;
    bool operator!=(const Point3& other) const;
    std::size_t hashCode() const override;

    friend std::ostream& operator<<(std::ostream& os, const Point3& point);
};

#endif // POINT3_HPP
