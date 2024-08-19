#include "Layouts.hpp"

std::unique_ptr<Layout> Layouts::newLayoutAlgorithm() {
    std::string layoutClassName;

    try {
        const char* envLayout = std::getenv("GS_UI_LAYOUT");
        if (envLayout != nullptr) {
            layoutClassName = std::string(envLayout);
            std::cerr << "\"GS_UI_LAYOUT\" is deprecated, use \"ORG_GRAPHSTREAM_UI_LAYOUT\" instead." << std::endl;
        } else {
            const char* envLayoutOrg = std::getenv("ORG_GRAPHSTREAM_UI_LAYOUT");
            if (envLayoutOrg != nullptr) {
                layoutClassName = std::string(envLayoutOrg);
            }
        }
    } catch (const std::exception& e) {
        layoutClassName.clear();
    }

    if (!layoutClassName.empty()) {
        try {
            if (layoutClassName == "SpringBox") {
                return std::make_unique<SpringBox>(false);
            } else {
                std::cerr << "Class '" << layoutClassName << "' is not a 'Layout'" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Cannot create layout: " << e.what() << std::endl;
        }
    }

    return std::make_unique<SpringBox>(false);
}
