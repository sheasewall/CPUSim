#include <gtest/gtest.h>
#include "controlunit.h"

TEST(ControlUnitTest, InitialRegVals) {
    ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/initial_reg_vals_mem.txt");
    EXPECT_EQ(cu.peekValRegister("A"), 0);
    EXPECT_EQ(cu.peekValRegister("B"), 0);
    EXPECT_EQ(cu.peekValRegister("C"), 0);
}

TEST(ControlUnitTest, MoveVals) {
    ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/move_vals_mem.txt");
    EXPECT_EQ(cu.peekValRegister("A"), 0);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 100);

    EXPECT_EQ(cu.peekValRegister("B"), 0);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("B"), 200);

    EXPECT_EQ(cu.peekValRegister("C"), 0);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("C"), 300);
}

TEST(ControlUnitTest, MoveRegs) {
    ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/move_regs_mem.txt");
    EXPECT_EQ(cu.peekValRegister("A"), 0);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 99);
    EXPECT_EQ(cu.peekValRegister("B"), 0);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 99);
    EXPECT_EQ(cu.peekValRegister("B"), 99);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 99);
    EXPECT_EQ(cu.peekValRegister("B"), 11);
    EXPECT_EQ(cu.peekValRegister("C"), 0);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 99);
    EXPECT_EQ(cu.peekValRegister("B"), 11);
    EXPECT_EQ(cu.peekValRegister("C"), 11);
}