#include "DGSParser.hpp"

DGSParser::DGSParser(std::shared_ptr<FileSourceDGS> dgs, std::istream& reader)
    : dgs(dgs), reader(reader), line(1), column(0), pushbackOffset(-1) {
    this->sourceId = "<DGS stream>";
}

void DGSParser::close() {
    // In C++, closing is handled by the destructor of streams, no need to manually close.
}

void DGSParser::open() {
    header();
}

void DGSParser::all() {
    open();
    while (next());
}

bool DGSParser::next() {
    lastDirective = directive();

    switch (lastDirective) {
    case Token::AN:
        dgs->sendNodeAdded(sourceId, id());
        attributes(ElementType::NODE, id());
        break;
    case Token::CN:
        attributes(ElementType::NODE, id());
        break;
    case Token::DN:
        dgs->sendNodeRemoved(sourceId, id());
        break;
    case Token::AE:
        // handle AE logic here...
        break;
    case Token::CE:
        attributes(ElementType::EDGE, id());
        break;
    case Token::DE:
        dgs->sendEdgeRemoved(sourceId, id());
        break;
    case Token::CG:
        attributes(ElementType::GRAPH, "");
        break;
    case Token::ST:
        // handle ST logic here...
        break;
    case Token::CL:
        dgs->sendGraphCleared(sourceId);
        break;
    case Token::TF:
        // handle TF logic here...
        break;
    case Token::EOF:
        return false;
    }

    skipWhitespaces();
    int c = nextChar();
    if (c == '#') {
        skipLine();
        return true;
    }

    if (c < 0)
        return false;

    if (c != '\n')
        throw parseException("eol expected");

    return true;
}

bool DGSParser::nextStep() {
    bool r;
    Token next;

    do {
        r = next();
        next = directive();

        if (next != Token::EOF) {
            pushbackChar(static_cast<int>(next));
        }
    } while (next != Token::ST && next != Token::EOF);

    return r;
}

int DGSParser::nextChar() {
    if (pushbackOffset >= 0)
        return pushback[pushbackOffset--];

    char c;
    if (!reader.get(c))
        return -1;

    if (c == '\n') {
        line++;
        column = 0;
    } else {
        column++;
    }

    return static_cast<int>(c);
}

void DGSParser::pushbackChar(int c) {
    if (c < 0)
        return;

    if (pushbackOffset + 1 >= sizeof(pushback) / sizeof(int))
        throw std::overflow_error("Pushback buffer overflow");

    pushback[++pushbackOffset] = c;
}

void DGSParser::skipLine() {
    int c;
    while ((c = nextChar()) != '\n' && c >= 0);
}

void DGSParser::skipWhitespaces() {
    int c;
    while ((c = nextChar()) == ' ' || c == '\t');
    pushbackChar(c);
}

void DGSParser::header() {
    std::string dgs;
    for (int i = 0; i < 3; ++i)
        dgs += static_cast<char>(nextChar());

    if (dgs != "DGS")
        throw parseException("bad magic header, 'DGS' expected");

    if (nextChar() != '\n')
        throw parseException("end-of-line is missing");

    skipLine();
}

void DGSParser::attributes(ElementType type, const std::string& id) {
    skipWhitespaces();
    int c;
    while ((c = nextChar()) != '\n' && c != '#' && c >= 0) {
        pushbackChar(c);
        attribute(type, id);
        skipWhitespaces();
    }
    pushbackChar(c);
}

void DGSParser::attribute(ElementType type, const std::string& elementId) {
    skipWhitespaces();
    int c = nextChar();
    AttributeChangeEvent ch = AttributeChangeEvent::CHANGE;

    if (c == '+')
        ch = AttributeChangeEvent::ADD;
    else if (c == '-')
        ch = AttributeChangeEvent::REMOVE;
    else
        pushbackChar(c);

    std::string key = id();
    if (key.empty())
        throw parseException("attribute key expected");

    skipWhitespaces();
    c = nextChar();
    std::shared_ptr<void> value = nullptr;

    if (c == '=' || c == ':') {
        skipWhitespaces();
        value = value(true);
    } else {
        value = std::make_shared<bool>(true);
        pushbackChar(c);
    }

    dgs->sendAttributeChangedEvent(sourceId, elementId, type, key, ch, nullptr, value);
}

Token DGSParser::directive() {
    int c1, c2;

    do {
        c1 = nextChar();
        if (c1 == '#')
            skipLine();

        if (c1 < 0)
            return Token::EOF;
    } while (c1 == '#' || c1 == '\n');

    c2 = nextChar();

    switch (c1) {
    case 'A':
    case 'a':
        if (c2 == 'n' || c2 == 'N')
            return Token::AN;
        if (c2 == 'e' || c2 == 'E')
            return Token::AE;
        break;
    case 'C':
    case 'c':
        if (c2 == 'n' || c2 == 'N')
            return Token::CN;
        if (c2 == 'e' || c2 == 'E')
            return Token::CE;
        if (c2 == 'g' || c2 == 'G')
            return Token::CG;
        if (c2 == 'l' || c2 == 'L')
            return Token::CL;
        break;
    case 'D':
    case 'd':
        if (c2 == 'n' || c2 == 'N')
            return Token::DN;
        if (c2 == 'e' || c2 == 'E')
            return Token::DE;
        break;
    case 'S':
    case 's':
        if (c2 == 't' || c2 == 'T')
            return Token::ST;
        break;
    case 'T':
    case 't':
        if (c2 == 'f' || c2 == 'F')
            return Token::TF;
        break;
    }

    throw parseException("unknown directive");
}

std::string DGSParser::string() {
    int c, s;
    std::string result;
    bool slash = false;

    c = nextChar();
    if (c != '\"' && c != '\'')
        throw parseException("string expected");

    s = c;

    while ((c = nextChar()) != s || slash) {
        if (slash && c != s)
            result += '\\';

        slash = c == '\\';

        if (!slash)
            result += static_cast<char>(c);
    }

    return result;
}

std::string DGSParser::id() {
    skipWhitespaces();
    std::string result;
    int c = nextChar();
    pushbackChar(c);

    if (c == '\"' || c == '\'')
        return string();

    bool stop = false;
    while (!stop) {
        c = nextChar();
        if (isalnum(c) || c == '-' || c == '+' || c == '_' || c == '.')
            result += static_cast<char>(c);
        else
            stop = true;
    }

    pushbackChar(c);
    return result;
}

std::shared_ptr<void> DGSParser::value(bool array) {
    // Implement value parsing
    return nullptr;
}

std::shared_ptr<void> DGSParser::map() {
    // Implement map parsing
    return nullptr;
}

std::shared_ptr<void> DGSParser::array() {
    // Implement array parsing
    return nullptr;
}

std::string DGSParser::color() {
    // Implement color parsing
    return "";
}

ParseException DGSParser::parseException(const std::string& message) {
    std::ostringstream oss;
    oss << "Parse error at (" << line << ";" << column << ") : " << message;
    return ParseException(oss.str());
}
