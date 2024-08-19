#ifndef OLDFILESOURCEDGS_HPP
#define OLDFILESOURCEDGS_HPP

#include "FileSourceBase.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <memory>

class OldFileSourceDGS : public FileSourceBase {
public:
    OldFileSourceDGS();
    virtual bool nextEvents() override;
    bool nextStep();
    virtual void begin(const std::string& filename) override;
    virtual void begin(std::istream& stream) override;
    
protected:
    virtual std::unique_ptr<std::istream> createReaderFrom(const std::string& file) override;
    virtual std::unique_ptr<std::istream> createReaderFrom(std::istream& stream) override;
    virtual void configureTokenizer(std::istringstream& tokenizer);

private:
    int version;
    std::string graphName;
    int stepCountAnnounced;
    int eventCountAnnounced;
    int stepCount;
    int eventCount;
    std::map<std::string, std::shared_ptr<void>> attributes;
    bool finished;

    bool next(bool readSteps, bool stop);
    void readCE();
    void readCN();
    void readCG();
    void readAE();
    void readAN();
    void readDE();
    void readDN();
    void readST();
    void readAttributes(std::map<std::string, std::shared_ptr<void>>& attributes);
    std::shared_ptr<void> readAttributeValue(const std::string& key);
    std::vector<std::shared_ptr<void>> readAttributeArray(const std::string& key);
    void beginParsing();
};

#endif // OLDFILESOURCEDGS_HPP
