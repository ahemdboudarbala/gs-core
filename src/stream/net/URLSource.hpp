/*
 * This file is part of GraphStream <http://graphstream-project.org>.
 * 
 * GraphStream is a library whose purpose is to handle static or dynamic
 * graph, create them from scratch, file or any source and display them.
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
 * PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-C and LGPL licenses and that you accept their terms.
 */

/**
 * @since 2009-04-17
 * 
 * @author Yoann Pigné <yoann.pigne@graphstream-project.org>
 * @author Antoine Duot <antoine.dutot@graphstream-project.org>
 * @author Guilhelm Savin <guilhelm.savin@graphstream-project.org>
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 */

#ifndef URLSOURCE_HPP
#define URLSOURCE_HPP

#include "Source.hpp"
#include <string>
#include <stdexcept>

/**
 * Graph event input source from a URL.
 */
class URLSource : public Source {
public:
    /**
     * Read the whole URL in one big non-interruptible operation.
     * 
     * @param url The URL to fetch.
     * @throws std::runtime_error If an I/O error occurs while fetching the URL.
     */
    virtual void fetchAll(const std::string& url) = 0;

    /**
     * Begin fetching the URL stopping as soon as possible. Next graph events from
     * the URL will be sent by calling nextEvents(). Once begin() has been
     * called, you must finish the reading process using end(). You cannot
     * call begin() twice without having called end() in between.
     * 
     * @param url The URL to fetch.
     * @throws std::runtime_error If an I/O error occurs while reading.
     */
    virtual void begin(const std::string& url) = 0;

    /**
     * Try to process one graph event, or as few as possible, if more must be read
     * at once. For this method to work, you must have called begin().
     * This method returns true while there are still events to read.
     * 
     * @return true if there are still events to read, false as soon as the file is
     *         finished.
     * @throws std::runtime_error If an I/O error occurs while reading.
     */
    virtual bool nextEvents() = 0;

    /**
     * Finish the reading process (even if nextEvents() did not return
     * false). You must call this method after reading.
     * 
     * @throws std::runtime_error If an I/O error occurs while closing the file.
     */
    virtual void end() = 0;

    virtual ~URLSource() = default;
};

#endif // URLSOURCE_HPP
