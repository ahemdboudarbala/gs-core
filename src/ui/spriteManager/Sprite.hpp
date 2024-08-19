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
 * @author Stefan Balev <stefan.balev@graphstream-project.org>
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 */

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "graph/Element.hpp"
#include "SpriteManager.hpp"
#include "ui/graphicGraph/stylesheet/Style.hpp"
#include "ui/graphicGraph/stylesheet/Values.hpp"
#include <string>
#include <map>
#include <vector>
#include <memory>

class Sprite : public Element {
public:
    Sprite();
    Sprite(const std::string& id, SpriteManager* manager);
    Sprite(const std::string& id, SpriteManager* manager, const Values& position);
    void init(const std::string& id, SpriteManager* manager, const Values& position);
    void removed();

    Element* getAttachment() const;
    bool attached() const;

    double getX() const;
    double getY() const;
    double getZ() const;
    Style::Units getUnits() const;

    void attachToNode(const std::string& id);
    void attachToEdge(const std::string& id);
    void detach();

    void setPosition(double percent);
    void setPosition(double x, double y, double z);
    void setPosition(Style::Units units, double x, double y, double z);

    std::string getId() const override;
    std::string getLabel(const std::string& key) const override;
    std::any getAttribute(const std::string& key) const override;
    void setAttribute(const std::string& attribute, const std::vector<std::any>& values) override;
    void removeAttribute(const std::string& attribute) override;

private:
    std::string id;
    std::string completeId;
    SpriteManager* manager;
    Values position;
    Element* attachment;

    void setPosition(const Values& values);
};

#endif // SPRITE_HPP
