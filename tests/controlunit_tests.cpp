#include <gtest/gtest.h>
#include "controlunit.h"

// TEST(ControlUnitTest, InitialRegVals) {
//     ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/initial_reg_vals_mem.txt");
//     EXPECT_EQ(cu.peekValRegister("A"), 0);
//     EXPECT_EQ(cu.peekValRegister("B"), 0);
//     EXPECT_EQ(cu.peekValRegister("C"), 0);
// }

// TEST(ControlUnitTest, NoOps) {
//     ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/instructions/readonly/noops_mem.txt");
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 100);
//     EXPECT_EQ(cu.peekValRegister("B"), 200);
//     EXPECT_EQ(cu.peekValRegister("C"), 300);
// }

// TEST(ControlUnitTest, Moves) {
//     ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/instructions/readonly/moves_mem.txt");
//     EXPECT_EQ(cu.peekValRegister("A"), 0);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 99);
//     EXPECT_EQ(cu.peekValRegister("B"), 0);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 99);
//     EXPECT_EQ(cu.peekValRegister("B"), 99);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 99);
//     EXPECT_EQ(cu.peekValRegister("B"), 11);
//     EXPECT_EQ(cu.peekValRegister("C"), 0);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 99);
//     EXPECT_EQ(cu.peekValRegister("B"), 11);
//     EXPECT_EQ(cu.peekValRegister("C"), 11);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("C"), -1);
// }

// TEST(ControlUnitTest, Adds) {
//     ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/instructions/readonly/adds_mem.txt");
//     cu.executeInstruction();
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 444);
//     cu.executeInstruction();
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 444);
//     EXPECT_EQ(cu.peekValRegister("B"), 545);
//     cu.executeInstruction();
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("C"), 544);
// }

// TEST(ControlUnitTest, Jumps) {
//     ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/instructions/readonly/jumps_mem.txt");
//     cu.executeInstruction();
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 3);
//     EXPECT_EQ(cu.peekValRegister("B"), 0);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 3);
//     EXPECT_EQ(cu.peekValRegister("B"), 1);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 3);
//     EXPECT_EQ(cu.peekValRegister("B"), 1);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 6);
//     EXPECT_EQ(cu.peekValRegister("B"), 1);
// }

// TEST(ControlUnitTest, Comps) {
//     ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/instructions/readonly/comps_mem.txt");
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekCompRegister(), true);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekCompRegister(), false);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekCompRegister(), true);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekCompRegister(), false);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekCompRegister(), true);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekCompRegister(), true);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekCompRegister(), false);
// }

// TEST(ControlUnitTest, JumpIfs) {
//     ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/instructions/readonly/jumpifs_mem.txt");
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 1);
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 2);
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 3);
//     EXPECT_EQ(cu.peekValRegister("C"), 100);
// }

// bool compareFiles(const std::string& p1, const std::string& p2) {
//     std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
//     std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

//     if (f1.fail() || f2.fail()) {
//         return false; //file problem
//     }

//     if (f1.tellg() != f2.tellg()) {
//         return false; //size mismatch
//     }

//     //seek back to beginning and use std::equal to compare contents
//     f1.seekg(0, std::ifstream::beg);
//     f2.seekg(0, std::ifstream::beg);
//     return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
//         std::istreambuf_iterator<char>(),
//         std::istreambuf_iterator<char>(f2.rdbuf()));
// }

// void restoreMemoryFile(const std::string& memory_file_path) {
//     std::ifstream src(memory_file_path + ".bak", std::ios::binary);
//     std::ofstream dst(memory_file_path, std::ios::binary);
//     dst << src.rdbuf();
//     src.close();
//     dst.close();
// }

// TEST(ControlUnitTest, Writes) {
//     std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/instructions/writers/writes_mem.txt";
//     restoreMemoryFile(memory_file_path);
//     ControlUnit cu(memory_file_path);

//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     EXPECT_TRUE(compareFiles(memory_file_path, std::string(MEMORY_FILES_DIR) + "/instructions/writers/writes_mem_ref1.txt"));
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     EXPECT_TRUE(compareFiles(memory_file_path, std::string(MEMORY_FILES_DIR) + "/instructions/writers/writes_mem_ref2.txt"));
// }

// TEST(ControlUnitTest, Reads) {
//     std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/instructions/readonly/reads_mem.txt";
//     ControlUnit cu(memory_file_path);

//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 1);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("B"), 2);
//     cu.executeInstruction();
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 3);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("C"), 3);
// }

// TEST(ControlUnitTest, Fibs) {
//     std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/routines/writers/fibs_mem.txt";
//     restoreMemoryFile(memory_file_path);
//     ControlUnit cu(memory_file_path);

//     while (!cu.isHalted()) {
//         cu.executeInstruction();
//     }
//     EXPECT_EQ(cu.peekLineMemory(10000), 2);
//     EXPECT_EQ(cu.peekLineMemory(10001), 3);
//     EXPECT_EQ(cu.peekLineMemory(10002), 5);
//     EXPECT_EQ(cu.peekLineMemory(10003), 8);
//     EXPECT_EQ(cu.peekLineMemory(10004), 13);
//     EXPECT_EQ(cu.peekLineMemory(10005), 21);
//     EXPECT_EQ(cu.peekLineMemory(10006), 34);
//     EXPECT_EQ(cu.peekLineMemory(10007), 55);
//     EXPECT_EQ(cu.peekLineMemory(10008), 89);
//     EXPECT_EQ(cu.peekLineMemory(10009), 144);
// }

// TEST(ControlUnitTest, WriteBlanks) {
//     std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/instructions/writers/clears_mem.txt";
//     ControlUnit cu(memory_file_path);

//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
// }

// TEST(ControlUnitTest, Appends) {
//     std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/instructions/writers/appends_mem.txt";
//     restoreMemoryFile(memory_file_path);
//     ControlUnit cu(memory_file_path);

//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     cu.executeInstruction();
//     std::string line = cu.peekDataLine(5);
//     EXPECT_EQ("10 ABC", line);
// }

// TEST(ControlUnitTest, ReadChars) {
//     std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/instructions/writers/read_chars_mem.txt";
//     restoreMemoryFile(memory_file_path);
//     ControlUnit cu(memory_file_path);

//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("A"), 72);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("B"), 69);
//     cu.executeInstruction();
//     EXPECT_EQ(cu.peekValRegister("B"), 76);
//     std::string line = cu.peekDataLine(5);
//     EXPECT_EQ("LO", line);
// }

// TEST(ControlUnitTest, EqRoutine) {
//     std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/routines/readonly/eq_mem.txt";
//     ControlUnit cu(memory_file_path);

//     while (!cu.isHalted()) {
//         cu.executeInstruction();
//     }
//     EXPECT_EQ(cu.peekValRegister("C"), 1);
// }

// TEST(ControlUnitTest, NegRoutine) {
//     std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/routines/readonly/neg_mem.txt";
//     ControlUnit cu(memory_file_path);

//     while (!cu.isHalted()) {
//         cu.executeInstruction();
//     }
//     EXPECT_EQ(cu.peekValRegister("C"), 1);
// }

// TEST(ControlUnitTest, SubtractRoutine) {
//     std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/routines/writers/sub_mem.txt";
//     restoreMemoryFile(memory_file_path);
//     ControlUnit cu(memory_file_path);

//     while (!cu.isHalted()) {
//         cu.executeInstruction();
//     }
//     EXPECT_EQ(cu.peekValRegister("C"), 1);
// }

// TEST(ControlUnitTest, MulRoutine) {
//     std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/routines/writers/mul_mem.txt";
//     restoreMemoryFile(memory_file_path);
//     ControlUnit cu(memory_file_path);

//     while (!cu.isHalted()) {
//         cu.executeInstruction();
//     }
//     EXPECT_EQ(cu.peekValRegister("C"), 1);
// }

// TEST(ControlUnitTest, DivRoutine) {
//     std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/routines/writers/div_mem.txt";
//     restoreMemoryFile(memory_file_path);
//     ControlUnit cu(memory_file_path);

//     while (!cu.isHalted()) {
//         cu.executeInstruction();
//     }
//     EXPECT_EQ(cu.peekValRegister("C"), 1);
// }

// TEST(ControlUnitTest, AddLinesRoutine) {
//     std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/routines/writers/add_line_nums_mem.txt";
//     restoreMemoryFile(memory_file_path);
//     ControlUnit cu(memory_file_path);

//     while (!cu.isHalted()) {
//         cu.executeInstruction();
//     }
//     EXPECT_EQ(cu.peekValRegister("C"), 1);
// }