#include "alu.h"

std::bitset<32> ALU::add(std::bitset<32> rs1_val, std::bitset<32> rs2_val)
{
    std::bitset<1> carry = 0;
    std::bitset<32> result;
    for (int i = 0; i < 32; i++) {
        result.set(i, rs1_val.test(i) ^ rs2_val.test(i) ^ carry.any());
        carry = (rs1_val.test(i) & rs2_val.test(i)) | (rs2_val.test(i) & carry.any()) | (rs1_val.test(i) & carry.any());
    }
    return result;
}

std::bitset<32> ALU::addImmediate(std::bitset<32> rs1_val, std::bitset<12> imm_val)
{
    // Sign extend the immediate value
    std::bitset<32> imm = imm_val.to_ulong();
    if (imm.test(11)) {
        for (std::size_t i = 12; i >= 31; i++) {
            imm.set(i);
        }
    }
}

std::bitset<32> ALU::addressOffset(std::bitset<32> address, std::bitset<32> offset)
{
    return std::bitset<32>();
}
