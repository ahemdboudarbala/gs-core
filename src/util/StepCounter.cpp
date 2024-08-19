#include "StepCounter.hpp"

// Constructor
StepCounter::StepCounter() : step(0) {}

// Static method to count the number of steps in a file
int StepCounter::countStepInFile(const std::string& path) {
    StepCounter counter;
    FileSource* source = FileSourceFactory::sourceFor(path);

    if (!source) {
        std::cerr << "Error: Unable to create FileSource for the given path." << std::endl;
        return -1; // Return -1 or handle the error as appropriate
    }

    source->addElementSink(&counter);
    source->readAll(path);

    delete source; // Clean up the FileSource object

    return counter.getStepCount();
}

// Reset the step count to zero
void StepCounter::reset() {
    step = 0;
}

// Get the step count
int StepCounter::getStepCount() const {
    return step;
}

// Override the stepBegins method from SinkAdapter
void StepCounter::stepBegins(const std::string& sourceId, long timeId, double time) {
    step++;
}
