#ifndef ISODATECOMPONENT_HPP
#define ISODATECOMPONENT_HPP

#include <string>
#include <regex>
#include <ctime>
#include <locale>

class ISODateComponent {
public:
    ISODateComponent(const std::string& directive, const std::string& replace);
    virtual ~ISODateComponent() = default;

    std::string getDirective() const;
    virtual bool isAlias() const;
    std::string getReplacement() const;
    virtual void set(const std::string& value, std::tm& calendar) const = 0;
    virtual std::string get(const std::tm& calendar) const = 0;

protected:
    std::string directive;
    std::string replace;
};

class AliasComponent : public ISODateComponent {
public:
    AliasComponent(const std::string& shortcut, const std::string& replace);
    bool isAlias() const override;
    void set(const std::string& value, std::tm& calendar) const override;
    std::string get(const std::tm& calendar) const override;
};

class TextComponent : public ISODateComponent {
public:
    explicit TextComponent(const std::string& value);
    void set(const std::string& value, std::tm& calendar) const override;
    std::string get(const std::tm& calendar) const override;

private:
    std::string unquoted;
};

class FieldComponent : public ISODateComponent {
public:
    FieldComponent(const std::string& shortcut, const std::string& replace, int field, const std::string& format);
    FieldComponent(const std::string& shortcut, const std::string& replace, int field, int offset, const std::string& format);
    void set(const std::string& value, std::tm& calendar) const override;
    std::string get(const std::tm& calendar) const override;

protected:
    int field;
    int offset;
    std::string format;
};

class AMPMComponent : public ISODateComponent {
public:
    AMPMComponent();
    void set(const std::string& value, std::tm& calendar) const override;
    std::string get(const std::tm& calendar) const override;

private:
    std::locale locale;
    std::string am;
    std::string pm;
};

class UTCOffsetComponent : public ISODateComponent {
public:
    UTCOffsetComponent();
    void set(const std::string& value, std::tm& calendar) const override;
    std::string get(const std::tm& calendar) const override;
};

class EpochComponent : public ISODateComponent {
public:
    EpochComponent();
    void set(const std::string& value, std::tm& calendar) const override;
    std::string get(const std::tm& calendar) const override;
};

class NotImplementedComponent : public ISODateComponent {
public:
    NotImplementedComponent(const std::string& shortcut, const std::string& replace);
    void set(const std::string& value, std::tm& calendar) const override;
    std::string get(const std::tm& calendar) const override;
};

// Define all the components as extern global variables
extern const ISODateComponent* ABBREVIATED_WEEKDAY_NAME;
extern const ISODateComponent* FULL_WEEKDAY_NAME;
extern const ISODateComponent* ABBREVIATED_MONTH_NAME;
extern const ISODateComponent* FULL_MONTH_NAME;
extern const ISODateComponent* LOCALE_DATE_AND_TIME;
// ... continue with all components

#endif // ISODATECOMPONENT_HPP
