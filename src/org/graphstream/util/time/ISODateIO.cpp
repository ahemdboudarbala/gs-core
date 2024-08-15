#include "ISODateIO.hpp"
#include <sstream>
#include <stdexcept>

ISODateIO::ISODateIO() {
    setFormat("%K");
}

ISODateIO::ISODateIO(const std::string& format) {
    setFormat(format);
}

std::regex ISODateIO::getPattern() const {
    return pattern;
}

void ISODateIO::setFormat(const std::string& format) {
    components = findComponents(format);
    buildRegularExpression();
}

std::tm ISODateIO::parse(const std::string& time) const {
    std::tm cal = {};
    std::smatch match;

    if (std::regex_match(time, match, pattern)) {
        for (size_t i = 0; i < components.size(); ++i) {
            components[i]->set(match[i + 1].str(), cal);
        }
    } else {
        throw std::runtime_error("Time string does not match the format");
    }

    return cal;
}

std::string ISODateIO::toString(const std::tm& calendar) const {
    std::stringstream buffer;

    for (const auto& component : components) {
        buffer << component->get(calendar);
    }

    return buffer.str();
}

std::vector<std::shared_ptr<ISODateComponent>> ISODateIO::findComponents(const std::string& format) {
    std::vector<std::shared_ptr<ISODateComponent>> result;
    size_t offset = 0;

    while (offset < format.length()) {
        if (format[offset] == '%') {
            bool found = false;

            for (const auto& component : KNOWN_COMPONENTS) { // Assume KNOWN_COMPONENTS is defined globally
                if (format.compare(offset, component->getDirective().length(), component->getDirective()) == 0) {
                    found = true;
                    if (component->isAlias()) {
                        auto subComponents = findComponents(component->getAliasReplacement());
                        result.insert(result.end(), subComponents.begin(), subComponents.end());
                    } else {
                        result.push_back(component);
                    }
                    offset += component->getDirective().length();
                    break;
                }
            }

            if (!found) {
                throw std::runtime_error("Unknown identifier in format");
            }
        } else {
            size_t from = offset;
            while (offset < format.length() && format[offset] != '%') {
                offset++;
            }
            result.push_back(std::make_shared<TextComponent>(format.substr(from, offset - from)));
        }
    }

    return result;
}

void ISODateIO::buildRegularExpression() {
    std::stringstream patternStream;

    for (const auto& component : components) {
        patternStream << "(" << component->getReplacement() << ")";
    }

    pattern = std::regex(patternStream.str());
}

// TextComponent implementation
TextComponent::TextComponent(const std::string& text) : text(text) {}

std::string TextComponent::getDirective() const {
    return text;
}

std::string TextComponent::getReplacement() const {
    return text;
}

void TextComponent::set(const std::string& match, std::tm& calendar) const {
    // Do nothing, as this is just text
}

std::string TextComponent::get(const std::tm& calendar) const {
    return text;
}
