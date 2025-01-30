#ifndef INSTRUCTIONFILE_H
#define INSTRUCTIONFILE_H

#include <unordered_map>
#include <bitset>
#include <string>
#include <fstream>
#include <sstream>

class InstructionMemory {
    std::unordered_map<std::bitset<32>, std::bitset<32>> instructions;
public:
    InstructionMemory(std::string instruction_file) {
        std::ifstream file(instruction_file);
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string address;
            std::string instruction;
            iss >> address >> instruction;
            instructions.insert({ std::bitset<32>(std::stoi(address)), std::bitset<32>(std::stoi(instruction)) });
        }
    }

    std::bitset<32> read(std::bitset<32> address) {
        return instructions.at(address);
    }
};

#endif // INSTRUCTIONFILE_H