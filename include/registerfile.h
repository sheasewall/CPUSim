#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include "file.hpp"

#include <bitset>
#include <iostream>
#include <map>

class RegisterFile : public File<5, 32> {
public:
    RegisterFile(std::string _memory_file) : File(_memory_file) {
        for (int i = 0; i < 32; i++) {
            data.insert({ std::bitset<5>(i), std::bitset<32>(0) });
        }
    }

    std::pair<std::bitset<32>, std::bitset<32>> read(std::bitset<5> reg1, std::bitset<5> reg2) {
        return std::pair<std::bitset<32>, std::bitset<32>>{ data.at(reg1), data.at(reg2) };
    }

    void write(std::bitset<5> reg, std::bitset<32> value) {
        if (reg == std::bitset<5>(0)) {
            return;
        }
        data.at(reg) = value;
    }

    void print() {
        File::print("Register ");
    }
};

#endif // REGISTERFILE_H