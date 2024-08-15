#ifndef VIEWER_LISTENER_HPP
#define VIEWER_LISTENER_HPP

#include <string>

class ViewerListener {
public:
    virtual ~ViewerListener() = default;

    virtual void viewClosed(const std::string& viewName) = 0;
    virtual void buttonPushed(const std::string& id) = 0;
    virtual void buttonReleased(const std::string& id) = 0;
    virtual void mouseOver(const std::string& id) = 0;
    virtual void mouseLeft(const std::string& id) = 0;
};

#endif // VIEWER_LISTENER_HPP
