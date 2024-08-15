#ifndef FILE_SINK_BASE_FILTERED_HPP
#define FILE_SINK_BASE_FILTERED_HPP

#include "FileSinkBase.hpp"
#include <vector>
#include <string>

class FileSinkBaseFiltered : public FileSinkBase {
public:
    FileSinkBaseFiltered();
    virtual ~FileSinkBaseFiltered() = default;

    // Getters and setters for filters
    std::vector<std::string> getGraphAttributesFiltered() const;
    void setGraphAttributesFiltered(const std::vector<std::string>& graphAttributesFiltered);
    bool addGraphAttributeFiltered(const std::string& attr);
    bool removeGraphAttributeFilter(const std::string& attr);

    std::vector<std::string> getNodeAttributesFiltered() const;
    void setNodeAttributesFiltered(const std::vector<std::string>& nodeAttributesFiltered);
    bool addNodeAttributeFiltered(const std::string& attr);
    bool removeNodeAttributeFilter(const std::string& attr);

    std::vector<std::string> getEdgeAttributesFiltered() const;
    void setEdgeAttributesFiltered(const std::vector<std::string>& edgeAttributesFiltered);
    bool addEdgeAttributeFiltered(const std::string& attr);
    bool removeEdgeAttributeFilter(const std::string& attr);

    bool isNoFilterGraphAttributeAdded() const;
    void setNoFilterGraphAttributeAdded(bool noFilter);

    bool isNoFilterGraphAttributeChanged() const;
    void setNoFilterGraphAttributeChanged(bool noFilter);

    bool isNoFilterGraphAttributeRemoved() const;
    void setNoFilterGraphAttributeRemoved(bool noFilter);

    bool isNoFilterNodeAttributeAdded() const;
    void setNoFilterNodeAttributeAdded(bool noFilter);

    bool isNoFilterNodeAttributeChanged() const;
    void setNoFilterNodeAttributeChanged(bool noFilter);

    bool isNoFilterNodeAttributeRemoved() const;
    void setNoFilterNodeAttributeRemoved(bool noFilter);

    bool isNoFilterNodeAdded() const;
    void setNoFilterNodeAdded(bool noFilter);

    bool isNoFilterNodeRemoved() const;
    void setNoFilterNodeRemoved(bool noFilter);

    bool isNoFilterEdgeAttributeAdded() const;
    void setNoFilterEdgeAttributeAdded(bool noFilter);

    bool isNoFilterEdgeAttributeChanged() const;
    void setNoFilterEdgeAttributeChanged(bool noFilter);

    bool isNoFilterEdgeAttributeRemoved() const;
    void setNoFilterEdgeAttributeRemoved(bool noFilter);

    bool isNoFilterEdgeAdded() const;
    void setNoFilterEdgeAdded(bool noFilter);

    bool isNoFilterEdgeRemoved() const;
    void setNoFilterEdgeRemoved(bool noFilter);

    bool isNoFilterGraphCleared() const;
    void setNoFilterGraphCleared(bool noFilter);

    bool isNoFilterStepBegins() const;
    void setNoFilterStepBegins(bool noFilter);

protected:
    // Attributes to hold filter settings
    bool noFilterGraphAttributeAdded;
    bool noFilterGraphAttributeChanged;
    bool noFilterGraphAttributeRemoved;
    bool noFilterNodeAttributeAdded;
    bool noFilterNodeAttributeChanged;
    bool noFilterNodeAttributeRemoved;
    bool noFilterNodeAdded;
    bool noFilterNodeRemoved;
    bool noFilterEdgeAttributeAdded;
    bool noFilterEdgeAttributeChanged;
    bool noFilterEdgeAttributeRemoved;
    bool noFilterEdgeAdded;
    bool noFilterEdgeRemoved;
    bool noFilterGraphCleared;
    bool noFilterStepBegins;

    std::vector<std::string> graphAttributesFiltered;
    std::vector<std::string> nodeAttributesFiltered;
    std::vector<std::string> edgeAttributesFiltered;
};

#endif // FILE_SINK_BASE_FILTERED_HPP
