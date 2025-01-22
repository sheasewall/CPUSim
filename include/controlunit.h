#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include "register.h"
#include "instruction.h"
#include "alu.h"
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>

class ControlUnit {
    Register<int> A;
    Register<int> B;
    Register<int> C;

    Register<bool> compare;
    // I should probably have a halt register

    // Program Counter
    Register<unsigned int> PC;
    // Instruction Register (string)
    Register<std::string> sIR;
    // Instruction Register (pointer)
    Register<std::shared_ptr<Instruction>> pIR;

    // Memory (map of line number to instruction)
    std::unordered_map<unsigned int, std::string> memory;
    // Map of value register names to value register pointers
    std::unordered_map<std::string, Register<int>*> val_reg_registry;

    ALU alu;
    
    std::string memory_file;

public:
    ControlUnit(std::string memory_file);
    ControlUnit();
    ~ControlUnit() {}

    void executeInstruction();
    int peekValRegister(std::string reg);
    void printValRegisters();
    void run(unsigned int line_num);
    void run() { run(PC.getVal()); }
    bool isHalted();

private:    
    void readMemory();

    void fetch();
    void decode();
    void execute();
};

#endif // CONTROLUNIT_H