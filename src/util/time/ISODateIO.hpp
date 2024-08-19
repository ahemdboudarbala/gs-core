#ifndef ISODATEIO_HPP
#define ISODATEIO_HPP

#include <string>
#include <vector>
#include <regex>
#include <memory>
#include <stdexcept>
#include <ctime>

class ISODateComponent;

class ISODateIO {
public:
    ISODateIO();  // Default constructor with format "%K"
    explicit ISODateIO(const std::string& format);  // Constructor with custom format

    std::regex getPattern() const;

    void setFormat(const std::string& format);
    std::tm parse(const std::string& time) const;
    std::string toString(const std::tm& calendar) const;

private:
    std::vector<std::shared_ptr<ISODateComponent>> components;
    std::regex pattern;

    std::vector<std::shared_ptr<ISODateComponent>> findComponents(const std::string& format);
    void buildRegularExpression();
};

class ISODateComponent {
public:
    virtual ~ISODateComponent() = default;
    virtual std::string getDirective() const = 0;
    virtual std::string getReplacement() const = 0;
    virtual void set(const std::string& match, std::tm& calendar) const = 0;
    virtual std::string get(const std::tm& calendar) const = 0;
    virtual bool isAlias() const { return false; }
    virtual std::string getAliasReplacement() const { return ""; }
};

class TextComponent : public ISODateComponent {
public:
    explicit TextComponent(const std::string& text);
    std::string getDirective() const override;
    std::string getReplacement() const override;
    void set(const std::string& match, std::tm& calendar) const override;
    std::string get(const std::tm& calendar) const override;

private:
    std::string text;
};

#endif // ISODATEIO_HPP
