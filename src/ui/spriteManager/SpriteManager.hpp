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
 * @since 2009-04-17
 *
 * @author Antoine Dutot <antoine.dutot@graphstream-project.org>
 * @author Guilhelm Savin <guilhelm.savin@graphstream-project.org>
 * @author Richard O. Legendi <richard.legendi@gmail.com>
 * @author Alex Bowen <bowen.a@gmail.com>
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 */

#ifndef SPRITEMANAGER_HPP
#define SPRITEMANAGER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <iterator>
#include <memory>
#include <stdexcept>
#include "Sprite.hpp"
#include "graph/Graph.hpp"
#include "SpriteFactory.hpp"
#include "ui/graphicGraph/stylesheet/Values.hpp"

class SpriteManager {
public:
    SpriteManager(Graph* graph);
    ~SpriteManager();

    int getSpriteCount() const;
    bool hasSprite(const std::string& identifier) const;
    Sprite* getSprite(const std::string& identifier) const;
    std::vector<Sprite*> sprites() const;

    void detach();
    void setSpriteFactory(std::unique_ptr<SpriteFactory> factory);
    void resetSpriteFactory();

    Sprite* addSprite(const std::string& identifier);
    Sprite* addSprite(const std::string& identifier, const Values* position);
    
    template <typename T>
    T* addSprite(const std::string& identifier, const Values* position = nullptr);

    void removeSprite(const std::string& identifier);

private:
    void lookForExistingSprites();
    Values* getPositionValue(void* value);

    Graph* graph_;
    std::unordered_map<std::string, std::unique_ptr<Sprite>> sprites_;
    std::unique_ptr<SpriteFactory> factory_;
    bool attributeLock_;
};

#endif // SPRITEMANAGER_HPP
