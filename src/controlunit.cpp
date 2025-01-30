#include "controlunit.h"

void ControlUnit::step() {
    fetch();
    decode();
    execute();
    memoryAccess();
    writeBack();
    cycles++;
}

void ControlUnit::print()
{
    std::cout << "Cycle: " << cycles << std::endl;
    p_reg_file->print();
    p_data_mem->print();
    std::cout << std::endl;
}

void ControlUnit::fetch() {
    current_instruction = p_instruction_memory->read(pc);
    // "Add circuit" to increment the program counter
    pc = pc.to_ullong() + 4;
}

void ControlUnit::decode() {
    RISC::Instruction generic_instruction(current_instruction);
    
    if (generic_instruction.opcode == 0b0110011) { 
        // RType
        RISC::RType r_instruction(current_instruction);
        if (r_instruction.funct3 == 0b000 && r_instruction.funct7 == 0b0000000) {
            // Add
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Add(r_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3/funct7 combination: " + r_instruction.funct3.to_string() + "/" + r_instruction.funct7.to_string());
        }
    }
    else if (generic_instruction.opcode == 0b0010011) {
        // IType
        RISC::IType i_instruction(current_instruction);
        if (i_instruction.funct3 == 0b000) {
            // Addi
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::AddI(i_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3: " + i_instruction.funct3.to_string());
        }
    }
    else if (generic_instruction.opcode == 0b0000011) {
        // IType (Load)
        RISC::IType i_instruction(current_instruction);
        if (i_instruction.funct3 == 0b010) {
            // LoadW
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadW(i_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3: " + i_instruction.funct3.to_string());
        }
    }
    else if (generic_instruction.opcode == 0b0100011) {
        // SType 
        RISC::SType i_instruction(current_instruction);
        if (i_instruction.funct3 == 0b010) {
            // LoadW
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SaveW(i_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3: " + i_instruction.funct3.to_string());
        }
    }
    else {
        throw std::runtime_error("Unknown opcode: " + generic_instruction.opcode.to_string());
    }

    p_current_instruction->decode(p_reg_file, p_imm_gen);
}

void ControlUnit::execute() {
    p_current_instruction->execute(p_alu);
}

void ControlUnit::memoryAccess() {
    p_current_instruction->accessMemory(p_data_mem);
}

void ControlUnit::writeBack() {
    p_current_instruction->writeBack(p_reg_file);
}
