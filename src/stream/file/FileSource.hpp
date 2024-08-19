#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <memory>

class FileSource {
public:
    virtual ~FileSource() = default;

    /**
     * Read the whole file in one big non-interruptible operation.
     *
     * @param fileName Name of the file to read.
     * @throws std::ios_base::failure If an I/O error occurs while reading.
     */
    virtual void readAll(const std::string& fileName) = 0;

    /**
     * Read the whole file in one big non-interruptible operation.
     *
     * @param url The URL of the file to read.
     * @throws std::ios_base::failure If an I/O error occurs while reading.
     */
    virtual void readAll(const std::string& url) = 0;

    /**
     * Read the whole file in one big non-interruptible operation.
     *
     * @param stream The input stream to use for reading.
     * @throws std::ios_base::failure If an I/O error occurs while reading.
     */
    virtual void readAll(std::istream& stream) = 0;

    /**
     * Read the whole file in one big non-interruptible operation.
     *
     * @param reader The reader to use.
     * @throws std::ios_base::failure If an I/O error occurs while reading.
     */
    virtual void readAll(std::ifstream& reader) = 0;

    /**
     * Begin reading the file stopping as soon as possible.
     * Once begin() has been called, you must finish the reading process using end().
     *
     * @param fileName Name of the file to read.
     * @throws std::ios_base::failure If an I/O error occurs while reading.
     */
    virtual void begin(const std::string& fileName) = 0;

    /**
     * Begin reading the file stopping as soon as possible.
     * Once begin() has been called, you must finish the reading process using end().
     *
     * @param url The URL of the file to read.
     * @throws std::ios_base::failure If an I/O error occurs while reading.
     */
    virtual void begin(const std::string& url) = 0;

    /**
     * Begin reading the file stopping as soon as possible.
     * Once begin() has been called, you must finish the reading process using end().
     *
     * @param stream The input stream to use for reading.
     * @throws std::ios_base::failure If an I/O error occurs while reading.
     */
    virtual void begin(std::istream& stream) = 0;

    /**
     * Begin reading the file stopping as soon as possible.
     * Once begin() has been called, you must finish the reading process using end().
     *
     * @param reader The reader to use.
     * @throws std::ios_base::failure If an I/O error occurs while reading.
     */
    virtual void begin(std::ifstream& reader) = 0;

    /**
     * Try to process one graph event, or as few as possible.
     * This method returns true while there are still events to read.
     *
     * @return true if there are still events to read, false as soon as the file is finished.
     * @throws std::ios_base::failure If an I/O error occurs while reading.
     */
    virtual bool nextEvents() = 0;

    /**
     * Try to process all the events occurring during one time step.
     * This method returns true while there are still events to read.
     *
     * @return true if there are still events to read, false as soon as the file is finished.
     * @throws std::ios_base::failure If an I/O error occurs while reading.
     */
    virtual bool nextStep() = 0;

    /**
     * Finish the reading process.
     * You must call this method after reading.
     *
     * @throws std::ios_base::failure If an I/O error occurs while closing the file.
     */
    virtual void end() = 0;
};
