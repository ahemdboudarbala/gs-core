#include "FileSinkDGSFiltered.hpp"
#include "FileSinkDGSUtility.hpp"
#include <iomanip>

FileSinkDGSFiltered::FileSinkDGSFiltered()
    : out(nullptr), graphName("") {
}

void FileSinkDGSFiltered::outputHeader() {
    out = dynamic_cast<std::ostream*>(output);

    if (out) {
        *out << "DGS004" << std::endl;

        if (graphName.empty()) {
            *out << "null 0 0" << std::endl;
        } else {
            *out << "\"" << FileSinkDGSUtility::formatStringForQuoting(graphName) << "\" 0 0" << std::endl;
        }
    }
}

void FileSinkDGSFiltered::outputEndOfFile() {
    // No operation needed for end of file in DGS format
}

void FileSinkDGSFiltered::edgeAttributeAdded(const std::string& graphId, long timeId, const std::string& edgeId,
                                             const std::string& attribute, const std::string& value) {
    if (noFilterEdgeAttributeAdded && edgeAttributesFiltered.find(attribute) == edgeAttributesFiltered.end()) {
        edgeAttributeChanged(graphId, timeId, edgeId, attribute, "", value);
    }
}

void FileSinkDGSFiltered::edgeAttributeChanged(const std::string& graphId, long timeId, const std::string& edgeId,
                                               const std::string& attribute, const std::string& oldValue,
                                               const std::string& newValue) {
    if (noFilterEdgeAttributeChanged && edgeAttributesFiltered.find(attribute) == edgeAttributesFiltered.end()) {
        if (out) {
            *out << "ce \"" << FileSinkDGSUtility::formatStringForQuoting(edgeId) << "\" "
                 << FileSinkDGSUtility::attributeString(attribute, newValue, false) << std::endl;
        }
    }
}

void FileSinkDGSFiltered::edgeAttributeRemoved(const std::string& graphId, long timeId, const std::string& edgeId,
                                               const std::string& attribute) {
    if (noFilterEdgeAttributeRemoved && edgeAttributesFiltered.find(attribute) == edgeAttributesFiltered.end()) {
        if (out) {
            *out << "ce \"" << FileSinkDGSUtility::formatStringForQuoting(edgeId) << "\" "
                 << FileSinkDGSUtility::attributeString(attribute, "", true) << std::endl;
        }
    }
}

void FileSinkDGSFiltered::graphAttributeAdded(const std::string& graphId, long timeId, const std::string& attribute,
                                              const std::string& value) {
    if (noFilterGraphAttributeAdded && graphAttributesFiltered.find(attribute) == graphAttributesFiltered.end()) {
        graphAttributeChanged(graphId, timeId, attribute, "", value);
    }
}

void FileSinkDGSFiltered::graphAttributeChanged(const std::string& graphId, long timeId, const std::string& attribute,
                                                const std::string& oldValue, const std::string& newValue) {
    if (noFilterGraphAttributeChanged && graphAttributesFiltered.find(attribute) == graphAttributesFiltered.end()) {
        if (out) {
            *out << "cg " << FileSinkDGSUtility::attributeString(attribute, newValue, false) << std::endl;
        }
    }
}

void FileSinkDGSFiltered::graphAttributeRemoved(const std::string& graphId, long timeId, const std::string& attribute) {
    if (noFilterGraphAttributeRemoved && graphAttributesFiltered.find(attribute) == graphAttributesFiltered.end()) {
        if (out) {
            *out << "cg " << FileSinkDGSUtility::attributeString(attribute, "", true) << std::endl;
        }
    }
}

void FileSinkDGSFiltered::nodeAttributeAdded(const std::string& graphId, long timeId, const std::string& nodeId,
                                             const std::string& attribute, const std::string& value) {
    if (noFilterNodeAttributeAdded && nodeAttributesFiltered.find(attribute) == nodeAttributesFiltered.end()) {
        nodeAttributeChanged(graphId, timeId, nodeId, attribute, "", value);
    }
}

void FileSinkDGSFiltered::nodeAttributeChanged(const std::string& graphId, long timeId, const std::string& nodeId,
                                               const std::string& attribute, const std::string& oldValue,
                                               const std::string& newValue) {
    if (noFilterNodeAttributeChanged && nodeAttributesFiltered.find(attribute) == nodeAttributesFiltered.end()) {
        if (out) {
            *out << "cn \"" << FileSinkDGSUtility::formatStringForQuoting(nodeId) << "\" "
                 << FileSinkDGSUtility::attributeString(attribute, newValue, false) << std::endl;
        }
    }
}

void FileSinkDGSFiltered::nodeAttributeRemoved(const std::string& graphId, long timeId, const std::string& nodeId,
                                               const std::string& attribute) {
    if (noFilterNodeAttributeRemoved && nodeAttributesFiltered.find(attribute) == nodeAttributesFiltered.end()) {
        if (out) {
            *out << "cn \"" << FileSinkDGSUtility::formatStringForQuoting(nodeId) << "\" "
                 << FileSinkDGSUtility::attributeString(attribute, "", true) << std::endl;
        }
    }
}

void FileSinkDGSFiltered::edgeAdded(const std::string& graphId, long timeId, const std::string& edgeId,
                                    const std::string& fromNodeId, const std::string& toNodeId, bool directed) {
    if (noFilterEdgeAdded) {
        if (out) {
            *out << "ae \"" << FileSinkDGSUtility::formatStringForQuoting(edgeId) << "\" \""
                 << FileSinkDGSUtility::formatStringForQuoting(fromNodeId) << "\" "
                 << (directed ? ">" : "") << " \""
                 << FileSinkDGSUtility::formatStringForQuoting(toNodeId) << "\"" << std::endl;
        }
    }
}

void FileSinkDGSFiltered::edgeRemoved(const std::string& graphId, long timeId, const std::string& edgeId) {
    if (noFilterEdgeRemoved) {
        if (out) {
            *out << "de \"" << FileSinkDGSUtility::formatStringForQuoting(edgeId) << "\"" << std::endl;
        }
    }
}

void FileSinkDGSFiltered::graphCleared(const std::string& graphId, long timeId) {
    if (noFilterGraphCleared) {
        if (out) {
            *out << "cl" << std::endl;
        }
    }
}

void FileSinkDGSFiltered::nodeAdded(const std::string& graphId, long timeId, const std::string& nodeId) {
    if (noFilterNodeAdded) {
        if (out) {
            *out << "an \"" << FileSinkDGSUtility::formatStringForQuoting(nodeId) << "\"" << std::endl;
        }
    }
}

void FileSinkDGSFiltered::nodeRemoved(const std::string& graphId, long timeId, const std::string& nodeId) {
    if (noFilterNodeRemoved) {
        if (out) {
            *out << "dn \"" << FileSinkDGSUtility::formatStringForQuoting(nodeId) << "\"" << std::endl;
        }
    }
}

void FileSinkDGSFiltered::stepBegins(const std::string& graphId, long timeId, double step) {
    if (noFilterStepBegins) {
        if (out) {
            *out << std::fixed << std::setprecision(6) << "st " << step << std::endl;
        }
    }
}
