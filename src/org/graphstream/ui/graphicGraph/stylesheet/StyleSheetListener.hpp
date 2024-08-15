#ifndef STYLESHEETLISTENER_HPP
#define STYLESHEETLISTENER_HPP

class Rule;

class StyleSheetListener {
public:
    virtual ~StyleSheetListener() = default;

    /**
     * A style was changed or added.
     * 
     * @param oldRule The style that changed.
     * @param newRule The style that was added to the style sheet.
     */
    virtual void styleAdded(Rule* oldRule, Rule* newRule) = 0;

    /**
     * The complete style sheet was cleared.
     */
    virtual void styleSheetCleared() = 0;
};

#endif // STYLESHEETLISTENER_HPP
