#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <bitset>
#include <iostream>
#include <unordered_map>

class RegisterFile {
    std::unordered_map<std::bitset<5>, std::bitset<32>> registers;

public:
    RegisterFile() {
        for (int i = 0; i < 32; i++) {
            registers.insert({ std::bitset<5>(i), std::bitset<32>(0) });
        }
    }

    std::pair<std::bitset<32>, std::bitset<32>> read(std::bitset<5> reg1, std::bitset<5> reg2) {
        return std::pair<std::bitset<32>, std::bitset<32>>{ registers.at(reg1), registers.at(reg2) };
    }

    void write(std::bitset<5> reg, std::bitset<32> value) {
        if (reg == std::bitset<5>(0)) {
            return;
        }
        registers.at(reg) = value;
    }

    void print() {
        for (auto& reg : registers) {
            std::cout << "Register " << reg.first.to_ulong() << ": " << reg.second << std::endl;
        }
    }
};

#endif // REGISTERFILE_H