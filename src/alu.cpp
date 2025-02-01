#include "alu.h"

std::bitset<32> ALU::add(std::bitset<32> val1, std::bitset<32> val2)
{
    std::bitset<1> carry = 0;
    std::bitset<32> result;
    for (int i = 0; i < 32; i++) {
        result.set(i, val1.test(i) ^ val2.test(i) ^ carry.any());
        carry = (val1.test(i) & val2.test(i)) | (val2.test(i) & carry.any()) | (val1.test(i) & carry.any());
    }
    if (result == 0) {
        
    }
    return result;
}

bool ALU::isEqual(std::bitset<32> val1, std::bitset<32> val2)
{
    return val1 == val2;
}
