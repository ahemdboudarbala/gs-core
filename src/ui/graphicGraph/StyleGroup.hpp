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

#ifndef STYLE_GROUP_HPP
#define STYLE_GROUP_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include "Element.hpp"
#include "Style.hpp"
#include "Rule.hpp"
#include "StyleGroupSet.hpp"

class StyleGroup : public Style, public std::enable_shared_from_this<StyleGroup> {
public:
    class ElementEvents;
    class BulkElements;
    class BulkIterator;

    // Constructor
    StyleGroup(const std::string& identifier, const std::vector<Rule>& rules, 
               std::shared_ptr<Element> firstElement, std::shared_ptr<StyleGroupSet::EventSet> eventSet);

    // Access methods
    std::string getId() const;
    Selector::Type getType() const;
    bool hasDynamicElements() const;
    bool hasEventElements() const;
    bool elementHasEvents(const std::shared_ptr<Element>& element) const;
    bool elementIsDynamic(const std::shared_ptr<Element>& element) const;
    std::shared_ptr<Element> getElement(const std::string& id) const;
    int getElementCount() const;
    bool isEmpty() const;
    bool contains(const std::string& elementId) const;
    bool contains(const std::shared_ptr<Element>& element) const;
    std::shared_ptr<SwingElementRenderer> getRenderer(const std::string& id) const;
    std::shared_ptr<ElementEvents> getEventsFor(const std::shared_ptr<Element>& element) const;

    // Iterators and iterable sets
    std::shared_ptr<StyleGroup::BulkElements> bulkElements();
    std::shared_ptr<BulkIterator> bulkIterator();
    std::vector<std::shared_ptr<Element>> elements() const;
    std::vector<std::shared_ptr<ElementEvents>> elementsEvents() const;
    std::vector<std::shared_ptr<Element>> dynamicElements() const;

    // Command methods
    void addElement(const std::shared_ptr<Element>& element);
    std::shared_ptr<Element> removeElement(const std::shared_ptr<Element>& element);
    void pushEventFor(const std::shared_ptr<Element>& element, const std::string& event);
    void popEventFor(const std::shared_ptr<Element>& element, const std::string& event);
    void activateEventsFor(const std::shared_ptr<Element>& element);
    void deactivateEvents();
    void pushElementAsDynamic(const std::shared_ptr<Element>& element);
    void popElementAsDynamic(const std::shared_ptr<Element>& element);
    void release();
    void setValue(const std::string& property, const std::any& value) override;

    // Renderers management
    void addRenderer(const std::string& id, const std::shared_ptr<SwingElementRenderer>& renderer);
    std::shared_ptr<SwingElementRenderer> removeRenderer(const std::string& id);

    // ToString method
    std::string toString() const override;
    std::string toString(int level) const override;

protected:
    std::string id;
    std::vector<Rule> rules;
    std::unordered_map<std::string, std::shared_ptr<Element>> elements;
    std::shared_ptr<StyleGroupSet::EventSet> eventSet;
    std::unordered_map<std::shared_ptr<Element>, std::shared_ptr<ElementEvents>> eventsFor;
    std::unordered_set<std::shared_ptr<Element>> dynamicOnes;
    std::vector<std::string> curEvents;
    std::shared_ptr<BulkElements> bulkElements;
    std::unordered_map<std::string, std::shared_ptr<SwingElementRenderer>> renderers;

public:
    class ElementEvents {
    public:
        ElementEvents(const std::shared_ptr<Element>& element, const std::shared_ptr<StyleGroup>& group, const std::string& event);
        std::shared_ptr<Element> getElement() const;
        int eventCount() const;
        std::vector<std::string> events() const;
        void activate();
        void deactivate();
        void pushEvent(const std::string& event);
        void popEvent(const std::string& event);

    private:
        std::vector<std::string> events;
        std::shared_ptr<Element> element;
        std::shared_ptr<StyleGroup> group;
    };

    class BulkElements : public std::enable_shared_from_this<BulkElements> {
    public:
        std::shared_ptr<BulkIterator> iterator();
    };

    class BulkIterator : public std::iterator<std::input_iterator_tag, std::shared_ptr<Element>> {
    public:
        BulkIterator(const std::vector<std::shared_ptr<Element>>& elements);
        bool hasNext();
        std::shared_ptr<Element> next();

    private:
        std::vector<std::shared_ptr<Element>>::iterator iterator;
        std::shared_ptr<Element> nextElement;
    };
};

#endif // STYLE_GROUP_HPP
