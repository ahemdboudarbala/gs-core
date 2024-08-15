#include "Environment.hpp"
#include <fstream>
#include <algorithm>
#include <stdexcept>

Environment& Environment::getGlobalEnvironment() {
    static Environment GLOBAL_ENV;
    return GLOBAL_ENV;
}

Environment::Environment() : configFileName("config"), configFileRead(false), locked(false) {}

Environment Environment::clone() const {
    Environment e;
    e.configFileName = configFileName;
    e.configFileRead = configFileRead;
    e.locked = locked;
    e.parameters = parameters;
    return e;
}

bool Environment::isLocked() const {
    return locked;
}

void Environment::lockEnvironment(bool on) {
    locked = on;
}

std::string Environment::getParameter(const std::string& parameter) const {
    auto it = parameters.find(parameter);
    return (it != parameters.end()) ? it->second : "";
}

bool Environment::hasParameter(const std::string& parameter) const {
    return parameters.find(parameter) != parameters.end();
}

void Environment::setParameter(const std::string& parameter, const std::string& value) {
    if (!locked || parameters.find(parameter) != parameters.end()) {
        parameters[parameter] = value;
    }
}

int Environment::getBooleanParameteri(const std::string& parameter) const {
    auto it = parameters.find(parameter);
    if (it != parameters.end()) {
        std::string p = toLower(it->second);
        if (p == "1" || p == "true" || p == "on" || p == "yes") {
            return 1;
        }
        return 0;
    }
    return -1;
}

bool Environment::getBooleanParameter(const std::string& parameter) const {
    return getBooleanParameteri(parameter) == 1;
}

double Environment::getNumberParameter(const std::string& parameter) const {
    try {
        return std::stod(getParameter(parameter));
    } catch (std::exception&) {
        return 0.0;
    }
}

size_t Environment::getParameterCount() const {
    return parameters.size();
}

std::set<std::string> Environment::getParametersKeySet() const {
    std::set<std::string> keys;
    for (const auto& kv : parameters) {
        keys.insert(kv.first);
    }
    return keys;
}

void Environment::printParameters(std::ostream& out) const {
    for (const auto& kv : parameters) {
        out << kv.first << " = " << kv.second << std::endl;
    }
}

void Environment::readCommandLine(const std::vector<std::string>& args) {
    readCommandLine(args, {});
}

void Environment::readCommandLine(const std::vector<std::string>& args, std::vector<std::string>& trashcan) {
    for (const auto& arg : args) {
        bool startsWithMinus = arg[0] == '-';
        size_t equalPos = arg.find('=');
        std::string value = "true";

        if (equalPos != std::string::npos) {
            value = arg.substr(equalPos + 1);
            if (startsWithMinus) {
                parameters[arg.substr(1, equalPos - 1)] = value;
            } else {
                parameters[arg.substr(0, equalPos)] = value;
            }
        } else {
            if (startsWithMinus) {
                parameters[arg.substr(1)] = value;
            } else {
                readConfigFile(arg, trashcan);
            }
        }
    }
}

void Environment::writeParameterFile(const std::string& fileName) const {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + fileName);
    }
    for (const auto& kv : parameters) {
        file << kv.first << " = " << kv.second << std::endl;
    }
    file.close();
}

void Environment::readParameterFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for reading: " + fileName);
    }

    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        count++;
        if (!line.empty() && line[0] != '#') {
            size_t equalPos = line.find('=');
            if (equalPos != std::string::npos) {
                std::string key = trim(line.substr(0, equalPos));
                std::string value = trim(line.substr(equalPos + 1));
                parameters[key] = value;
            } else {
                throw std::runtime_error("Invalid format in configuration file at line " + std::to_string(count));
            }
        }
    }

    file.close();
}

void Environment::readConfigFile(const std::string& filename, std::vector<std::string>& trashcan) {
    try {
        readParameterFile(filename);
    } catch (std::exception& e) {
        trashcan.push_back(filename);
    }
}

std::string Environment::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string Environment::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

template <typename T>
void Environment::invokeSetMethod(T& object, const std::string& name, const std::string& value) {
    // Implement the logic for invoking setter methods on the object
    // This is a placeholder example and should be tailored to your specific use case
    auto setterName = "set" + name;
    std::transform(setterName.begin() + 3, setterName.end(), setterName.begin() + 3, ::toupper);

    // Example of invoking the setter
    if (setterName == "setConfigFileName") {
        object.setConfigFileName(value);
    } else if (setterName == "setBooleanValue") {
        object.setBooleanValue(getBooleanParameter(name));
    } else if (setterName == "setIntValue") {
        object.setIntValue(std::stoi(value));
    } else if (setterName == "setDoubleValue") {
        object.setDoubleValue(std::stod(value));
    } else if (setterName == "setStringValue") {
        object.setStringValue(value);
    } else {
        throw std::runtime_error("No matching setter method found for " + name);
    }
}

#endif // ENVIRONMENT_HPP
