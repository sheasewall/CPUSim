#ifndef INSTRUCTIONFILE_H
#define INSTRUCTIONFILE_H

#include "file.hpp"

#include <map>
#include <bitset>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class InstructionFile : public File<32, 8> {
public:
    InstructionFile(std::string _memory_file) : File(_memory_file) {}

    std::bitset<32> read(std::bitset<32> address) {
    // this should be handled with a proper "circuit"
    // I am just being lazy for testing purposes
        long long address_long = address.to_ullong();

        std::bitset<32> instruction = 0;
        for (int i = 0; i < 4; i++) {
            // Little Endian
            std::bitset<8> instr_word = data.at(address_long + i);
            instruction |= instr_word.to_ulong() << (i * 8);
        }
        return instruction;
    }

    void print() {
        File::print("Instruction ");
    }
};

#endif // INSTRUCTIONFILE_H