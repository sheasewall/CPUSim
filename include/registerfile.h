#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include "file.hpp"

#include <bitset>
#include <iostream>
#include <map>

class RegisterFile : public File<5, 32> {
public:
    RegisterFile(std::string _memory_file = "reg") : File(_memory_file) {
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
        for (auto& datum : data) {
            std::cout << "Register " << std::setw(2) << std::setfill(' ') << std::dec << datum.first.to_ulong() << ": " << std::setw(8) << std::setfill('0') << std::hex << datum.second.to_ulong() << std::endl;
        }
    }

    void dump() {
        File::dump(4, "reg");
    }
};

#endif // REGISTERFILE_H