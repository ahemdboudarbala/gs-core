#ifndef DOTPARSER_HPP
#define DOTPARSER_HPP

#include "FileSourceDOT.hpp"
#include "SimpleCharStream.hpp"
#include "Token.hpp"
#include "TokenMgrError.hpp"
#include <iostream>
#include <istream>
#include <map>
#include <set>
#include <string>
#include <vector>

class DOTParser {
public:
    DOTParser(FileSourceDOT* dot, std::istream& stream);
    DOTParser(FileSourceDOT* dot, std::unique_ptr<std::istream>& stream);
    void close();
    void all();
    bool next();
    void open();

private:
    FileSourceDOT* dot;
    std::string sourceId;
    bool directed;
    bool strict;
    std::map<std::string, std::shared_ptr<void>> globalNodesAttributes;
    std::map<std::string, std::shared_ptr<void>> globalEdgesAttributes;
    std::set<std::string> nodeAdded;

    SimpleCharStream jj_input_stream;
    Token* token;
    Token* jj_nt;
    int jj_ntk;
    int jj_gen;
    int jj_la;

    void init(FileSourceDOT* dot);
    void addNode(const std::string& nodeId, const std::vector<std::string>& port, std::map<std::string, std::shared_ptr<void>>* attr);
    void addEdges(std::vector<std::string>& edges, std::map<std::string, std::shared_ptr<void>>* attr);
    void setGlobalAttributes(const std::string& who, std::map<std::string, std::shared_ptr<void>>* attr);
    std::string id();
    void statement();
    void nodeStatement();
    std::string compassPoint();
    std::vector<std::string> port();
    void edgeStatement();
    void edgeRHS(std::vector<std::string>& edges);
    void attributeStatement();
    std::map<std::string, std::shared_ptr<void>> attributesList();
    void attributeList(std::map<std::string, std::shared_ptr<void>>& attributes);
    bool jj_2_1(int xla);
    bool jj_2_2(int xla);
    bool jj_3R_6();
    bool jj_3_2();
    bool jj_3R_8();
    bool jj_3R_10();
    bool jj_3R_7();
    bool jj_3R_9();
    bool jj_3R_5();
    bool jj_3_1();
    Token* jj_consume_token(int kind);
    bool jj_scan_token(int kind);
    void jj_save(int index, int xla);
    void jj_rescan_token();
    void graph();
    void subgraph();
    ParseException generateParseException();
};

#endif // DOTPARSER_HPP
