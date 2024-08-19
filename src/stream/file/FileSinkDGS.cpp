#include "FileSinkDGS.hpp"
#include "FileSinkDGSUtility.hpp"

FileSinkDGS::FileSinkDGS()
    : out(nullptr), graphName("") {
}

void FileSinkDGS::outputHeader() {
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

void FileSinkDGS::outputEndOfFile() {
    // No operation needed for end of file in DGS format
}

void FileSinkDGS::edgeAttributeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute, const std::string& value) {
    edgeAttributeChanged(graphId, timeId, edgeId, attribute, "", value);
}

void FileSinkDGS::edgeAttributeChanged(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute, const std::string& oldValue, const std::string& newValue) {
    if (out) {
        *out << "ce \"" << FileSinkDGSUtility::formatStringForQuoting(edgeId) << "\" "
             << FileSinkDGSUtility::attributeString(attribute, newValue, false) << std::endl;
    }
}

void FileSinkDGS::edgeAttributeRemoved(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute) {
    if (out) {
        *out << "ce \"" << FileSinkDGSUtility::formatStringForQuoting(edgeId) << "\" "
             << FileSinkDGSUtility::attributeString(attribute, "", true) << std::endl;
    }
}

void FileSinkDGS::graphAttributeAdded(const std::string& graphId, long timeId, const std::string& attribute, const std::string& value) {
    graphAttributeChanged(graphId, timeId, attribute, "", value);
}

void FileSinkDGS::graphAttributeChanged(const std::string& graphId, long timeId, const std::string& attribute, const std::string& oldValue, const std::string& newValue) {
    if (out) {
        *out << "cg " << FileSinkDGSUtility::attributeString(attribute, newValue, false) << std::endl;
    }
}

void FileSinkDGS::graphAttributeRemoved(const std::string& graphId, long timeId, const std::string& attribute) {
    if (out) {
        *out << "cg " << FileSinkDGSUtility::attributeString(attribute, "", true) << std::endl;
    }
}

void FileSinkDGS::nodeAttributeAdded(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute, const std::string& value) {
    nodeAttributeChanged(graphId, timeId, nodeId, attribute, "", value);
}

void FileSinkDGS::nodeAttributeChanged(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute, const std::string& oldValue, const std::string& newValue) {
    if (out) {
        *out << "cn \"" << FileSinkDGSUtility::formatStringForQuoting(nodeId) << "\" "
             << FileSinkDGSUtility::attributeString(attribute, newValue, false) << std::endl;
    }
}

void FileSinkDGS::nodeAttributeRemoved(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute) {
    if (out) {
        *out << "cn \"" << FileSinkDGSUtility::formatStringForQuoting(nodeId) << "\" "
             << FileSinkDGSUtility::attributeString(attribute, "", true) << std::endl;
    }
}

void FileSinkDGS::edgeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed) {
    if (out) {
        *out << "ae \"" << FileSinkDGSUtility::formatStringForQuoting(edgeId) << "\" \""
             << FileSinkDGSUtility::formatStringForQuoting(fromNodeId) << "\" "
             << (directed ? ">" : "") << " \""
             << FileSinkDGSUtility::formatStringForQuoting(toNodeId) << "\"" << std::endl;
    }
}

void FileSinkDGS::edgeRemoved(const std::string& graphId, long timeId, const std::string& edgeId) {
    if (out) {
        *out << "de \"" << FileSinkDGSUtility::formatStringForQuoting(edgeId) << "\"" << std::endl;
    }
}

void FileSinkDGS::graphCleared(const std::string& graphId, long timeId) {
    if (out) {
        *out << "cl" << std::endl;
    }
}

void FileSinkDGS::nodeAdded(const std::string& graphId, long timeId, const std::string& nodeId) {
    if (out) {
        *out << "an \"" << FileSinkDGSUtility::formatStringForQuoting(nodeId) << "\"" << std::endl;
    }
}

void FileSinkDGS::nodeRemoved(const std::string& graphId, long timeId, const std::string& nodeId) {
    if (out) {
        *out << "dn \"" << FileSinkDGSUtility::formatStringForQuoting(nodeId) << "\"" << std::endl;
    }
}

void FileSinkDGS::stepBegins(const std::string& graphId, long timeId, double step) {
    if (out) {
        *out << std::setprecision(6) << std::fixed << "st " << step << std::endl;
    }
}
