#include <gtest/gtest.h>
#include "riscinstructions.h"

using namespace RISC;

TEST(InstructionParsingTests, RTypes) {
    std::bitset<32> instruction = 0b01011110001000001000001010110011;
    RType rType(instruction);
    EXPECT_EQ(rType.opcode, 0b0110011);
    EXPECT_EQ(rType.rd, 0b00101);
    EXPECT_EQ(rType.funct3, 0b000);
    EXPECT_EQ(rType.rs1, 0b00001);
    EXPECT_EQ(rType.rs2, 0b00010);
    EXPECT_EQ(rType.funct7, 0b0101111);
}

TEST(InstructionParsingTests, ITypes) {
    std::bitset<32> instruction = 0b11000100001010001100001010010011;
    IType iType(instruction);
    EXPECT_EQ(iType.opcode, 0b0010011);
    EXPECT_EQ(iType.rd, 0b00101);
    EXPECT_EQ(iType.funct3, 0b100);
    EXPECT_EQ(iType.rs1, 0b10001);
    EXPECT_EQ(iType.imm, 0b110001000010);
}

TEST(InstructionParsingTests, STypes) {
    std::bitset<32> instruction = 0b00010100101010001010001010100011;
    SType sType(instruction);
    EXPECT_EQ(sType.opcode, 0b0100011);
    EXPECT_EQ(sType.funct3, 0b010);
    EXPECT_EQ(sType.rs1, 0b10001);
    EXPECT_EQ(sType.rs2, 0b01010);
    EXPECT_EQ(sType.imm, 0b000101000101);
}

TEST(InstructionParsingTests, BTypes) {
    std::bitset<32> instruction = 0b00100111111111101001010111100011;
    BType bType(instruction);
    EXPECT_EQ(bType.opcode, 0b1100011);
    EXPECT_EQ(bType.funct3, 0b001);
    EXPECT_EQ(bType.rs1, 0b11101);
    EXPECT_EQ(bType.rs2, 0b11111);
    EXPECT_EQ(bType.imm, 0b010100110101);
}

TEST(InstructionParsingTests, UTypes) {
    std::bitset<32> instruction = 0b01010101010101010101101100110111;
    UType uType(instruction);
    EXPECT_EQ(uType.opcode, 0b0110111);
    EXPECT_EQ(uType.rd, 0b10110);
    EXPECT_EQ(uType.imm_val, 0b01010101010101010101);
}

TEST(InstructionParsingTests, JTypes) {
    std::bitset<32> instruction = 0b10110110110100111100011101101111;
    JType jType(instruction);
    EXPECT_EQ(jType.opcode, 0b1101111);
    EXPECT_EQ(jType.rd, 0b01110);
    EXPECT_EQ(jType.imm_val, 0b10011110010110110110);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}