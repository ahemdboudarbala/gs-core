#include "StyleSheet.hpp"
#include "StyleSheetParser.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>

StyleSheet::StyleSheet() 
    : graphRules(Selector::Type::GRAPH), 
      nodeRules(Selector::Type::NODE), 
      edgeRules(Selector::Type::EDGE), 
      spriteRules(Selector::Type::SPRITE) {
    initRules();
}

std::shared_ptr<Rule> StyleSheet::getDefaultGraphRule() const {
    return graphRules.defaultRule;
}

std::shared_ptr<Rule> StyleSheet::getDefaultNodeRule() const {
    return nodeRules.defaultRule;
}

std::shared_ptr<Rule> StyleSheet::getDefaultEdgeRule() const {
    return edgeRules.defaultRule;
}

std::shared_ptr<Rule> StyleSheet::getDefaultSpriteRule() const {
    return spriteRules.defaultRule;
}

std::shared_ptr<Style> StyleSheet::getDefaultGraphStyle() const {
    return getDefaultGraphRule()->getStyle();
}

std::shared_ptr<Style> StyleSheet::getDefaultNodeStyle() const {
    return getDefaultNodeRule()->getStyle();
}

std::shared_ptr<Style> StyleSheet::getDefaultEdgeStyle() const {
    return getDefaultEdgeRule()->getStyle();
}

std::shared_ptr<Style> StyleSheet::getDefaultSpriteStyle() const {
    return getDefaultSpriteRule()->getStyle();
}

std::vector<std::shared_ptr<Rule>> StyleSheet::getRulesFor(const Element& element) const {
    if (dynamic_cast<const Graph*>(&element)) {
        return graphRules.getRulesFor(element);
    } else if (dynamic_cast<const Node*>(&element)) {
        return nodeRules.getRulesFor(element);
    } else if (dynamic_cast<const Edge*>(&element)) {
        return edgeRules.getRulesFor(element);
    } else if (dynamic_cast<const GraphicSprite*>(&element)) {
        return spriteRules.getRulesFor(element);
    } else {
        return { defaultRule };
    }
}

std::string StyleSheet::getStyleGroupIdFor(const Element& element, const std::vector<std::shared_ptr<Rule>>& rules) const {
    std::ostringstream builder;

    if (dynamic_cast<const Graph*>(&element)) {
        builder << "g";
    } else if (dynamic_cast<const Node*>(&element)) {
        builder << "n";
    } else if (dynamic_cast<const Edge*>(&element)) {
        builder << "e";
    } else if (dynamic_cast<const GraphicSprite*>(&element)) {
        builder << "s";
    } else {
        throw std::runtime_error("Unknown element type");
    }

    if (!rules[0]->selector.getId().empty()) {
        builder << '_' << rules[0]->selector.getId();
    }

    if (rules.size() > 1) {
        builder << '(' << rules[1]->selector.getClazz();
        for (size_t i = 2; i < rules.size(); ++i) {
            builder << ',' << rules[i]->selector.getClazz();
        }
        builder << ')';
    }

    return builder.str();
}

void StyleSheet::addListener(StyleSheetListener* listener) {
    listeners.push_back(listener);
}

void StyleSheet::removeListener(StyleSheetListener* listener) {
    auto it = std::find(listeners.begin(), listeners.end(), listener);
    if (it != listeners.end()) {
        listeners.erase(it);
    }
}

void StyleSheet::clear() {
    graphRules.clear();
    nodeRules.clear();
    edgeRules.clear();
    spriteRules.clear();
    initRules();

    for (auto& listener : listeners) {
        listener->styleSheetCleared();
    }
}

void StyleSheet::parseFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }
    parse(std::make_unique<std::istream>(&file));
}

void StyleSheet::parseFromURL(const std::string& url) {
    // URL handling logic (similar to Java implementation)
    throw std::runtime_error("URL parsing not implemented");
}

void StyleSheet::parseFromString(const std::string& styleSheet) {
    std::istringstream stream(styleSheet);
    parse(std::make_unique<std::istream>(&stream));
}

void StyleSheet::load(const std::string& styleSheetValue) {
    if (styleSheetValue.find("url") == 0) {
        // Extract and parse from URL
        throw std::runtime_error("URL loading not implemented");
    } else {
        parseFromString(styleSheetValue);
    }
}

void StyleSheet::addRule(const std::shared_ptr<Rule>& newRule) {
    std::shared_ptr<Rule> oldRule = nullptr;

    switch (newRule->selector.getType()) {
        case Selector::Type::GRAPH:
            oldRule = graphRules.addRule(newRule);
            break;
        case Selector::Type::NODE:
            oldRule = nodeRules.addRule(newRule);
            break;
        case Selector::Type::EDGE:
            oldRule = edgeRules.addRule(newRule);
            break;
        case Selector::Type::SPRITE:
            oldRule = spriteRules.addRule(newRule);
            break;
        default:
            throw std::runtime_error("Unexpected selector type");
    }

    for (auto& listener : listeners) {
        listener->styleAdded(oldRule, newRule);
    }
}

std::string StyleSheet::toString() const {
    std::ostringstream builder;

    builder << "StyleSheet : {\n";
    builder << "  default styles:\n";
    builder << defaultRule->toString(1);
    builder << graphRules.toString(1);
    builder << nodeRules.toString(1);
    builder << edgeRules.toString(1);
    builder << spriteRules.toString(1);

    return builder.str();
}

void StyleSheet::initRules() {
    defaultRule = std::make_shared<Rule>(Selector(Selector::Type::ANY), nullptr);
    defaultRule->getStyle()->setDefaults();

    graphRules.defaultRule = std::make_shared<Rule>(Selector(Selector::Type::GRAPH), defaultRule);
    nodeRules.defaultRule = std::make_shared<Rule>(Selector(Selector::Type::NODE), defaultRule);
    edgeRules.defaultRule = std::make_shared<Rule>(Selector(Selector::Type::EDGE), defaultRule);
    spriteRules.defaultRule = std::make_shared<Rule>(Selector(Selector::Type::SPRITE), defaultRule);

    // Further rule initialization here...

    for (auto& listener : listeners) {
        listener->styleAdded(defaultRule, defaultRule);
        listener->styleAdded(graphRules.defaultRule, graphRules.defaultRule);
        listener->styleAdded(nodeRules.defaultRule, nodeRules.defaultRule);
        listener->styleAdded(edgeRules.defaultRule, edgeRules.defaultRule);
        listener->styleAdded(spriteRules.defaultRule, spriteRules.defaultRule);
    }
}

void StyleSheet::parse(std::unique_ptr<std::istream> reader) {
    StyleSheetParser parser(this, std::move(reader));
    parser.start();
}

// NameSpace methods implementation

StyleSheet::NameSpace::NameSpace(Selector::Type type) 
    : type(type) {}

Selector::Type StyleSheet::NameSpace::getGraphElementType() const {
    return type;
}

int StyleSheet::NameSpace::getIdRulesCount() const {
    return byId.size();
}

int StyleSheet::NameSpace::getClassRulesCount() const {
    return byClass.size();
}

std::vector<std::shared_ptr<Rule>> StyleSheet::NameSpace::getRulesFor(const Element& element) const {
    auto rule = byId.find(element.getId());
    std::vector<std::shared_ptr<Rule>> rules;

    if (rule != byId.end()) {
        rules.push_back(rule->second);
    } else {
        rules.push_back(defaultRule);
    }

    getClassRules(element, rules);

    if (rules.empty()) {
        rules.push_back(defaultRule);
    }

    return rules;
}

void StyleSheet::NameSpace::getClassRules(const Element& element, std::vector<std::shared_ptr<Rule>>& rules) const {
    auto classes = element.getAttribute("ui.class");

    if (!classes.empty()) {
        std::istringstream ss(classes);
        std::string token;
        while (std::getline(ss, token, ',')) {
            auto rule = byClass.find(token);
            if (rule != byClass.end()) {
                rules.push_back(rule->second);
            }
        }
    }
}

std::shared_ptr<Rule> StyleSheet::NameSpace::addRule(const std::shared_ptr<Rule>& newRule) {
    std::shared_ptr<Rule> oldRule = nullptr;

    if (newRule->selector.getPseudoClass() != nullptr) {
        oldRule = addEventRule(newRule);
    } else if (!newRule->selector.getId().empty()) {
        oldRule = byId[newRule->selector.getId()];

        if (oldRule != nullptr) {
            oldRule->getStyle()->augment(newRule->getStyle());
        } else {
            byId[newRule->selector.getId()] = newRule;
            newRule->getStyle()->reparent(defaultRule);
        }
    } else if (!newRule->selector.getClazz().empty()) {
        oldRule = byClass[newRule->selector.getClazz()];

        if (oldRule != nullptr) {
            oldRule->getStyle()->augment(newRule->getStyle());
        } else {
            byClass[newRule->selector.getClazz()] = newRule;
            newRule->getStyle()->reparent(defaultRule);
        }
    } else {
        oldRule = defaultRule;
        defaultRule->getStyle()->augment(newRule->getStyle());
        newRule = defaultRule;
    }

    return oldRule;
}

std::shared_ptr<Rule> StyleSheet::NameSpace::addEventRule(const std::shared_ptr<Rule>& newRule) {
    std::shared_ptr<Rule> parentRule = nullptr;

    if (!newRule->selector.getId().empty()) {
        parentRule = byId[newRule->selector.getId()];

        if (parentRule == nullptr) {
            parentRule = addRule(std::make_shared<Rule>(Selector(newRule->selector.getType(), newRule->selector.getId(), newRule->selector.getClazz())));
        }
    } else if (!newRule->selector.getClazz().empty()) {
        parentRule = byClass[newRule->selector.getClazz()];

        if (parentRule == nullptr) {
            parentRule = addRule(std::make_shared<Rule>(Selector(newRule->selector.getType(), newRule->selector.getId(), newRule->selector.getClazz())));
        }
    } else {
        parentRule = defaultRule;
    }

    newRule->getStyle()->reparent(parentRule);
    parentRule->getStyle()->addAlternateStyle(newRule->selector.getPseudoClass(), newRule);

    return parentRule;
}

void StyleSheet::NameSpace::clear() {
    defaultRule.reset();
    byId.clear();
    byClass.clear();
}

std::string StyleSheet::NameSpace::toString(int level) const {
    std::ostringstream builder;

    std::string prefix(level > 0 ? std::string(level * 4, ' ') : "");

    builder << prefix << type << " default style:\n";
    builder << defaultRule->toString(level + 1);
    toStringRules(level, builder, byId, prefix + type + " id styles");
    toStringRules(level, builder, byClass, prefix + type + " class styles");

    return builder.str();
}

void StyleSheet::NameSpace::toStringRules(int level, std::ostringstream& builder, const std::map<std::string, std::shared_ptr<Rule>>& rules, const std::string& title) const {
    builder << title << ":\n";
    for (const auto& rule : rules) {
        builder << rule.second->toString(level + 1);
    }
}
