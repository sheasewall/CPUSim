#ifndef INSTRUCTIONFILE_H
#define INSTRUCTIONFILE_H

#include <unordered_map>
#include <bitset>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class InstructionMemory {
    std::unordered_map<std::bitset<32>, std::bitset<8>> instructions;
public:
    InstructionMemory(std::string instruction_file) {
        std::ifstream file(instruction_file);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open instruction file: " + instruction_file);
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string address;
            std::string instruction;
            if (!(iss >> address >> instruction)) {
                throw std::runtime_error("Error reading line: " + line);
            }
            try {
                std::bitset<32> address_bitset = std::bitset<32>(std::stoul(address));
                std::bitset<8> instruction_byte_bitset = std::bitset<8>(instruction);
                instructions.insert({ address_bitset, instruction_byte_bitset });
            } catch (const std::invalid_argument& e) {
                throw std::runtime_error("Invalid address or instruction in line: " + line);
            } catch (const std::out_of_range& e) {
                throw std::runtime_error("Address or instruction out of range in line: " + line);
            }
        }
    }

    std::bitset<32> read(std::bitset<32> address) {
        // this should be handled with a proper "circuit"
        // I am just being lazy for testing purposes
        long long address_long = address.to_ullong();

        std::bitset<32> instruction = 0;
        for (int i = 0; i < 4; i++) {
            // Little Endian
            std::bitset<8> instr_word = instructions.at(address_long + i);
            instruction |= instr_word.to_ulong() << (i * 8);
        }
        return instruction;
    }
};

#endif // INSTRUCTIONFILE_H