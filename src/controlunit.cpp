#include "controlunit.h"
#include "fileiounit.hpp"

void ControlUnit::step() {
    fetch();
    decode();
    execute();
    memoryAccess();
    writeBack();
}

void ControlUnit::fetch() {
    if_id.instruction = instruction_memory.read(pc);
    if_id.pc = pc;
    // "Add circuit" to increment the program counter
    pc = pc.to_ullong() + 4;
}

void ControlUnit::decode() {
    // std::bitset<7> opcode{if_id.instruction.to_string().substr(25, 7)};
    // // if we parse into an R-Type
    // // then we have an rs1, rs2, and rd
    // if (opcode == 0b0110011) {
    //     id_ex.p_instruction = std::unique_ptr<RISC::RISCInstruction>(new RISC::RType(if_id.instruction));
    // }
    
    // if we parse into an I-Type
    // then we have an rs1, rd, and imm

    // if we parse into an S-Type
    // then we have an rs1, rs2, and imm

    // if we parse into a B-Type
    // then we have an rs1, rs2, and imm

    // if we parse into a U-Type
    // then we have an rd and imm

    // if we parse into a J-Type
    // then we have an rd and imm


    // id_ex.pc = if_id.pc;
    // id_ex.imm = if_id.instruction.imm;
    // id_ex.rs1 = if_id.instruction.rs1;
    // id_ex.rs2 = if_id.instruction.rs2;
    // id_ex.write_reg = if_id.instruction.rd;
}

void ControlUnit::execute()
{
    
}

void ControlUnit::memoryAccess()
{
}

void ControlUnit::writeBack()
{
}
