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

#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "ui/geom/Point3.hpp"

class Backend {
public:
    virtual Point3 transform(double x, double y, double z) = 0;
    virtual Point3 inverseTransform(double x, double y, double z) = 0;
    virtual Point3 transform(Point3& p) = 0;
    virtual Point3 inverseTransform(Point3& p) = 0;
    virtual void pushTransform() = 0;
    virtual void beginTransform() = 0;
    virtual void setIdentity() = 0;
    virtual void translate(double tx, double ty, double tz) = 0;
    virtual void rotate(double angle, double ax, double ay, double az) = 0;
    virtual void scale(double sx, double sy, double sz) = 0;
    virtual void endTransform() = 0;
    virtual void popTransform() = 0;
    virtual void setAntialias(bool on) = 0;
    virtual void setQuality(bool on) = 0;

    virtual ~Backend() = default;
};

#endif // BACKEND_HPP
