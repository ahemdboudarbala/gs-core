#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP

#include <chrono>

/**
 * A simple counter that allows to count the number of frames per second.
 */
class FpsCounter {
public:
    // Constructor
    FpsCounter() : time(0), count(0), avgTime(0) {}

    // Access methods

    /**
     * The number of frames per second according to the last measured frame
     * (instantaneous measure).
     * 
     * @return The estimated frame-per-second measure of the last frame.
     */
    double getFramesPerSecond() const {
        return 1000000000.0 / time;
    }

    /**
     * The duration in seconds of the last measured frame.
     * 
     * @return The last frame time in seconds.
     */
    double getLastFrameTimeInSeconds() const {
        return time / 1000000000.0;
    }

    /**
     * The number of frames times used to compute the average frame-per-second and
     * frame time. This number augments with the measures until a maximum, where it
     * is reset to 0.
     * 
     * @return The number of frames measure.
     */
    int getAverageMeasureCount() const {
        return count;
    }

    /**
     * The average frame-per-second measure.
     * 
     * @return The average number of frames per second.
     * @see #getAverageMeasureCount()
     */
    double getAverageFramesPerSecond() const {
        return 1000000000.0 / (avgTime / count);
    }

    /**
     * The average frame time.
     * 
     * @return The time used by a frame in average.
     */
    double getAverageFrameTimeInSeconds() const {
        return (avgTime / count) / 1000000000.0;
    }

    // Command methods

    /**
     * Reset the averages.
     */
    void resetAverages() {
        count = 0;
        avgTime = 0;
    }

    /**
     * Start a frame measure.
     */
    void beginFrame() {
        t1 = std::chrono::high_resolution_clock::now();
    }

    /**
     * End a frame measure.
     */
    void endFrame() {
        if (count > 1000000) {
            resetAverages();
        }

        t2 = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
        avgTime += time;
        count += 1;
    }

private:
    // Attributes
    std::chrono::high_resolution_clock::time_point t1, t2;
    double time;
    int count;
    double avgTime;
};

#endif // FPS_COUNTER_HPP
