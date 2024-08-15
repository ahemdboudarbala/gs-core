#ifndef SIMPLECHARSTREAM_HPP
#define SIMPLECHARSTREAM_HPP

#include <iostream>
#include <vector>
#include <stdexcept>

class SimpleCharStream {
public:
    // Constructors
    SimpleCharStream(std::istream& stream, int startline = 1, int startcolumn = 1, int buffersize = 4096);
    
    // Methods
    char BeginToken();
    char readChar();
    void backup(int amount);
    std::string GetImage();
    std::vector<char> GetSuffix(int len);
    void Done();
    void close();
    void adjustBeginLineColumn(int newLine, int newCol);

    int getColumn() const;
    int getLine() const;
    int getEndColumn() const;
    int getEndLine() const;
    int getBeginColumn() const;
    int getBeginLine() const;

protected:
    void FillBuff();
    void ExpandBuff(bool wrapAround);
    void UpdateLineColumn(char c);

private:
    static const int DEFAULT_TAB_SIZE = 8;

    std::istream& inputStream;
    std::vector<char> buffer;
    std::vector<int> bufline;
    std::vector<int> bufcolumn;

    int bufsize;
    int available;
    int tokenBegin;
    int bufpos = -1;
    int column = 0;
    int line = 1;
    int maxNextCharInd = 0;
    int inBuf = 0;
    int tabSize = DEFAULT_TAB_SIZE;

    bool prevCharIsCR = false;
    bool prevCharIsLF = false;
};

#endif // SIMPLECHARSTREAM_HPP
