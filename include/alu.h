#ifndef ALU_H
#define ALU_H

#include <limits>
#include <stdexcept>
#include <bitset>

class ALU {
public:
    std::bitset<1> zero;

    ALU() {}
    ~ALU() {}

    static std::bitset<32> add(std::bitset<32> val1, std::bitset<32> val2);
    static bool isEqual(std::bitset<32> val1, std::bitset<32> val2);
};

#endif // ALU_H