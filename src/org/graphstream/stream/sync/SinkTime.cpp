/*
 * This file is part of GraphStream <http://graphstream-project.org>.
 * 
 * GraphStream is a library whose purpose is to handle static or dynamic
 * graphs, create them from scratch, file or any source and display them.
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

#include "SinkTime.hpp"
#include <iostream>

// Static members initialization
const std::string SinkTime::SYNC_DISABLE_KEY = "org.graphstream.stream.sync.disable";
const bool SinkTime::disableSync = []() {
    try {
        const char* val = std::getenv(SYNC_DISABLE_KEY.c_str());
        return val != nullptr;
    } catch (const std::exception&) {
        return false;
    }
}();

// Constructor
SinkTime::SinkTime() {}

// Public methods

bool SinkTime::isNewEvent(const std::string& sourceId, long timeId) {
    return disableSync || setTimeFor(sourceId, timeId);
}

// Protected methods

bool SinkTime::setTimeFor(const std::string& sourceId, long timeId) {
    auto it = times.find(sourceId);
    if (it == times.end()) {
        times[sourceId] = timeId;
        return true;
    } else if (timeId > it->second) {
        times[sourceId] = timeId;
        return true;
    }

    return false;
}
