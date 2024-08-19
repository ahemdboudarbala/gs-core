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

#ifndef GRAPHIC_ELEMENT_CHANGE_LISTENER_HPP
#define GRAPHIC_ELEMENT_CHANGE_LISTENER_HPP

#include "GraphicElement.hpp"

/**
 * Interface for entities interested in changes on graphic elements.
 * 
 * This interface's main role is to notify renderers that an element changed and
 * therefore must be redrawn. This helps avoid redrawing elements that did not change.
 */
class GraphicElementChangeListener {
public:
    virtual ~GraphicElementChangeListener() = default;

    /**
     * Method to be called when a graphic element changes.
     * 
     * @param element The graphic element that has changed.
     */
    virtual void graphicElementChanged(std::shared_ptr<GraphicElement> element) = 0;
};

#endif // GRAPHIC_ELEMENT_CHANGE_LISTENER_HPP
