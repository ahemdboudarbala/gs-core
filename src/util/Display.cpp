#include "Display.hpp"
#include "Graph.hpp"
#include "Viewer.hpp"
#include <iostream>
#include <cstdlib>

MissingDisplayException::MissingDisplayException(const std::string& message) : message(message) {}

const char* MissingDisplayException::what() const noexcept {
    return message.c_str();
}

std::shared_ptr<Display> Display::getDefault() {
    const char* uiModule = std::getenv("GRAPHSTREAM_UI");

    if (!uiModule) {
        throw MissingDisplayException("No UI package detected! Please set the GRAPHSTREAM_UI environment variable.");
    } else {
        std::shared_ptr<Display> display = nullptr;
        std::string candidates[] = { uiModule, std::string(uiModule) + ".Display",
                                     "org.graphstream.ui." + std::string(uiModule) + ".Display" };

        for (const auto& candidate : candidates) {
            try {
                // In C++, you would typically use a factory pattern or dynamic loading
                // but for this example, we'll simulate loading with a placeholder
                if (candidate == candidates[0]) {
                    // Placeholder: Simulate loading a Display class (must be implemented)
                    display = std::make_shared<Display>(); // Replace with actual class
                    break;
                }
            } catch (const std::exception& e) {
                continue;
            }
        }

        if (!display) {
            throw MissingDisplayException("No valid display found. Please check your GRAPHSTREAM_UI environment variable.");
        } else {
            return display;
        }
    }
}

