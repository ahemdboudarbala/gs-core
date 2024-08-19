/*
 * This file is part of GraphStream <http://graphstream-project.org>.
 * 
 * GraphStream is a library whose purpose is to handle static or dynamic
 * graphs, create them from scratch, files, or any source, and display them.
 * 
 * This program is free software distributed under the terms of two licenses: the
 * CeCILL-C license that fits European law, and the GNU Lesser General Public
 * License. You can use, modify, and/or redistribute the software under the terms
 * of the CeCILL-C license as circulated by CEA, CNRS, and INRIA at the following
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

#ifndef STYLE_GROUP_LISTENER_HPP
#define STYLE_GROUP_LISTENER_HPP

#include "Element.hpp"
#include "StyleGroup.hpp"

class StyleGroupListener {
public:
    virtual ~StyleGroupListener() = default;

    /**
     * The style of the element changed.
     * 
     * @param element The element.
     * @param oldStyle The old style.
     * @param style The changed style or the new style of the element.
     */
    virtual void elementStyleChanged(Element* element, StyleGroup* oldStyle, StyleGroup* style) = 0;
};

#endif // STYLE_GROUP_LISTENER_HPP
