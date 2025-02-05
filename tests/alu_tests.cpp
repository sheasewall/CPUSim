#include <gtest/gtest.h>
#include "alu.h"

// TEST(ALUTest, AddTest) {
//     ALU alu;
//     EXPECT_EQ(alu.add(1, 1), 2);
//     EXPECT_EQ(alu.add(-1, 1), 0);
// }

// TEST(ALUTest, SubtractTest) {
//     ALU alu;
//     EXPECT_EQ(alu.subtract(2, 1), 1);
//     EXPECT_EQ(alu.subtract(1, 2), -1);
// }

// TEST(ALUTest, AddressOffsetTest) {
//     ALU alu;
//     EXPECT_THROW(alu.addressOffset(-1, 1), std::out_of_range);
//     EXPECT_THROW(alu.addressOffset(std::numeric_limits<unsigned int>::max(), 0), std::out_of_range);
//     EXPECT_THROW(alu.addressOffset(std::numeric_limits<unsigned int>::max() - 1, 1), std::out_of_range);
//     EXPECT_THROW(alu.addressOffset(0, -1), std::out_of_range);
// }

ALU alu;

TEST(ALUTest, HardwareNor) {
    std::bitset<32> val1     = 0b00110000000000000000000000000000;
    std::bitset<32> val2     = 0b01010000000000000000000000000000;

    std::bitset<32> expected = 0b10001111111111111111111111111111;
    EXPECT_EQ(alu.hardwareNor(val1, val2), expected);
}

TEST(ALUTest, Not) {
    std::bitset<32> val1     = 0b00110000000000000000000000000000;

    std::bitset<32> expected = 0b11001111111111111111111111111111;
    EXPECT_EQ(alu.bitwiseNot(val1), expected);
}

TEST(ALUTest, Or) {
    std::bitset<32> val1     = 0b00110000000000000000000000000000;
    std::bitset<32> val2     = 0b01010000000000000000000000000000;

    std::bitset<32> expected = 0b01110000000000000000000000000000;
    EXPECT_EQ(alu.bitwiseOr(val1, val2), expected);
}

TEST(ALUTest, And) {
    std::bitset<32> val1     = 0b00110000000000000000000000000000;
    std::bitset<32> val2     = 0b01010000000000000000000000000000;

    std::bitset<32> expected = 0b00010000000000000000000000000000;
    EXPECT_EQ(alu.bitwiseAnd(val1, val2), expected);
}

TEST(ALUTest, Nand) {
    std::bitset<32> val1     = 0b00110000000000000000000000000000;
    std::bitset<32> val2     = 0b01010000000000000000000000000000;

    std::bitset<32> expected = 0b11101111111111111111111111111111;
    EXPECT_EQ(alu.bitwiseNand(val1, val2), expected);
}

TEST(ALUTest, Xor) {
    std::bitset<32> val1     = 0b00110000000000000000000000000000;
    std::bitset<32> val2     = 0b01010000000000000000000000000000;

    std::bitset<32> expected = 0b01100000000000000000000000000000;
    EXPECT_EQ(alu.bitwiseXor(val1, val2), expected);
}

TEST(ALUTest, Xnor) {
    std::bitset<32> val1     = 0b00110000000000000000000000000000;
    std::bitset<32> val2     = 0b01010000000000000000000000000000;

    std::bitset<32> expected = 0b10011111111111111111111111111111;
    EXPECT_EQ(alu.bitwiseXnor(val1, val2), expected);
}

TEST(ALUTest, HardwareLeftShift) {
    std::bitset<32> val1     = 0b00000000000000000000000000000001;
    std::bitset<32> val2     = 0b00000000000000000000000000000000;
    std::bitset<32> expected = 0b00000000000000000000000000000001;
    EXPECT_EQ(alu.hardwareLeftShift(val1, val2), expected);

    val1     = 0b00000000000000000000000000000001;
    val2     = 0b00000000000000000000000000000001;
    expected = 0b00000000000000000000000000000010;
    EXPECT_EQ(alu.hardwareLeftShift(val1, val2), expected);
    
    val1     = 0b00000000000000000000000000000001;
    val2     = 0b00000000000000000000000000000011;
    expected = 0b00000000000000000000000000001000;
    EXPECT_EQ(alu.hardwareLeftShift(val1, val2), expected);

    val1     = 0b00000000000000000000000000000001;
    val2     = 0b10000000000000000000000000000000;
    expected = 0b00000000000000000000000000000000;
    EXPECT_EQ(alu.hardwareLeftShift(val1, val2), expected);

    val1     = 0b10000000000000000000000000000001;
    val2     = 0b00000000000000000000000000000001;
    expected = 0b00000000000000000000000000000010;
    EXPECT_EQ(alu.hardwareLeftShift(val1, val2), expected);
    
    val1     = 0b11111000000000000000000000010101;
    val2     = 0b00000000000000000000000000000100;
    expected = 0b10000000000000000000000101010000;
    EXPECT_EQ(alu.hardwareLeftShift(val1, val2), expected);
}

TEST(ALUTest, HardwareRightShift) {
    std::bitset<32> val1     = 0b00000000000000000000000000000001;
    std::bitset<32> val2     = 0b00000000000000000000000000000000;
    std::bitset<32> expected = 0b00000000000000000000000000000001;
    EXPECT_EQ(alu.hardwareRightShift(val1, val2), expected);

    val1     = 0b10000000000000000000000000000000;
    val2     = 0b00000000000000000000000000000001;
    expected = 0b01000000000000000000000000000000;
    EXPECT_EQ(alu.hardwareRightShift(val1, val2), expected);
    
    val1     = 0b10000000000000000000000000000000;
    val2     = 0b00000000000000000000000000000011;
    expected = 0b00010000000000000000000000000000;
    EXPECT_EQ(alu.hardwareRightShift(val1, val2), expected);

    val1     = 0b10000000000000000000000000000000;
    val2     = 0b10000000000000000000000000000000;
    expected = 0b00000000000000000000000000000000;
    EXPECT_EQ(alu.hardwareRightShift(val1, val2), expected);
    
    val1     = 0b10000000000000000000000000000001;
    val2     = 0b00000000000000000000000000000001;
    expected = 0b01000000000000000000000000000000;
    EXPECT_EQ(alu.hardwareRightShift(val1, val2), expected);

    val1     = 0b11111000000000000000000000010101;
    val2     = 0b00000000000000000000000000000100;
    expected = 0b00001111100000000000000000000001;
    EXPECT_EQ(alu.hardwareRightShift(val1, val2), expected);
}

TEST(ALUTest, Add) {
    std::bitset<32> val1     = 0b00000000000000000000000000000001;
    std::bitset<32> val2     = 0b00000000000000000000000000000000;
    std::bitset<32> expected = 0b00000000000000000000000000000001;
    EXPECT_EQ(alu.add(val1, val2), expected);

    val1     = 0b10000000000000000000000000000000;
    val2     = 0b00000000000000000000000000000001;
    expected = 0b10000000000000000000000000000001;
    EXPECT_EQ(alu.add(val1, val2), expected);

    val1     = 0b10000000000000000000000000000000;
    val2     = 0b10000000000000000000000000000001;
    expected = 0b00000000000000000000000000000001;
    EXPECT_EQ(alu.add(val1, val2), expected);
    
    val1     = 0b00000000000000000000000000000011;
    val2     = 0b00000000000000000000000000000011;
    expected = 0b00000000000000000000000000000110;
    EXPECT_EQ(alu.add(val1, val2), expected);

    val1     = 0b11001100110011001100110011001100;
    val2     = 0b10101010101010101010101010101010;
    expected = 0b01110111011101110111011101110110;
    EXPECT_EQ(alu.add(val1, val2), expected);
    
    val1     = 0b11111111111111111111111111111111;
    val2     = 0b11111111111111111111111111111111;
    expected = 0b11111111111111111111111111111110;
    EXPECT_EQ(alu.add(val1, val2), expected);
}

TEST(ALUTest, Negate) {
    std::bitset<32> val1     = 0b00000000000000000000000000000000;
    std::bitset<32> expected = 0b00000000000000000000000000000000;
    EXPECT_EQ(alu.negate(val1), expected);

    val1     = 0b00000000000000000000000000000001;
    expected = 0b11111111111111111111111111111111;
    EXPECT_EQ(alu.negate(val1), expected);

    expected = 0b00000000000000000000000000000001;
    val1     = 0b11111111111111111111111111111111;
    EXPECT_EQ(alu.negate(val1), expected);

    val1     = 0b00000000000000000000000000000010;
    expected = 0b11111111111111111111111111111110;
    EXPECT_EQ(alu.negate(val1), expected);

    val1     = 0b01111111111111111111111111111110;
    expected = 0b10000000000000000000000000000010;
    EXPECT_EQ(alu.negate(val1), expected);

    val1     = 0b01111111111111111111111111111111;
    expected = 0b10000000000000000000000000000001;
    EXPECT_EQ(alu.negate(val1), expected);

    val1     = 0b10000000000000000000000000000000;
    expected = 0b10000000000000000000000000000000;
    EXPECT_EQ(alu.negate(val1), expected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}