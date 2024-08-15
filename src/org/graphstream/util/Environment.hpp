#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <iostream>

class Environment {
public:
    static Environment& getGlobalEnvironment();

    Environment clone() const;
    
    bool isLocked() const;
    void lockEnvironment(bool on);

    std::string getParameter(const std::string& parameter) const;
    bool hasParameter(const std::string& parameter) const;
    void setParameter(const std::string& parameter, const std::string& value);

    int getBooleanParameteri(const std::string& parameter) const;
    bool getBooleanParameter(const std::string& parameter) const;
    double getNumberParameter(const std::string& parameter) const;
    size_t getParameterCount() const;
    std::set<std::string> getParametersKeySet() const;

    template <typename T>
    void initializeFieldsOf(T& object, const std::vector<std::string>& fieldList = {});

    void printParameters(std::ostream& out) const;

    void readCommandLine(const std::vector<std::string>& args);
    void readCommandLine(const std::vector<std::string>& args, std::vector<std::string>& trashcan);

    void writeParameterFile(const std::string& fileName) const;
    void readParameterFile(const std::string& fileName);

private:
    Environment();

    std::unordered_map<std::string, std::string> parameters;
    std::string configFileName;
    bool configFileRead;
    bool locked;

    void readConfigFile(const std::string& filename, std::vector<std::string>& trashcan);

    template <typename T>
    void invokeSetMethod(T& object, const std::string& name, const std::string& value);

    static std::string toLower(const std::string& str);
    static std::string trim(const std::string& str);
};

#endif // ENVIRONMENT_HPP
