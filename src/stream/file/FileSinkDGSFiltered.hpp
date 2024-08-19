#ifndef FILE_SINK_DGS_FILTERED_HPP
#define FILE_SINK_DGS_FILTERED_HPP

#include "FileSinkBaseFiltered.hpp"
#include <iostream>
#include <string>
#include <unordered_set>
#include <locale>

class FileSinkDGSFiltered : public FileSinkBaseFiltered {
public:
    FileSinkDGSFiltered();
    virtual ~FileSinkDGSFiltered() = default;

    // Overridden methods from FileSinkBaseFiltered
    virtual void edgeAttributeAdded(const std::string& graphId, long timeId, const std::string& edgeId,
                                    const std::string& attribute, const std::string& value) override;
    virtual void edgeAttributeChanged(const std::string& graphId, long timeId, const std::string& edgeId,
                                      const std::string& attribute, const std::string& oldValue,
                                      const std::string& newValue) override;
    virtual void edgeAttributeRemoved(const std::string& graphId, long timeId, const std::string& edgeId,
                                      const std::string& attribute) override;

    virtual void graphAttributeAdded(const std::string& graphId, long timeId, const std::string& attribute,
                                     const std::string& value) override;
    virtual void graphAttributeChanged(const std::string& graphId, long timeId, const std::string& attribute,
                                       const std::string& oldValue, const std::string& newValue) override;
    virtual void graphAttributeRemoved(const std::string& graphId, long timeId, const std::string& attribute) override;

    virtual void nodeAttributeAdded(const std::string& graphId, long timeId, const std::string& nodeId,
                                    const std::string& attribute, const std::string& value) override;
    virtual void nodeAttributeChanged(const std::string& graphId, long timeId, const std::string& nodeId,
                                      const std::string& attribute, const std::string& oldValue,
                                      const std::string& newValue) override;
    virtual void nodeAttributeRemoved(const std::string& graphId, long timeId, const std::string& nodeId,
                                      const std::string& attribute) override;

    virtual void edgeAdded(const std::string& graphId, long timeId, const std::string& edgeId,
                           const std::string& fromNodeId, const std::string& toNodeId, bool directed) override;
    virtual void edgeRemoved(const std::string& graphId, long timeId, const std::string& edgeId) override;

    virtual void graphCleared(const std::string& graphId, long timeId) override;

    virtual void nodeAdded(const std::string& graphId, long timeId, const std::string& nodeId) override;
    virtual void nodeRemoved(const std::string& graphId, long timeId, const std::string& nodeId) override;

    virtual void stepBegins(const std::string& graphId, long timeId, double step) override;

protected:
    virtual void outputHeader() override;
    virtual void outputEndOfFile() override;

private:
    std::ostream* out;
    std::string graphName;
};

#endif // FILE_SINK_DGS_FILTERED_HPP
