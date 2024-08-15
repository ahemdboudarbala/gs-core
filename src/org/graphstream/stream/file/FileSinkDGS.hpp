#ifndef FILE_SINK_DGS_HPP
#define FILE_SINK_DGS_HPP

#include "FileSinkBase.hpp"
#include <iostream>
#include <string>
#include <locale>

class FileSinkDGS : public FileSinkBase {
public:
    FileSinkDGS();
    virtual ~FileSinkDGS() = default;

    // Methods to override from FileSinkBase
    virtual void edgeAttributeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute, const std::string& value);
    virtual void edgeAttributeChanged(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute, const std::string& oldValue, const std::string& newValue);
    virtual void edgeAttributeRemoved(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& attribute);

    virtual void graphAttributeAdded(const std::string& graphId, long timeId, const std::string& attribute, const std::string& value);
    virtual void graphAttributeChanged(const std::string& graphId, long timeId, const std::string& attribute, const std::string& oldValue, const std::string& newValue);
    virtual void graphAttributeRemoved(const std::string& graphId, long timeId, const std::string& attribute);

    virtual void nodeAttributeAdded(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute, const std::string& value);
    virtual void nodeAttributeChanged(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute, const std::string& oldValue, const std::string& newValue);
    virtual void nodeAttributeRemoved(const std::string& graphId, long timeId, const std::string& nodeId, const std::string& attribute);

    virtual void edgeAdded(const std::string& graphId, long timeId, const std::string& edgeId, const std::string& fromNodeId, const std::string& toNodeId, bool directed);
    virtual void edgeRemoved(const std::string& graphId, long timeId, const std::string& edgeId);

    virtual void graphCleared(const std::string& graphId, long timeId);

    virtual void nodeAdded(const std::string& graphId, long timeId, const std::string& nodeId);
    virtual void nodeRemoved(const std::string& graphId, long timeId, const std::string& nodeId);

    virtual void stepBegins(const std::string& graphId, long timeId, double step);

protected:
    virtual void outputHeader() override;
    virtual void outputEndOfFile() override;

private:
    std::ostream* out;
    std::string graphName;
};

#endif // FILE_SINK_DGS_HPP
