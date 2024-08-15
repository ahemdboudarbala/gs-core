#include "DOTParserTokenManager.hpp"

namespace dot_parser {

const long DOTParserTokenManager::jjtoToken[1] = {0x3ffffffc01L};
const long DOTParserTokenManager::jjtoSkip[1] = {0x7eL};
const int DOTParserTokenManager::jjnextStates[14] = {21, 27, 8, 10, 11, 18, 19, 22, 23, 26, 1, 2, 13, 14};

DOTParserTokenManager::DOTParserTokenManager(SimpleCharStream& stream)
    : debugStream(std::cout), curLexState(0), defaultLexState(0) {
    if (SimpleCharStream::staticFlag)
        throw std::runtime_error("ERROR: Cannot use a static CharStream class with a non-static lexical analyzer.");
    input_stream = &stream;
    ReInitRounds();
}

DOTParserTokenManager::DOTParserTokenManager(SimpleCharStream& stream, int lexState)
    : DOTParserTokenManager(stream) {
    SwitchTo(lexState);
}

void DOTParserTokenManager::setDebugStream(std::ostream& ds) {
    debugStream = ds;
}

void DOTParserTokenManager::ReInit(SimpleCharStream& stream) {
    jjmatchedPos = jjnewStateCnt = 0;
    curLexState = defaultLexState;
    input_stream = &stream;
    ReInitRounds();
}

void DOTParserTokenManager::ReInit(SimpleCharStream& stream, int lexState) {
    ReInit(stream);
    SwitchTo(lexState);
}

void DOTParserTokenManager::SwitchTo(int lexState) {
    if (lexState >= 1 || lexState < 0)
        throw TokenMgrError("Error: Ignoring invalid lexical state: " + std::to_string(lexState) + ". State unchanged.",
                            TokenMgrError::INVALID_LEXICAL_STATE);
    else
        curLexState = lexState;
}

Token DOTParserTokenManager::getNextToken() {
    Token matchedToken;
    int curPos = 0;

    while (true) {
        try {
            curChar = input_stream->BeginToken();
        } catch (const std::ios_base::failure&) {
            jjmatchedKind = 0;
            matchedToken = jjFillToken();
            return matchedToken;
        }

        try {
            input_stream->backup(0);
            while (curChar <= 32 && (0x100002600L & (1L << curChar)) != 0L)
                curChar = input_stream->BeginToken();
        } catch (const std::ios_base::failure&) {
            continue;
        }

        jjmatchedKind = 0x7fffffff;
        jjmatchedPos = 0;
        curPos = jjMoveStringLiteralDfa0_0();
        if (jjmatchedKind != 0x7fffffff) {
            if (jjmatchedPos + 1 < curPos)
                input_stream->backup(curPos - jjmatchedPos - 1);
            if ((jjtoToken[jjmatchedKind >> 6] & (1L << (jjmatchedKind & 077))) != 0L) {
                matchedToken = jjFillToken();
                return matchedToken;
            } else {
                continue;
            }
        }
        int error_line = input_stream->getEndLine();
        int error_column = input_stream->getEndColumn();
        std::string error_after;
        bool EOFSeen = false;
        try {
            input_stream->readChar();
            input_stream->backup(1);
        } catch (const std::ios_base::failure&) {
            EOFSeen = true;
            error_after = curPos <= 1 ? "" : input_stream->GetImage();
            if (curChar == '\n' || curChar == '\r') {
                error_line++;
                error_column = 0;
            } else
                error_column++;
        }
        if (!EOFSeen) {
            input_stream->backup(1);
            error_after = curPos <= 1 ? "" : input_stream->GetImage();
        }
        throw TokenMgrError(EOFSeen, curLexState, error_line, error_column, error_after, curChar,
                            TokenMgrError::LEXICAL_ERROR);
    }
}

Token DOTParserTokenManager::jjFillToken() {
    std::string curTokenImage = (jjstrLiteralImages[jjmatchedKind] == nullptr) ? input_stream->GetImage() : jjstrLiteralImages[jjmatchedKind];
    Token t = Token::newToken(jjmatchedKind, curTokenImage);
    t.beginLine = input_stream->getBeginLine();
    t.endLine = input_stream->getEndLine();
    t.beginColumn = input_stream->getBeginColumn();
    t.endColumn = input_stream->getEndColumn();
    return t;
}

void DOTParserTokenManager::ReInitRounds() {
    jjround = 0x80000001;
    for (int i = 28; i-- > 0;)
        jjrounds[i] = 0x80000000;
}

void DOTParserTokenManager::jjCheckNAdd(int state) {
    if (jjrounds[state] != jjround) {
        jjstateSet[jjnewStateCnt++] = state;
        jjrounds[state] = jjround;
    }
}

void DOTParserTokenManager::jjAddStates(int start, int end) {
    do {
        jjstateSet[jjnewStateCnt++] = jjnextStates[start];
    } while (start++ != end);
}

void DOTParserTokenManager::jjCheckNAddTwoStates(int state1, int state2) {
    jjCheckNAdd(state1);
    jjCheckNAdd(state2);
}

void DOTParserTokenManager::jjCheckNAddStates(int start, int end) {
    do {
        jjCheckNAdd(jjnextStates[start]);
    } while (start++ != end);
}

} // namespace dot_parser
