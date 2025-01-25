#ifndef FILEIOUNIT_HPP
#define FILEIOUNIT_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

class FileIOUnit {
public:
    template <typename T>
    static void writeLineToFile(const std::string& filename, unsigned int lineNumber, T value);
    static void clearLineFromFile(const std::string& filename, unsigned int lineNumber);
    static int readLineFromFile(const std::string& filename, unsigned int lineNumber);

private:
    static void checkFileOpen(const std::string& filename, std::fstream& file);
};

///

inline void FileIOUnit::checkFileOpen(const std::string& filename, std::fstream& file) {
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open " + filename);
    }
}

inline void FileIOUnit::clearLineFromFile(const std::string& filename, unsigned int lineNumber) {
    std::fstream file(filename);
    checkFileOpen(filename, file);

    std::stringstream buffer;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        unsigned int currentLineNumber;
        iss >> currentLineNumber;

        if (currentLineNumber == lineNumber) {
        } else {
            buffer << line << "\n";
        }
    }

    file.close();
    file.open(filename, std::ios::out | std::ios::trunc);
    checkFileOpen(filename, file);
    file << buffer.str();
}

template <typename T>
inline void FileIOUnit::writeLineToFile(const std::string& filename, unsigned int lineNumber, T value) {
    std::fstream file(filename);
    checkFileOpen(filename, file);

    std::stringstream buffer;
    bool found = false;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        unsigned int currentLineNumber;
        iss >> currentLineNumber;

        if (currentLineNumber == lineNumber) {
            buffer << lineNumber << " " << value << "\n";
            found = true;
        } else {
            buffer << line << "\n";
        }
    }

    if (!found) {
        buffer << lineNumber << " " << value << "\n";
    }

    file.close();
    file.open(filename, std::ios::out | std::ios::trunc);
    checkFileOpen(filename, file);
    file << buffer.str();
}

inline int FileIOUnit::readLineFromFile(const std::string& filename, unsigned int lineNumber) {
    std::fstream file(filename);
    checkFileOpen(filename, file);

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        unsigned int currentLineNumber;
        iss >> currentLineNumber;

        if (currentLineNumber == lineNumber) {
            int value;
            iss >> value;
            return value;
        }
    }

    throw std::out_of_range("Memory address not found: " + std::to_string(lineNumber));
}

#endif // FILEIOUNIT_HPP