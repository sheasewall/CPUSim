#ifndef ALU_H
#define ALU_H

#include <limits>
#include <stdexcept>
#include <bitset>

class ALU {
public:
    ALU() {}
    ~ALU() {}

    static std::bitset<32> add(std::bitset<32> val1, std::bitset<32> val2);
    static std::bitset<32> addressOffset(std::bitset<32> address, std::bitset<32> offset);
};

#endif // ALU_H