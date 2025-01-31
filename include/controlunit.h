#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H


#include "alu.h"
#include "riscinstructions.h"
#include "immgen.h"
#include "registerfile.h"
#include "memoryfile.h"
#include "instructionfile.h"
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

class ControlUnit {
private:
    unsigned long cycles;

    std::bitset<32> pc;
    std::bitset<32> current_instruction;
    std::shared_ptr<RISC::Instruction> p_current_instruction;

    std::shared_ptr<InstructionFile> p_instruction_file;
    std::shared_ptr<ImmGen> p_imm_gen;
    std::shared_ptr<RegisterFile> p_reg_file;
    std::shared_ptr<ALU> p_alu;
    std::shared_ptr<MemoryFile> p_data_file;

public:
    ControlUnit(std::string instruction_file, std::string register_file, std::string data_file) {
        cycles = 0;
        pc = std::bitset<32>(0);
        p_instruction_file = std::make_shared<InstructionFile>(instruction_file);
        p_imm_gen = std::make_shared<ImmGen>();
        p_reg_file = std::make_shared<RegisterFile>(register_file);
        p_alu = std::make_shared<ALU>();
        p_data_file = std::make_shared<MemoryFile>(data_file);
    }
    ~ControlUnit() {}

    void step();
    void print();
    void dump();

private:
    void fetch();
    void decode();
    void execute();
    void memoryAccess();
    void writeBack();
};

#endif // CONTROLUNIT_H