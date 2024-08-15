/*
 * This file is part of GraphStream <http://graphstream-project.org>.
 * 
 * GraphStream is a library whose purpose is to handle static or dynamic
 * graphs, create them from scratch, file, or any source, and display them.
 * 
 * This program is free software distributed under the terms of two licenses, the
 * CeCILL-C license that fits European law, and the GNU Lesser General Public
 * License. You can use, modify and/or redistribute the software under the terms
 * of the CeCILL-C license as circulated by CEA, CNRS, and INRIA at the following
 * URL <http://www.cecill.info> or under the terms of the GNU LGPL as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-C and LGPL licenses and that you accept their terms.
 */

/**
 * @since 2018-01-18
 * 
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 * @author Yoann Pigné <yoann.pigne@graphstream-project.org>
 */

#ifndef CONNECTORSKELETON_HPP
#define CONNECTORSKELETON_HPP

#include "Point3.hpp"
#include <string>

class ConnectorSkeleton {
public:
    virtual std::string kindString() = 0;
    virtual bool isPoly() = 0;
    virtual bool isCurve() = 0;
    virtual int multi() = 0;
    virtual bool isLoop() = 0;
    virtual void setPoly(void* aSetOfPoints) = 0;
    virtual void setPoly(Point3* aSetOfPoints) = 0;
    virtual void setCurve(double x0, double y0, double z0, double x1, double y1, double z1, 
                          double x2, double y2, double z2, double x3, double y3, double z3) = 0;
    virtual void setLine(double x0, double y0, double z0, double x1, double y1, double z1) = 0;
    virtual void setMulti(int aMulti) = 0;
    virtual bool isMulti() = 0;
    virtual void setLoop(double x0, double y0, double z0, double x1, double y1, double z1, 
                         double x2, double y2, double z2) = 0;
    virtual int size() = 0;
    virtual Point3 apply(int i) = 0;
    virtual void update(int i, const Point3& p) = 0;
    virtual Point3 to() = 0;
    virtual Point3 from() = 0;
    virtual double length() = 0;
    virtual double* segmentsLengths() = 0;
    virtual double segmentLength(int i) = 0;
    virtual Point3 pointOnShape(double percent) = 0;
    virtual Point3 pointOnShape(double percent, Point3& target) = 0;
    virtual Point3 pointOnShapeAndPerpendicular(double percent, double perpendicular) = 0;
    virtual Point3 pointOnShapeAndPerpendicular(double percent, double perpendicular, Point3& target) = 0;
    
    virtual ~ConnectorSkeleton() = default;
};

#endif // CONNECTORSKELETON_HPP
