#ifndef COMPOUND_ATTRIBUTE_HPP
#define COMPOUND_ATTRIBUTE_HPP

#include <string>
#include <unordered_map>
#include <memory>

class CompoundAttribute {
public:
    // Virtual destructor for interface
    virtual ~CompoundAttribute() = default;

    // Method to convert the object to a hash map
    virtual std::unordered_map<std::string, std::shared_ptr<void>> toHashMap() const = 0;

    // Method to get the key used to store this attribute
    virtual std::string getKey() const = 0;
};

#endif // COMPOUND_ATTRIBUTE_HPP
