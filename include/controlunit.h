#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include "register.h"
#include "instruction.h"
#include "alu.h"
#include "riscinstructions.h"
#include "registerfile.h"
#include "memoryfile.h"
#include "instructionfile.h"
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

// Pipeline registers definitions
struct IFID {
    std::bitset<32> instruction;
    std::bitset<32> pc;
};

struct IDEX {
    std::bitset<32> pc;
    // std::bitset<32> imm;
    // std::bitset<32> reg1_data;
    // std::bitset<32> reg2_data;
    // std::bitset<5> write_reg_address;
    std::unique_ptr<RISC::RISCInstruction> p_instruction;
};

struct EXMEM {
    std::bitset<32> result;
    std::bitset<32> reg2;
    std::bitset<32> jump_target;
    std::bitset<5> write_reg;
};

struct MEMWB {
    std::bitset<32> result;
    std::bitset<32> read_data;
    std::bitset<5> write_reg;
};

class ControlUnit {
    std::bitset<32> pc;

    InstructionMemory instruction_memory;
    IFID if_id;
    RegisterFile register_file;
    IDEX id_ex;
    ALU alu;
    EXMEM ex_mem;
    DataMemory data_memory;
    MEMWB mem_wb;

public:
    ControlUnit(std::string memory_file, std::string instruction_file, std::string data_file)
        : instruction_memory(instruction_file), data_memory(data_file) {
        pc = std::bitset<32>(0);
    }
    ~ControlUnit() {}

    void step();

private:
    void fetch();
    void decode();
    void execute();
    void memoryAccess();
    void writeBack();
};

#endif // CONTROLUNIT_H