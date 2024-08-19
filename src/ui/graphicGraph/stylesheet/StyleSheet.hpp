#ifndef STYLESHEET_HPP
#define STYLESHEET_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Rule.hpp"
#include "StyleSheetListener.hpp"
#include "Selector.hpp"
#include "Element.hpp"

class StyleSheet {
public:
    StyleSheet();

    // Accessor methods
    std::shared_ptr<Rule> getDefaultGraphRule() const;
    std::shared_ptr<Rule> getDefaultNodeRule() const;
    std::shared_ptr<Rule> getDefaultEdgeRule() const;
    std::shared_ptr<Rule> getDefaultSpriteRule() const;

    std::shared_ptr<Style> getDefaultGraphStyle() const;
    std::shared_ptr<Style> getDefaultNodeStyle() const;
    std::shared_ptr<Style> getDefaultEdgeStyle() const;
    std::shared_ptr<Style> getDefaultSpriteStyle() const;

    std::vector<std::shared_ptr<Rule>> getRulesFor(const Element& element) const;
    std::string getStyleGroupIdFor(const Element& element, const std::vector<std::shared_ptr<Rule>>& rules) const;

    // Command methods
    void addListener(StyleSheetListener* listener);
    void removeListener(StyleSheetListener* listener);
    void clear();
    void parseFromFile(const std::string& fileName);
    void parseFromURL(const std::string& url);
    void parseFromString(const std::string& styleSheet);
    void load(const std::string& styleSheetValue);

    void addRule(const std::shared_ptr<Rule>& newRule);

    std::string toString() const;

private:
    void initRules();
    void parse(std::unique_ptr<std::istream> reader);
    
    // Nested classes
    class NameSpace {
    public:
        NameSpace(Selector::Type type);
        
        Selector::Type getGraphElementType() const;
        int getIdRulesCount() const;
        int getClassRulesCount() const;
        
        std::vector<std::shared_ptr<Rule>> getRulesFor(const Element& element) const;
        void clear();
        std::shared_ptr<Rule> addRule(const std::shared_ptr<Rule>& newRule);

        std::string toString(int level = -1) const;

    private:
        void getClassRules(const Element& element, std::vector<std::shared_ptr<Rule>>& rules) const;
        std::shared_ptr<Rule> addEventRule(const std::shared_ptr<Rule>& newRule);
        void toStringRules(int level, std::ostringstream& builder, const std::map<std::string, std::shared_ptr<Rule>>& rules, const std::string& title) const;

        Selector::Type type;
        std::shared_ptr<Rule> defaultRule;
        std::map<std::string, std::shared_ptr<Rule>> byId;
        std::map<std::string, std::shared_ptr<Rule>> byClass;
    };

    std::shared_ptr<Rule> defaultRule;
    NameSpace graphRules;
    NameSpace nodeRules;
    NameSpace edgeRules;
    NameSpace spriteRules;
    std::vector<StyleSheetListener*> listeners;
};

#endif // STYLESHEET_HPP
