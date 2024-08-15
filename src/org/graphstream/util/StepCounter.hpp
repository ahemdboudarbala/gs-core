#pragma once

#include <string>
#include <iostream>
#include "SinkAdapter.hpp"
#include "FileSource.hpp"
#include "FileSourceFactory.hpp"

class StepCounter : public SinkAdapter {
public:
    // Constructor
    StepCounter();

    // Static method to count the number of steps in a file
    static int countStepInFile(const std::string& path);

    // Reset the step count to zero
    void reset();

    // Get the step count
    int getStepCount() const;

    // Override the stepBegins method from SinkAdapter
    void stepBegins(const std::string& sourceId, long timeId, double time) override;

private:
    int step;
};
