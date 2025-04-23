#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H


#include "alu.h"
#include "riscinstructions.h"
#include "immgen.h"
#include "registerfile.h"
#include "memoryfile.h"
#include "instructionfile.h"
#include "exceptions.h"
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

class ControlUnit {
protected:
    unsigned long cycles;

    std::bitset<32> pc;
    std::bitset<32> current_instruction;
    std::shared_ptr<RISC::Instruction> p_current_instruction;

    bool execute_resolved = true;
    std::shared_ptr<RISC::Instruction> p_to_decode_next;
    std::shared_ptr<RISC::Instruction> p_to_execute_next;
    std::shared_ptr<RISC::Instruction> p_to_memory_access_next;
    std::shared_ptr<RISC::Instruction> p_to_write_back_next;

    std::shared_ptr<InstructionFile> p_instruction_file;
    std::shared_ptr<ImmGen> p_imm_gen;
    std::shared_ptr<RegisterFile> p_reg_file;
    std::shared_ptr<ALU> p_alu;
    std::shared_ptr<MemoryFile> p_data_file;

public:
    ControlUnit(std::string bin_file) {
        cycles = 0;
        pc = std::bitset<32>(0);
        p_current_instruction = std::make_shared<RISC::Instruction>(std::bitset<32>(0));

        p_to_decode_next = std::make_shared<RISC::Instruction>(std::bitset<32>(0));
        p_to_execute_next = std::make_shared<RISC::Instruction>(std::bitset<32>(0));
        p_to_memory_access_next = std::make_shared<RISC::Instruction>(std::bitset<32>(0));
        p_to_write_back_next = std::make_shared<RISC::Instruction>(std::bitset<32>(0));

        p_instruction_file = std::make_shared<InstructionFile>(bin_file);
        p_imm_gen = std::make_shared<ImmGen>();
        p_reg_file = std::make_shared<RegisterFile>();
        p_alu = std::make_shared<ALU>();
        p_data_file = std::make_shared<MemoryFile>(bin_file);
    }
    ~ControlUnit() {}

    void step();
    void flush_pipeline();

    // For verification only
    void signature();

private:
    std::bitset<32> fetch(std::shared_ptr<InstructionFile> _p_instruction_file, std::bitset<32> _pc);
    void decode(std::shared_ptr<RISC::Instruction>& _p_current_instruction, std::bitset<32> _current_instruction,
        std::shared_ptr<RegisterFile> _p_reg_file, std::shared_ptr<ImmGen> _p_imm_gen);
    void execute(std::shared_ptr<RISC::Instruction> _p_current_instruction, std::shared_ptr<ALU> _p_alu, std::bitset<32>& _pc);
    void memoryAccess(std::shared_ptr<RISC::Instruction> _p_current_instruction, std::shared_ptr<MemoryFile> _p_data_file);
    void writeBack(std::shared_ptr<RISC::Instruction> _p_current_instruction, std::shared_ptr<RegisterFile> _p_reg_file);
};

#endif // CONTROLUNIT_H