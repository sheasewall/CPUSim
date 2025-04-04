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
        long address_long = address.to_ulong();

        std::bitset<32> instruction = 0;
        for (int i = 0; i < 4; i++) {
            // Little Endian
            std::bitset<8> instr_word = data.at(address_long + i);
            instruction |= instr_word.to_ulong() << (i * 8);
        }
        return instruction;
    }

    void print() {
        for (int i = 0; i < data.size(); i += 4) {
            // Collect the instruction bytes
            std::bitset<32> instruction = 0;
            for (int j = 0; j < 4; j++) {
                std::bitset<8> instr_word = data.at(i + j);
                instruction |= instr_word.to_ulong() << (j * 8);
            }
            std::cout << "Instruction " << std::setw(8) << std::setfill('0') << std::dec << i << ": " << std::setw(8) << std::setfill('0') << std::hex << instruction.to_ulong() << std::endl;
        }
    }

    void dump() {
        File::dump(1, "instr");
    }

    void dump(std::bitset<32> pc) {
        InstructionFile::dump();
        std::ofstream dump_file("instr.dump", std::ios::app);
        u_long value = pc.to_ulong();
        dump_file.write(reinterpret_cast<const char*>(&value), 4);
    }
};

#endif // INSTRUCTIONFILE_H