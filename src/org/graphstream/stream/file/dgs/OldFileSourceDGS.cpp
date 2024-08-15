#include "OldFileSourceDGS.hpp"

OldFileSourceDGS::OldFileSourceDGS() : FileSourceBase(true), finished(false), stepCount(0), eventCount(0) {
}

bool OldFileSourceDGS::nextEvents() {
    if (finished)
        return false;
    return next(false, false);
}

bool OldFileSourceDGS::nextStep() {
    if (finished)
        return false;
    return next(true, false);
}

void OldFileSourceDGS::begin(const std::string& filename) {
    FileSourceBase::begin(filename);
    beginParsing();
}

void OldFileSourceDGS::begin(std::istream& stream) {
    FileSourceBase::begin(stream);
    beginParsing();
}

bool OldFileSourceDGS::next(bool readSteps, bool stop) {
    std::string key;
    bool loop = readSteps;

    do {
        key = getWordOrSymbolOrStringOrEolOrEof();

        if (key == "ce") {
            readCE();
        } else if (key == "cn") {
            readCN();
        } else if (key == "ae") {
            readAE();
        } else if (key == "an") {
            readAN();
        } else if (key == "de") {
            readDE();
        } else if (key == "dn") {
            readDN();
        } else if (key == "cg") {
            readCG();
        } else if (key == "st") {
            if (readSteps) {
                if (stop) {
                    loop = false;
                    pushBack();
                } else {
                    stop = true;
                    readST();
                }
            } else {
                readST();
            }
        } else if (key == "#") {
            eatAllUntilEol();
            return next(readSteps, stop);
        } else if (key == "EOL") {
            return next(readSteps, stop);
        } else if (key == "EOF") {
            finished = true;
            return false;
        } else {
            parseError("Unknown token '" + key + "'");
        }
    } while (loop);

    return true;
}

void OldFileSourceDGS::readCE() {
    std::string tag = getStringOrWordOrNumber();
    readAttributes(attributes);

    for (const auto& attr : attributes) {
        const std::string& key = attr.first;
        std::shared_ptr<void> value = attr.second;

        if (!value)
            sendEdgeAttributeRemoved(graphName, tag, key);
        else
            sendEdgeAttributeChanged(graphName, tag, key, nullptr, value);
    }

    if (eatEolOrEof() == EOF)
        pushBack();
}

void OldFileSourceDGS::readCN() {
    std::string tag = getStringOrWordOrNumber();
    readAttributes(attributes);

    for (const auto& attr : attributes) {
        const std::string& key = attr.first;
        std::shared_ptr<void> value = attr.second;

        if (!value)
            sendNodeAttributeRemoved(graphName, tag, key);
        else
            sendNodeAttributeChanged(graphName, tag, key, nullptr, value);
    }

    if (eatEolOrEof() == EOF)
        pushBack();
}

void OldFileSourceDGS::readCG() {
    readAttributes(attributes);

    for (const auto& attr : attributes) {
        const std::string& key = attr.first;
        std::shared_ptr<void> value = attr.second;

        if (!value)
            sendGraphAttributeRemoved(graphName, key);
        else
            sendGraphAttributeChanged(graphName, key, nullptr, value);
    }

    if (eatEolOrEof() == EOF)
        pushBack();
}

void OldFileSourceDGS::readAE() {
    int dir = 0;
    bool directed = false;
    std::string dirc;
    std::string tag;
    std::string fromTag;
    std::string toTag;

    tag = getStringOrWordOrNumber();
    fromTag = getStringOrWordOrNumber();
    dirc = getWordOrSymbolOrNumberOrStringOrEolOrEof();

    if (dirc == ">") {
        directed = true;
        dir = 1;
    } else if (dirc == "<") {
        directed = true;
        dir = 2;
    } else {
        pushBack();
    }

    toTag = getStringOrWordOrNumber();

    if (dir == 2) {
        std::swap(toTag, fromTag);
    }

    readAttributes(attributes);
    sendEdgeAdded(graphName, tag, fromTag, toTag, directed);

    for (const auto& attr : attributes) {
        const std::string& key = attr.first;
        std::shared_ptr<void> value = attr.second;
        sendEdgeAttributeAdded(graphName, tag, key, value);
    }

    if (eatEolOrEof() == EOF)
        pushBack();
}

void OldFileSourceDGS::readAN() {
    std::string tag = getStringOrWordOrNumber();
    readAttributes(attributes);

    sendNodeAdded(graphName, tag);

    for (const auto& attr : attributes) {
        const std::string& key = attr.first;
        std::shared_ptr<void> value = attr.second;
        sendNodeAttributeAdded(graphName, tag, key, value);
    }

    if (eatEolOrEof() == EOF)
        pushBack();
}

void OldFileSourceDGS::readDE() {
    std::string tag = getStringOrWordOrNumber();
    sendEdgeRemoved(graphName, tag);

    if (eatEolOrEof() == EOF)
        pushBack();
}

void OldFileSourceDGS::readDN() {
    std::string tag = getStringOrWordOrNumber();
    sendNodeRemoved(graphName, tag);

    if (eatEolOrEof() == EOF)
        pushBack();
}

void OldFileSourceDGS::readST() {
    std::string w = getWordOrNumber();

    try {
        double time = std::stod(w);
        sendStepBegins(graphName, time);
    } catch (const std::invalid_argument&) {
        parseError("Expecting a number after 'st', got '" + w + "'");
    }

    if (eatEolOrEof() == EOF)
        pushBack();
}

void OldFileSourceDGS::readAttributes(std::map<std::string, std::shared_ptr<void>>& attributes) {
    bool del = false;
    std::string key = getWordOrSymbolOrStringOrEolOrEof();

    attributes.clear();

    if (key == "-") {
        key = getWordOrSymbolOrStringOrEolOrEof();
        del = true;
    }

    if (key == "+")
        key = getWordOrSymbolOrStringOrEolOrEof();

    while (key != "EOF" && key != "EOL" && key != "]") {
        if (del)
            attributes[key] = nullptr;
        else
            attributes[key] = readAttributeValue(key);

        key = getWordOrSymbolOrStringOrEolOrEof();

        if (key == "-") {
            key = getWordOrStringOrEolOrEof();
            del = true;
        }

        if (key == "+") {
            key = getWordOrStringOrEolOrEof();
            del = false;
        }
    }

    pushBack();
}

std::shared_ptr<void> OldFileSourceDGS::readAttributeValue(const std::string& key) {
    std::vector<std::shared_ptr<void>> vector;
    std::shared_ptr<void> value;
    std::shared_ptr<void> value2;
    std::string next;

    if (!key.empty())
        eatSymbols(":=");

    value = getStringOrWordOrSymbolOrNumberO();

    if (*value == "[") {
        std::map<std::string, std::shared_ptr<void>> map;
        readAttributes(map);
        eatSymbol(']');
        value = std::make_shared<decltype(map)>(std::move(map));
    } else if (*value == "{") {
        vector = readAttributeArray(key);
        eatSymbol('}');
    } else {
        pushBack();

        value = getStringOrWordOrNumberO();

        if (!key.empty()) {
            next = getWordOrSymbolOrNumberOrStringOrEolOrEof();

            while (next == ",") {
                if (vector.empty()) {
                    vector.push_back(value);
                }

                value2 = getStringOrWordOrNumberO();
                next = getWordOrSymbolOrNumberOrStringOrEolOrEof();

                vector.push_back(value2);
            }

            pushBack();
        }
    }

    if (!vector.empty())
        return std::make_shared<decltype(vector)>(std::move(vector));
    else
        return value;
}

std::vector<std::shared_ptr<void>> OldFileSourceDGS::readAttributeArray(const std::string& key) {
    std::vector<std::shared_ptr<void>> list;
    std::shared_ptr<void> value;
    std::string next;

    do {
        value = readAttributeValue("");
        next = getWordOrSymbolOrNumberOrStringOrEolOrEof();
        list.push_back(value);
    } while (next == ",");

    pushBack();
    return list;
}

void OldFileSourceDGS::beginParsing() {
    st.parseNumbers();
    eatWords("DGS003", "DGS004");

    version = 3;
    eatEol();
    graphName = getWordOrString();
    stepCountAnnounced = static_cast<int>(getNumber());
    eventCountAnnounced = static_cast<int>(getNumber());
    eatEol();

    if (!graphName.empty())
        sendGraphAttributeAdded(graphName, "label", graphName);
    else
        graphName = "DGS_";

    graphName = graphName + "_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
}

std::unique_ptr<std::istream> OldFileSourceDGS::createReaderFrom(const std::string& file) {
    std::unique_ptr<std::istream> is(new std::ifstream(file, std::ios::in | std::ios::binary));

    if (is->good()) {
        is->seekg(0, std::ios::beg);
    }

    return is;
}

std::unique_ptr<std::istream> OldFileSourceDGS::createReaderFrom(std::istream& stream) {
    return std::make_unique<std::istream>(stream.rdbuf());
}

void OldFileSourceDGS::configureTokenizer(std::istringstream& tokenizer) {
    // Set up tokenizer options for parsing
}
