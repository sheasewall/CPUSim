#ifndef ALU_H
#define ALU_H

#include <limits>
#include <stdexcept>
#include <bitset>

class ALU {
public:
    ALU() {}
    ~ALU() {}

    static std::bitset<32> add(std::bitset<32> rs1_val, std::bitset<32> rs2_val);
    static std::bitset<32> addImmediate(std::bitset<32> rs1_val, std::bitset<12> imm_val);
    static std::bitset<32> addressOffset(std::bitset<32> address, std::bitset<32> offset);
};

#endif // ALU_H