#ifndef MEMORYFILE_H
#define MEMORYFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class DataMemory {
    std::unordered_map<std::bitset<32>, std::bitset<8>> data;

public:
    DataMemory(std::string memory_file) {
        std::ifstream file(memory_file);
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string address;
            std::string value;
            iss >> address >> value;
            data.insert({ std::bitset<32>(std::stoi(address)), std::bitset<8>(std::stoi(value)) });
        }
    }

    std::bitset<8> read(std::bitset<32> address) {
        return data.at(address);
    }

    void write(std::bitset<32> address, std::bitset<8> value) {
        data.at(address) = value;
    }
};

#endif // MEMORYFILE_H