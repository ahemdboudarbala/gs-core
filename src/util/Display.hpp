#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <string>
#include <exception>
#include <memory>

class Graph;
class Viewer;

class MissingDisplayException : public std::exception {
public:
    explicit MissingDisplayException(const std::string& message);
    const char* what() const noexcept override;
    
private:
    std::string message;
};

class Display {
public:
    static std::shared_ptr<Display> getDefault();

    virtual std::shared_ptr<Viewer> display(std::shared_ptr<Graph> graph, bool autoLayout) = 0;
    
    virtual ~Display() = default;
};

#endif // DISPLAY_HPP
