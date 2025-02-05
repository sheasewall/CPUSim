#include "alu.h"

std::bitset<32> ALU::bitwiseNot(std::bitset<32> val1)
{
    return hardwareNor(val1, val1);
}

std::bitset<32> ALU::negate(std::bitset<32> val)
{
    return add(bitwiseNot(val), std::bitset<32>(1));
}


std::bitset<32> ALU::bitwiseOr(std::bitset<32> val1, std::bitset<32> val2)
{
    return bitwiseNot(hardwareNor(val1, val2));
}

std::bitset<32> ALU::bitwiseAnd(std::bitset<32> val1, std::bitset<32> val2)
{
    return hardwareNor(bitwiseNot(val1), bitwiseNot(val2));
}

std::bitset<32> ALU::bitwiseNand(std::bitset<32> val1, std::bitset<32> val2)
{
    return bitwiseNot(bitwiseAnd(val1, val2));
}

std::bitset<32> ALU::bitwiseXor(std::bitset<32> val1, std::bitset<32> val2)
{
    return bitwiseAnd(bitwiseOr(val1, val2), bitwiseNand(val1, val2));
}

std::bitset<32> ALU::bitwiseXnor(std::bitset<32> val1, std::bitset<32> val2)
{
    return bitwiseNot(bitwiseXor(val1, val2));
}

std::bitset<32> ALU::add(std::bitset<32> val1, std::bitset<32> val2)
{
    std::bitset<32> preadd = bitwiseXor(val1, val2);
    std::bitset<32> carries = bitwiseAnd(val1, val2);
    std::bitset<32> carries_shifted = hardwareLeftShift(carries, std::bitset<32>(1));
    // We know the base case of carries_shifted == 0 will always reach 
    // as the hardware left shift is 0 filling, so an N-bit number
    // left shifted N times is always 0.
    if (hardwareIsEqual(carries_shifted, std::bitset<32>(0))) {
        return preadd;
    }
    return add(preadd, carries_shifted);
}
