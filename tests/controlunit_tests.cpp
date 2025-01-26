#include <gtest/gtest.h>
#include "controlunit.h"

TEST(ControlUnitTest, InitialRegVals) {
    ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/initial_reg_vals_mem.txt");
    EXPECT_EQ(cu.peekValRegister("A"), 0);
    EXPECT_EQ(cu.peekValRegister("B"), 0);
    EXPECT_EQ(cu.peekValRegister("C"), 0);
}

TEST(ControlUnitTest, NoOps) {
    ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/noops_mem.txt");
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 100);
    EXPECT_EQ(cu.peekValRegister("B"), 200);
    EXPECT_EQ(cu.peekValRegister("C"), 300);
}

TEST(ControlUnitTest, Moves) {
    ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/moves_mem.txt");
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
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("C"), -1);
}

TEST(ControlUnitTest, Adds) {
    ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/adds_mem.txt");
    cu.executeInstruction();
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 444);
    cu.executeInstruction();
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 444);
    EXPECT_EQ(cu.peekValRegister("B"), 545);
    cu.executeInstruction();
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("C"), 544);
}

TEST(ControlUnitTest, Jumps) {
    ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/jumps_mem.txt");
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 3);
    EXPECT_EQ(cu.peekValRegister("B"), 0);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 3);
    EXPECT_EQ(cu.peekValRegister("B"), 1);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 3);
    EXPECT_EQ(cu.peekValRegister("B"), 1);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 6);
    EXPECT_EQ(cu.peekValRegister("B"), 1);
}

TEST(ControlUnitTest, Comps) {
    ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/comps_mem.txt");
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    EXPECT_EQ(cu.peekCompRegister(), true);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekCompRegister(), false);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekCompRegister(), true);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekCompRegister(), false);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekCompRegister(), true);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekCompRegister(), true);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekCompRegister(), false);
}

TEST(ControlUnitTest, JumpIfs) {
    ControlUnit cu(std::string(MEMORY_FILES_DIR) + "/jumpifs_mem.txt");
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 1);
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 2);
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 3);
    EXPECT_EQ(cu.peekValRegister("C"), 100);
}

bool compareFiles(const std::string& p1, const std::string& p2) {
    std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
    std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

    if (f1.fail() || f2.fail()) {
        return false; //file problem
    }

    if (f1.tellg() != f2.tellg()) {
        return false; //size mismatch
    }

    //seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
        std::istreambuf_iterator<char>(),
        std::istreambuf_iterator<char>(f2.rdbuf()));
}

void restoreMemoryFile(const std::string& memory_file_path) {
    std::ifstream src(memory_file_path + ".bak", std::ios::binary);
    std::ofstream dst(memory_file_path, std::ios::binary);
    dst << src.rdbuf();
    src.close();
    dst.close();
}

TEST(ControlUnitTest, Writes) {
    std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/writes_mem.txt";
    restoreMemoryFile(memory_file_path);
    ControlUnit cu(memory_file_path);

    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    EXPECT_TRUE(compareFiles(memory_file_path, std::string(MEMORY_FILES_DIR) + "/writes_mem_ref1.txt"));
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    EXPECT_TRUE(compareFiles(memory_file_path, std::string(MEMORY_FILES_DIR) + "/writes_mem_ref2.txt"));
}

TEST(ControlUnitTest, Reads) {
    std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/reads_mem.txt";
    ControlUnit cu(memory_file_path);

    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 1);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("B"), 2);
    cu.executeInstruction();
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("A"), 3);
    cu.executeInstruction();
    EXPECT_EQ(cu.peekValRegister("C"), 3);
}

TEST(ControlUnitTest, Fibs) {
    std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/fibs_mem.txt";
    restoreMemoryFile(memory_file_path);
    ControlUnit cu(memory_file_path);

    while (!cu.isHalted()) {
        cu.executeInstruction();
    }
    EXPECT_EQ(cu.peekLineMemory(10000), 2);
    EXPECT_EQ(cu.peekLineMemory(10001), 3);
    EXPECT_EQ(cu.peekLineMemory(10002), 5);
    EXPECT_EQ(cu.peekLineMemory(10003), 8);
    EXPECT_EQ(cu.peekLineMemory(10004), 13);
    EXPECT_EQ(cu.peekLineMemory(10005), 21);
    EXPECT_EQ(cu.peekLineMemory(10006), 34);
    EXPECT_EQ(cu.peekLineMemory(10007), 55);
    EXPECT_EQ(cu.peekLineMemory(10008), 89);
    EXPECT_EQ(cu.peekLineMemory(10009), 144);
}

TEST(ControlUnitTest, WriteBlanks) {
    std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/clears_mem.txt";
    ControlUnit cu(memory_file_path);

    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
}

TEST(ControlUnitTest, Appends) {
    std::string memory_file_path = std::string(MEMORY_FILES_DIR) + "/appends_mem.txt";
    ControlUnit cu(memory_file_path);

    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    cu.executeInstruction();
    std::string line = cu.peekLineMemoryStr(100);
    EXPECT_EQ("ABC", line);
    cu.executeInstruction();
}