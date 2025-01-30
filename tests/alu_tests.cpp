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