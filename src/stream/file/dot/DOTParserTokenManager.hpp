#ifndef DOT_PARSER_TOKEN_MANAGER_HPP
#define DOT_PARSER_TOKEN_MANAGER_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include "SimpleCharStream.hpp"
#include "Token.hpp"
#include "TokenMgrError.hpp"
#include "DOTParserConstants.hpp"

namespace dot_parser {

class DOTParserTokenManager : public DOTParserConstants {
public:
    DOTParserTokenManager(SimpleCharStream& stream);
    DOTParserTokenManager(SimpleCharStream& stream, int lexState);

    void setDebugStream(std::ostream& ds);
    void ReInit(SimpleCharStream& stream);
    void ReInit(SimpleCharStream& stream, int lexState);
    void SwitchTo(int lexState);
    Token getNextToken();

private:
    SimpleCharStream* input_stream;
    int curLexState;
    int defaultLexState;
    int jjnewStateCnt;
    int jjround;
    int jjmatchedPos;
    int jjmatchedKind;
    char curChar;

    std::ostream& debugStream;

    static const long jjtoToken[1];
    static const long jjtoSkip[1];
    static const int jjnextStates[14];

    int jjMoveStringLiteralDfa0_0();
    Token jjFillToken();
    void ReInitRounds();
    void jjCheckNAdd(int state);
    void jjAddStates(int start, int end);
    void jjCheckNAddTwoStates(int state1, int state2);
    void jjCheckNAddStates(int start, int end);
};

} // namespace dot_parser

#endif // DOT_PARSER_TOKEN_MANAGER_HPP
