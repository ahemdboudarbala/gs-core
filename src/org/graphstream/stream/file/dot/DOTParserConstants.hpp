// DOTParserConstants.hpp

#ifndef DOT_PARSER_CONSTANTS_HPP
#define DOT_PARSER_CONSTANTS_HPP

#include <string>
#include <array>

namespace dot_parser {

// Token literal values and constants
class DOTParserConstants {
public:
    // End of File
    static const int EOF = 0;
    // RegularExpression Ids
    static const int EOL = 7;
    static const int DIGIT = 8;
    static const int HEXDIGIT = 9;
    static const int LSQBR = 10;
    static const int RSQBR = 11;
    static const int LBRACE = 12;
    static const int RBRACE = 13;
    static const int COLON = 14;
    static const int COMMA = 15;
    static const int EQUALS = 16;
    static const int GRAPH = 17;
    static const int DIGRAPH = 18;
    static const int SUBGRAPH = 19;
    static const int NODE = 20;
    static const int EDGE = 21;
    static const int STRICT = 22;
    static const int EDGE_OP = 23;
    static const int REAL = 24;
    static const int STRING = 25;
    static const int WORD = 26;

    // Lexical state
    static const int DEFAULT = 0;

    // Literal token values
    static const std::array<std::string, 37> tokenImage;

private:
    DOTParserConstants() = default;
};

// Initialize the tokenImage array with literal values
const std::array<std::string, 37> DOTParserConstants::tokenImage = {
    "<EOF>", "\" \"", "\"\\r\"", "\"\\t\"", "\"\\n\"", "<token of kind 5>", "<token of kind 6>",
    "<EOL>", "<DIGIT>", "<HEXDIGIT>", "\"[\"", "\"]\"", "\"{\"", "\"}\"", "\":\"", "\",\"", "\"=\"",
    "\"graph\"", "\"digraph\"", "\"subgraph\"", "\"node\"", "\"edge\"", "\"strict\"", "<EDGE_OP>", "<REAL>",
    "<STRING>", "<WORD>", "\";\"", "\"n\"", "\"ne\"", "\"e\"", "\"se\"", "\"s\"", "\"sw\"", "\"w\"", "\"nw\"",
    "\"c\"", "\"_\""
};

} // namespace dot_parser

#endif // DOT_PARSER_CONSTANTS_HPP
