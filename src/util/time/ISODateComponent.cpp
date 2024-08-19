#include "ISODateComponent.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>

ISODateComponent::ISODateComponent(const std::string& directive, const std::string& replace)
    : directive(directive), replace(replace) {}

std::string ISODateComponent::getDirective() const {
    return directive;
}

bool ISODateComponent::isAlias() const {
    return false;
}

std::string ISODateComponent::getReplacement() const {
    return replace;
}

// AliasComponent implementation
AliasComponent::AliasComponent(const std::string& shortcut, const std::string& replace)
    : ISODateComponent(shortcut, replace) {}

bool AliasComponent::isAlias() const {
    return true;
}

void AliasComponent::set(const std::string& value, std::tm& calendar) const {
    // Nothing to do
}

std::string AliasComponent::get(const std::tm& calendar) const {
    return "";
}

// TextComponent implementation
TextComponent::TextComponent(const std::string& value)
    : ISODateComponent("", std::regex_replace(value, std::regex("([\\^\\$\\.\\*\\+\\?\\(\\)\\[\\]\\{\\}\\|\\\\])"), "\\$1")),
      unquoted(value) {}

void TextComponent::set(const std::string& value, std::tm& calendar) const {
    // Nothing to do
}

std::string TextComponent::get(const std::tm& calendar) const {
    return unquoted;
}

// FieldComponent implementation
FieldComponent::FieldComponent(const std::string& shortcut, const std::string& replace, int field, const std::string& format)
    : ISODateComponent(shortcut, replace), field(field), offset(0), format(format) {}

FieldComponent::FieldComponent(const std::string& shortcut, const std::string& replace, int field, int offset, const std::string& format)
    : ISODateComponent(shortcut, replace), field(field), offset(offset), format(format) {}

void FieldComponent::set(const std::string& value, std::tm& calendar) const {
    int val = std::stoi(value);
    val += offset;

    switch (field) {
        case 'Y': calendar.tm_year = val - 1900; break;
        case 'm': calendar.tm_mon = val - 1; break;
        case 'd': calendar.tm_mday = val; break;
        case 'H': calendar.tm_hour = val; break;
        case 'M': calendar.tm_min = val; break;
        case 'S': calendar.tm_sec = val; break;
        // Handle more fields as needed
        default: throw std::runtime_error("Unsupported field component");
    }
}

std::string FieldComponent::get(const std::tm& calendar) const {
    std::stringstream ss;
    ss << std::put_time(&calendar, format.c_str());
    return ss.str();
}

// AMPMComponent implementation
AMPMComponent::AMPMComponent()
    : ISODateComponent("%p", "AM|PM|am|pm"), locale(""), am("AM"), pm("PM") {}

void AMPMComponent::set(const std::string& value, std::tm& calendar) const {
    if (value == am) {
        calendar.tm_hour %= 12;
    } else if (value == pm) {
        calendar.tm_hour = (calendar.tm_hour % 12) + 12;
    }
}

std::string AMPMComponent::get(const std::tm& calendar) const {
    return calendar.tm_hour < 12 ? am : pm;
}

// UTCOffsetComponent implementation
UTCOffsetComponent::UTCOffsetComponent()
    : ISODateComponent("%z", "(?:[-+]\\d{4}|Z)") {}

void UTCOffsetComponent::set(const std::string& value, std::tm& calendar) const {
    if (value == "Z") {
        // Set UTC offset to 0
    } else {
        // Parse offset and set the timezone
    }
}

std::string UTCOffsetComponent::get(const std::tm& calendar) const {
    int offset = 0; // Assume we get offset from the timezone
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << (offset / 3600) << std::setw(2) << (offset % 3600) / 60;
    return ss.str();
}

// EpochComponent implementation
EpochComponent::EpochComponent()
    : ISODateComponent("%K", "\\d+") {}

void EpochComponent::set(const std::string& value, std::tm& calendar) const {
    time_t epoch = std::stol(value);
    calendar = *std::gmtime(&epoch);
}

std::string EpochComponent::get(const std::tm& calendar) const {
    time_t epoch = std::mktime(const_cast<std::tm*>(&calendar));
    return std::to_string(epoch);
}

// NotImplementedComponent implementation
NotImplementedComponent::NotImplementedComponent(const std::string& shortcut, const std::string& replace)
    : ISODateComponent(shortcut, replace) {}

void NotImplementedComponent::set(const std::string& value, std::tm& calendar) const {
    throw std::runtime_error("Not implemented component");
}

std::string NotImplementedComponent::get(const std::tm& calendar) const {
    throw std::runtime_error("Not implemented component");
}

// Define all the components
const ISODateComponent* ABBREVIATED_WEEKDAY_NAME = new NotImplementedComponent("%a", "\\w+[.]");
const ISODateComponent* FULL_WEEKDAY_NAME = new NotImplementedComponent("%A", "\\w+");
const ISODateComponent* ABBREVIATED_MONTH_NAME = new NotImplementedComponent("%b", "\\w+[.]");
const ISODateComponent* FULL_MONTH_NAME = new NotImplementedComponent("%B", "\\w+");
const ISODateComponent* LOCALE_DATE_AND_TIME = new NotImplementedComponent("%c", "");
// ...
