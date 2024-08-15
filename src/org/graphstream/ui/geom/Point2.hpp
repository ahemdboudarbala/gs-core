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

#ifndef POINT2_HPP
#define POINT2_HPP

#include <cmath>
#include <iostream>
#include <string>

class Point2 {
public:
    // Attributes
    double x;
    double y;

    // Shared Attributes
    static const Point2 NULL_POINT2;

    // Constructors
    Point2();
    Point2(double x, double y);
    Point2(const Point2& other);

    // Methods
    void set(double x, double y);
    void make(double x, double y);
    bool isZero() const;
    Point2 interpolate(const Point2& other, double factor) const;
    double distance(const Point2& other) const;
    void copy(const Point2& other);
    void moveTo(double x, double y);
    void move(double dx, double dy);
    void move(const Point2& p);
    void moveX(double dx);
    void moveY(double dy);
    void scale(double sx, double sy);
    void scale(const Point2& s);
    void setX(double x);
    void setY(double y);
    void swap(Point2& other);

    // Misc
    std::string toString() const;
    bool operator==(const Point2& other) const;
    bool operator!=(const Point2& other) const;
    std::size_t hashCode() const;

    friend std::ostream& operator<<(std::ostream& os, const Point2& point);
};

#endif // POINT2_HPP
