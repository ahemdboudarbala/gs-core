#include "DOTParser.hpp"

DOTParser::DOTParser(FileSourceDOT* dot, std::istream& stream) {
    init(dot);
    // Assume SimpleCharStream and TokenManager initialization logic here
}

DOTParser::DOTParser(FileSourceDOT* dot, std::unique_ptr<std::istream>& stream) {
    init(dot);
    // Assume SimpleCharStream and TokenManager initialization logic here
}

void DOTParser::init(FileSourceDOT* dot) {
    this->dot = dot;
    this->sourceId = "<DOT stream " + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ">";
    globalNodesAttributes.clear();
    globalEdgesAttributes.clear();
    nodeAdded.clear();
    jj_gen = 0;
}

void DOTParser::close() {
    jj_input_stream.close();
}

void DOTParser::all() {
    graph();
    while (true) {
        int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
        if (ntk == GRAPH || ntk == SUBGRAPH || ntk == NODE || ntk == EDGE || ntk == REAL || ntk == STRING || ntk == WORD) {
            statement();
        } else {
            break;
        }
    }
    jj_consume_token(RBRACE);
}

bool DOTParser::next() {
    bool hasMore = false;
    int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == GRAPH || ntk == SUBGRAPH || ntk == NODE || ntk == EDGE || ntk == REAL || ntk == STRING || ntk == WORD) {
        statement();
        hasMore = true;
    } else if (ntk == RBRACE) {
        jj_consume_token(RBRACE);
    } else {
        jj_consume_token(0);
    }
    return hasMore;
}

void DOTParser::open() {
    graph();
}

void DOTParser::graph() {
    directed = false;
    strict = false;
    globalNodesAttributes.clear();
    globalEdgesAttributes.clear();

    int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == STRICT) {
        jj_consume_token(STRICT);
        strict = true;
    }

    ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == GRAPH) {
        jj_consume_token(GRAPH);
    } else if (ntk == DIGRAPH) {
        jj_consume_token(DIGRAPH);
        directed = true;
    }

    ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == REAL || ntk == STRING || ntk == WORD) {
        sourceId = id();
    }
    jj_consume_token(LBRACE);
}

void DOTParser::subgraph() {
    jj_consume_token(SUBGRAPH);
    int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == REAL || ntk == STRING || ntk == WORD) {
        id();
    }
    jj_consume_token(LBRACE);

    while (true) {
        ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
        if (ntk == GRAPH || ntk == SUBGRAPH || ntk == NODE || ntk == EDGE || ntk == REAL || ntk == STRING || ntk == WORD) {
            statement();
        } else {
            break;
        }
    }
    jj_consume_token(RBRACE);
}

std::string DOTParser::id() {
    Token* t;
    std::string id;
    int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == STRING) {
        t = jj_consume_token(STRING);
        id = t->image.substr(1, t->image.length() - 2);
    } else if (ntk == REAL) {
        t = jj_consume_token(REAL);
        id = t->image;
    } else if (ntk == WORD) {
        t = jj_consume_token(WORD);
        id = t->image;
    } else {
        jj_consume_token(-1);
        throw generateParseException();
    }
    return id;
}

void DOTParser::statement() {
    if (jj_2_1(3)) {
        edgeStatement();
    } else {
        int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
        if (ntk == REAL || ntk == STRING || ntk == WORD) {
            nodeStatement();
        } else if (ntk == GRAPH || ntk == NODE || ntk == EDGE) {
            attributeStatement();
        } else if (ntk == SUBGRAPH) {
            subgraph();
        } else {
            jj_consume_token(-1);
            throw generateParseException();
        }
    }
    jj_consume_token(27);
}

void DOTParser::nodeStatement() {
    std::string nodeId;
    std::vector<std::string> port;
    std::map<std::string, std::shared_ptr<void>>* attr = nullptr;

    nodeId = id();
    int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == COLON) {
        port = port();
    }
    ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == LSQBR) {
        attr = &attributesList();
    }
    addNode(nodeId, port, attr);
}

std::string DOTParser::compassPoint() {
    Token* pt = nullptr;
    int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    switch (ntk) {
        case 28: pt = jj_consume_token(28); break;
        case 29: pt = jj_consume_token(29); break;
        case 30: pt = jj_consume_token(30); break;
        case 31: pt = jj_consume_token(31); break;
        case 32: pt = jj_consume_token(32); break;
        case 33: pt = jj_consume_token(33); break;
        case 34: pt = jj_consume_token(34); break;
        case 35: pt = jj_consume_token(35); break;
        case 36: pt = jj_consume_token(36); break;
        case 37: pt = jj_consume_token(37); break;
        default: jj_consume_token(-1); throw generateParseException();
    }
    return pt->image;
}

std::vector<std::string> DOTParser::port() {
    std::vector<std::string> p(2, "");
    jj_consume_token(COLON);
    int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == REAL || ntk == STRING || ntk == WORD) {
        p[0] = id();
        ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
        if (ntk == COLON) {
            jj_consume_token(COLON);
            p[1] = compassPoint();
        }
    } else if (ntk >= 28 && ntk <= 37) {
        p[1] = compassPoint();
    } else {
        jj_consume_token(-1);
        throw generateParseException();
    }
    return p;
}

void DOTParser::edgeStatement() {
    std::string id;
    std::vector<std::string> edges;
    std::map<std::string, std::shared_ptr<void>>* attr = nullptr;
    id = id();
    edges.push_back(id);
    edgeRHS(edges);
    int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == LSQBR) {
        attr = &attributesList();
    }
    addEdges(edges, attr);
}

void DOTParser::edgeRHS(std::vector<std::string>& edges) {
    Token* t;
    std::string i;
    t = jj_consume_token(EDGE_OP);
    edges.push_back(t->image);
    i = id();
    edges.push_back(i);
    int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == EDGE_OP) {
        edgeRHS(edges);
    }
}

void DOTParser::attributeStatement() {
    Token* t;
    std::map<std::string, std::shared_ptr<void>>* attr = nullptr;
    int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == GRAPH) {
        t = jj_consume_token(GRAPH);
    } else if (ntk == NODE) {
        t = jj_consume_token(NODE);
    } else if (ntk == EDGE) {
        t = jj_consume_token(EDGE);
    } else {
        jj_consume_token(-1);
        throw generateParseException();
    }
    attr = &attributesList();
    setGlobalAttributes(t->image, attr);
}

std::map<std::string, std::shared_ptr<void>> DOTParser::attributesList() {
    std::map<std::string, std::shared_ptr<void>> attributes;
    while (true) {
        jj_consume_token(LSQBR);
        int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
        if (ntk == REAL || ntk == STRING || ntk == WORD) {
            attributeList(attributes);
            while (true) {
                ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
                if (ntk == COMMA) {
                    jj_consume_token(COMMA);
                    attributeList(attributes);
                } else {
                    break;
                }
            }
        }
        jj_consume_token(RSQBR);
        ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
        if (ntk != LSQBR) {
            break;
        }
    }
    return attributes;
}

void DOTParser::attributeList(std::map<std::string, std::shared_ptr<void>>& attributes) {
    std::string key;
    std::shared_ptr<void> val = std::make_shared<bool>(true);
    Token* t;

    key = id();
    int ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
    if (ntk == EQUALS) {
        jj_consume_token(EQUALS);
        if (jj_2_2(2)) {
            t = jj_consume_token(REAL);
            val = std::make_shared<double>(std::stod(t->image));
        } else {
            ntk = jj_ntk == -1 ? jj_ntk() : jj_ntk;
            if (ntk == REAL || ntk == STRING || ntk == WORD) {
                val = std::make_shared<std::string>(id());
            } else {
                jj_consume_token(-1);
                throw generateParseException();
            }
        }
    }
    attributes[key] = val;
}

bool DOTParser::jj_2_1(int xla) {
    jj_la = xla;
    jj_lastpos = jj_scanpos = token;
    try {
        return !jj_3_1();
    } catch (LookaheadSuccess& ls) {
        return true;
    } finally {
        jj_save(0, xla);
    }
}

bool DOTParser::jj_2_2(int xla) {
    jj_la = xla;
    jj_lastpos = jj_scanpos = token;
    try {
        return !jj_3_2();
    } catch (LookaheadSuccess& ls) {
        return true;
    } finally {
        jj_save(1, xla);
    }
}

// Implementations of jj_3R_6, jj_3_2, jj_3R_8, jj_3R_10, jj_3R_7, jj_3R_9, jj_3R_5, jj_3_1 here...

ParseException DOTParser::generateParseException() {
    // Implement generateParseException similar to Java's method
}

void DOTParser::jj_save(int index, int xla) {
    // Implement jj_save similar to Java's method
}

void DOTParser::jj_rescan_token() {
    // Implement jj_rescan_token similar to Java's method
}

Token* DOTParser::jj_consume_token(int kind) {
    Token* oldToken = token;
    if (token->next != nullptr) {
        token = token->next;
    } else {
        token = token->next = token_source.getNextToken();
    }
    jj_ntk = -1;
    if (token->kind == kind) {
        jj_gen++;
        return token;
    }
    token = oldToken;
    throw generateParseException();
}

bool DOTParser::jj_scan_token(int kind) {
    if (jj_scanpos == jj_lastpos) {
        jj_la--;
        if (jj_scanpos->next == nullptr) {
            jj_lastpos = jj_scanpos = jj_scanpos->next = token_source.getNextToken();
        } else {
            jj_lastpos = jj_scanpos = jj_scanpos->next;
        }
    } else {
        jj_scanpos = jj_scanpos->next;
    }
    if (jj_scanpos->kind != kind)
        return true;
    return false;
}

void DOTParser::addNode(const std::string& nodeId, const std::vector<std::string>& port, std::map<std::string, std::shared_ptr<void>>* attr) {
    if (nodeAdded.find(nodeId) != nodeAdded.end()) {
        if (attr != nullptr) {
            for (const auto& [key, value] : *attr) {
                dot->sendAttributeChangedEvent(sourceId, nodeId, ElementType::NODE, key, AttributeChangeEvent::ADD, nullptr, value);
            }
        }
    } else {
        dot->sendNodeAdded(sourceId, nodeId);
        nodeAdded.insert(nodeId);

        if (attr == nullptr) {
            for (const auto& [key, value] : globalNodesAttributes) {
                dot->sendAttributeChangedEvent(sourceId, nodeId, ElementType::NODE, key, AttributeChangeEvent::ADD, nullptr, value);
            }
        } else {
            for (const auto& [key, value] : globalNodesAttributes) {
                if (attr->find(key) == attr->end()) {
                    dot->sendAttributeChangedEvent(sourceId, nodeId, ElementType::NODE, key, AttributeChangeEvent::ADD, nullptr, value);
                }
            }

            for (const auto& [key, value] : *attr) {
                dot->sendAttributeChangedEvent(sourceId, nodeId, ElementType::NODE, key, AttributeChangeEvent::ADD, nullptr, value);
            }
        }
    }
}

void DOTParser::addEdges(std::vector<std::string>& edges, std::map<std::string, std::shared_ptr<void>>* attr) {
    std::map<std::string, int> hash;
    std::vector<std::string> ids((edges.size() - 1) / 2);
    std::vector<bool> directedFlags((edges.size() - 1) / 2);
    int count = 0;

    for (size_t i = 0; i < edges.size() - 1; i += 2) {
        std::string from = edges[i];
        std::string to = edges[i + 2];

        if (nodeAdded.find(from) == nodeAdded.end())
            addNode(from, {}, nullptr);
        if (nodeAdded.find(to) == nodeAdded.end())
            addNode(to, {}, nullptr);

        std::string edgeId = "(" + from + ";" + to + ")";
        std::string rev = "(" + to + ";" + from + ")";

        if (hash.find(rev) != hash.end()) {
            directedFlags[hash[rev]] = false;
        } else {
            hash[edgeId] = count;
            ids[count] = edgeId;
            directedFlags[count] = edges[i + 1] == "->";
            count++;
        }
    }

    hash.clear();

    if (count == 1 && attr != nullptr && attr->find("id") != attr->end()) {
        ids[0] = (*attr)["id"].get()->toString();
        attr->erase("id");
    }

    for (int i = 0; i < count; i++) {
        bool addedEdge = false;
        std::string IDtoTry = ids[i];
        while (!addedEdge) {
            try {
                dot->sendEdgeAdded(sourceId, ids[i], edges[i * 2], edges[(i + 1) * 2], directedFlags[i]);
                addedEdge = true;
            } catch (IdAlreadyInUseException& e) {
                IDtoTry += "'";
            }
        }

        if (attr == nullptr) {
            for (const auto& [key, value] : globalEdgesAttributes) {
                dot->sendAttributeChangedEvent(sourceId, ids[i], ElementType::EDGE, key, AttributeChangeEvent::ADD, nullptr, value);
            }
        } else {
            for (const auto& [key, value] : globalEdgesAttributes) {
                if (attr->find(key) == attr->end()) {
                    dot->sendAttributeChangedEvent(sourceId, ids[i], ElementType::EDGE, key, AttributeChangeEvent::ADD, nullptr, value);
                }
            }

            for (const auto& [key, value] : *attr) {
                dot->sendAttributeChangedEvent(sourceId, ids[i], ElementType::EDGE, key, AttributeChangeEvent::ADD, nullptr, value);
            }
        }
    }
}

void DOTParser::setGlobalAttributes(const std::string& who, std::map<std::string, std::shared_ptr<void>>* attr) {
    if (who == "graph") {
        for (const auto& [key, value] : *attr) {
            dot->sendAttributeChangedEvent(sourceId, sourceId, ElementType::GRAPH, key, AttributeChangeEvent::ADD, nullptr, value);
        }
    } else if (who == "node") {
        globalNodesAttributes.insert(attr->begin(), attr->end());
    } else if (who == "edge") {
        globalEdgesAttributes.insert(attr->begin(), attr->end());
    }
}
