#include "SimpleCharStream.hpp"

// Constructor
SimpleCharStream::SimpleCharStream(std::istream& stream, int startline, int startcolumn, int buffersize)
    : inputStream(stream), line(startline), column(startcolumn - 1), bufsize(buffersize), available(buffersize) {
    buffer.resize(buffersize);
    bufline.resize(buffersize);
    bufcolumn.resize(buffersize);
}

// Start token
char SimpleCharStream::BeginToken() {
    tokenBegin = -1;
    char c = readChar();
    tokenBegin = bufpos;
    return c;
}

// Fill the buffer with more characters from the input stream
void SimpleCharStream::FillBuff() {
    if (maxNextCharInd == available) {
        if (available == bufsize) {
            if (tokenBegin > 2048) {
                bufpos = maxNextCharInd = 0;
                available = tokenBegin;
            } else if (tokenBegin < 0) {
                bufpos = maxNextCharInd = 0;
            } else {
                ExpandBuff(false);
            }
        } else if (available > tokenBegin) {
            available = bufsize;
        } else if ((tokenBegin - available) < 2048) {
            ExpandBuff(true);
        } else {
            available = tokenBegin;
        }
    }

    int i = inputStream.readsome(&buffer[maxNextCharInd], available - maxNextCharInd);
    if (i == 0) {
        throw std::ios_base::failure("End of stream");
    } else {
        maxNextCharInd += i;
    }
}

// Read a character from the buffer
char SimpleCharStream::readChar() {
    if (inBuf > 0) {
        --inBuf;
        if (++bufpos == bufsize) {
            bufpos = 0;
        }
        return buffer[bufpos];
    }

    if (++bufpos >= maxNextCharInd) {
        FillBuff();
    }

    char c = buffer[bufpos];
    UpdateLineColumn(c);
    return c;
}

// Backup the buffer by a specified amount
void SimpleCharStream::backup(int amount) {
    inBuf += amount;
    if ((bufpos -= amount) < 0) {
        bufpos += bufsize;
    }
}

// Get the image of the token
std::string SimpleCharStream::GetImage() {
    if (bufpos >= tokenBegin) {
        return std::string(buffer.begin() + tokenBegin, buffer.begin() + bufpos + 1);
    } else {
        return std::string(buffer.begin() + tokenBegin, buffer.end()) + std::string(buffer.begin(), buffer.begin() + bufpos + 1);
    }
}

// Get the suffix of the specified length
std::vector<char> SimpleCharStream::GetSuffix(int len) {
    std::vector<char> ret(len);

    if ((bufpos + 1) >= len) {
        std::copy(buffer.begin() + bufpos - len + 1, buffer.begin() + bufpos + 1, ret.begin());
    } else {
        std::copy(buffer.begin() + bufsize - (len - bufpos - 1), buffer.end(), ret.begin());
        std::copy(buffer.begin(), buffer.begin() + bufpos + 1, ret.begin() + (len - bufpos - 1));
    }

    return ret;
}

// Finalize and reset buffer
void SimpleCharStream::Done() {
    buffer.clear();
    bufline.clear();
    bufcolumn.clear();
}

// Close the input stream
void SimpleCharStream::close() {
    // No explicit close needed for std::istream in C++
}

// Adjust line and column numbers
void SimpleCharStream::adjustBeginLineColumn(int newLine, int newCol) {
    int start = tokenBegin;
    int len;

    if (bufpos >= tokenBegin) {
        len = bufpos - tokenBegin + inBuf + 1;
    } else {
        len = bufsize - tokenBegin + bufpos + 1 + inBuf;
    }

    int i = 0, j = 0, k = 0;
    int nextColDiff = 0, columnDiff = 0;

    while (i < len && bufline[j = start % bufsize] == bufline[k = ++start % bufsize]) {
        bufline[j] = newLine;
        nextColDiff = columnDiff + bufcolumn[k] - bufcolumn[j];
        bufcolumn[j] = newCol + columnDiff;
        columnDiff = nextColDiff;
        i++;
    }

    if (i < len) {
        bufline[j] = newLine++;
        bufcolumn[j] = newCol + columnDiff;

        while (i++ < len) {
            if (bufline[j = start % bufsize] != bufline[++start % bufsize]) {
                bufline[j] = newLine++;
            } else {
                bufline[j] = newLine;
            }
        }
    }

    line = bufline[j];
    column = bufcolumn[j];
}

// Expand the buffer if needed
void SimpleCharStream::ExpandBuff(bool wrapAround) {
    std::vector<char> newbuffer(bufsize + 2048);
    std::vector<int> newbufline(bufsize + 2048);
    std::vector<int> newbufcolumn(bufsize + 2048);

    if (wrapAround) {
        std::copy(buffer.begin() + tokenBegin, buffer.begin() + bufsize, newbuffer.begin());
        std::copy(buffer.begin(), buffer.begin() + bufpos, newbuffer.begin() + (bufsize - tokenBegin));
        buffer = std::move(newbuffer);

        std::copy(bufline.begin() + tokenBegin, bufline.begin() + bufsize, newbufline.begin());
        std::copy(bufline.begin(), bufline.begin() + bufpos, newbufline.begin() + (bufsize - tokenBegin));
        bufline = std::move(newbufline);

        std::copy(bufcolumn.begin() + tokenBegin, bufcolumn.begin() + bufsize, newbufcolumn.begin());
        std::copy(bufcolumn.begin(), bufcolumn.begin() + bufpos, newbufcolumn.begin() + (bufsize - tokenBegin));
        bufcolumn = std::move(newbufcolumn);

        maxNextCharInd = (bufpos += (bufsize - tokenBegin));
    } else {
        std::copy(buffer.begin() + tokenBegin, buffer.begin() + bufsize, newbuffer.begin());
        buffer = std::move(newbuffer);

        std::copy(bufline.begin() + tokenBegin, bufline.begin() + bufsize, newbufline.begin());
        bufline = std::move(newbufline);

        std::copy(bufcolumn.begin() + tokenBegin, bufcolumn.begin() + bufsize, newbufcolumn.begin());
        bufcolumn = std::move(newbufcolumn);

        maxNextCharInd = (bufpos -= tokenBegin);
    }

    bufsize += 2048;
    available = bufsize;
    tokenBegin = 0;
}

// Update line and column number based on the character read
void SimpleCharStream::UpdateLineColumn(char c) {
    column++;

    if (prevCharIsLF) {
        prevCharIsLF = false;
        line += (column = 1);
    } else if (prevCharIsCR) {
        prevCharIsCR = false;
        if (c == '\n') {
            prevCharIsLF = true;
        } else {
            line += (column = 1);
        }
    }

    switch (c) {
    case '\r':
        prevCharIsCR = true;
        break;
    case '\n':
        prevCharIsLF = true;
        break;
    case '\t':
        column--;
        column += (tabSize - (column % tabSize));
        break;
    default:
        break;
    }

    bufline[bufpos] = line;
    bufcolumn[bufpos] = column;
}

// Accessor methods for line and column numbers
int SimpleCharStream::getColumn() const {
    return bufcolumn[bufpos];
}

int SimpleCharStream::getLine() const {
    return bufline[bufpos];
}

int SimpleCharStream::getEndColumn() const {
    return bufcolumn[bufpos];
}

int SimpleCharStream::getEndLine() const {
    return bufline[bufpos];
}

int SimpleCharStream::getBeginColumn() const {
    return bufcolumn[tokenBegin];
}

int SimpleCharStream::getBeginLine() const {
    return bufline[tokenBegin];
}
