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
    p_instruction_file->print();
    p_reg_file->print();
    p_data_file->print();
    std::cout << std::endl;
}

void ControlUnit::dump()
{
    p_instruction_file->dump();
    p_reg_file->dump();
    p_data_file->dump();
}

void ControlUnit::fetch() {
    current_instruction = p_instruction_file->read(pc);
}

void ControlUnit::decode() {
    RISC::Instruction generic_instruction(current_instruction);

    switch (generic_instruction.opcode.to_ulong()) {
    case 0b0110011: {
        // RType
        RISC::RType r_instruction(current_instruction);
        if (r_instruction.funct3 == 0b000 && r_instruction.funct7 == 0b0000000) {
            // Add
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Add(r_instruction));
        }
        else if (r_instruction.funct3 == 0b000 && r_instruction.funct7 == 0b0100000) {
            // Sub
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Sub(r_instruction));
        }
        else if (r_instruction.funct3 == 0b001 && r_instruction.funct7 == 0b0000000) {
            // Sll
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Sll(r_instruction));
        }
        else if (r_instruction.funct3 == 0b010 && r_instruction.funct7 == 0b0000000) {
            // Slt
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Slt(r_instruction));
        }
        else if (r_instruction.funct3 == 0b011 && r_instruction.funct7 == 0b0000000) {
            // Sltu
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Sltu(r_instruction));
        }
        else if (r_instruction.funct3 == 0b100 && r_instruction.funct7 == 0b0000000) {
            // Xor
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Xor(r_instruction));
        }
        else if (r_instruction.funct3 == 0b101 && r_instruction.funct7 == 0b0000000) {
            // Srl 
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Srl(r_instruction));
        }
        else if (r_instruction.funct3 == 0b101 && r_instruction.funct7 == 0b0100000) {
            // Sra
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Sra(r_instruction));
        }
        else if (r_instruction.funct3 == 0b110 && r_instruction.funct7 == 0b0000000) {
            // Or
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Or(r_instruction));
        }
        else if (r_instruction.funct3 == 0b111 && r_instruction.funct7 == 0b0000000) {
            // And
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::And(r_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3/funct7 combination: " + r_instruction.funct3.to_string() + "/" + r_instruction.funct7.to_string());
        }
        break;
    }
    case 0b0010011: {
        // IType
        RISC::IType i_instruction(current_instruction);
        if (i_instruction.funct3 == 0b000) {
            // Addi
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Addi(i_instruction));
        }
        else if (i_instruction.funct3 == 0b100) {
            // Xori
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Xori(i_instruction));
        }
        else if (i_instruction.funct3 == 0b110) {
            // Ori
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Ori(i_instruction));
        }
        else if (i_instruction.funct3 == 0b111) {
            // Andi
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Andi(i_instruction));
        }
        else if (i_instruction.funct3 == 0b010) {
            // Slti
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Slti(i_instruction));
        }
        else if (i_instruction.funct3 == 0b011) {
            // Sltiu
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Sltiu(i_instruction));
        }
        else if (i_instruction.funct3 == 0b001 && i_instruction.funct7 == 0b0000000) {
            // Slli
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Slli(i_instruction));
        }
        else if (i_instruction.funct3 == 0b101 && i_instruction.funct7 == 0b0000000) {
            // Srli
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Srli(i_instruction));
        }
        else if (i_instruction.funct3 == 0b101 && i_instruction.funct7 == 0b0100000) {
            // Srai
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Srai(i_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3: " + i_instruction.funct3.to_string());
        }
        break;
    }
    case 0b0000011: {
        // IType (Load)
        RISC::IType i_instruction(current_instruction);
        if (i_instruction.funct3 == 0b010) {
            // LoadWord
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadWord(i_instruction));
        }
        else if (i_instruction.funct3 == 0b001) {
            // LoadHalfWord
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadHalfWord(i_instruction));
        }
        else if (i_instruction.funct3 == 0b000) {
            // LoadByte
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadByte(i_instruction));
        }
        else if (i_instruction.funct3 == 0b101) {
            // LoadUpperHalfWord
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadUpperHalfWord(i_instruction));
        }
        else if (i_instruction.funct3 == 0b100) {
            // LoadUpperByte
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadUpperByte(i_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3: " + i_instruction.funct3.to_string());
        }
        break;
    }
    case 0b0100011: {
        // SType 
        RISC::SType s_instruction(current_instruction);
        if (s_instruction.funct3 == 0b000) {
            // SaveByte
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SaveByte(s_instruction));
        }
        else if (s_instruction.funct3 == 0b001) {
            // SaveHalfWord
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SaveHalfWord(s_instruction));
        }
        else if (s_instruction.funct3 == 0b010) {
            // SaveWord
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SaveWord(s_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3: " + s_instruction.funct3.to_string());
        }
        break;
    }
    case 0b1100011: {
        // BType
        RISC::BType b_instruction(current_instruction);
        if (b_instruction.funct3 == 0b000) {
            // BranchEqual
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchEqual(b_instruction));
        }
        else if (b_instruction.funct3 == 0b001) {
            // BranchNotEqual
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchNotEqual(b_instruction));
        }
        else if (b_instruction.funct3 == 0b100) {
            // BranchLessThan
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchLessThan(b_instruction));
        }
        else if (b_instruction.funct3 == 0b101) {
            // BranchGreaterThanOrEqual
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchGreaterThanEqual(b_instruction));
        }
        else if (b_instruction.funct3 == 0b110) {
            // BranchLessThanUnsigned
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchLessThanUnsigned(b_instruction));
        }
        else if (b_instruction.funct3 == 0b111) {
            // BranchGreaterThanOrEqualUnsigned
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchGreaterThanEqualUnsigned(b_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3: " + b_instruction.funct3.to_string());
        }
        break;
    }
    case 0b0110111: {
        // UType (LoadUpperImmediate)
        p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadUpperImmediate(current_instruction));
        break;
    }
    case 0b0100111: {
        // UType (AUIPC)
        p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::AddUpperImmedateToPC(current_instruction));
        break;
    }
    case 0b1101111: {
        // JType (JumpAndLink)
        p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::JumpAndLink(current_instruction));
        break;
    }
    case 0b1100111: {
        // IType (JumpAndLinkRegister)
        p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::JumpAndLinkReg(current_instruction));
        break;
    }
    default:
        throw std::runtime_error("Unknown opcode: " + generic_instruction.opcode.to_string());
    }

    p_current_instruction->decode(p_reg_file, p_imm_gen);
}

void ControlUnit::execute() {
    p_current_instruction->execute(p_alu, pc);
}

void ControlUnit::memoryAccess() {
    p_current_instruction->accessMemory(p_data_file);
}

void ControlUnit::writeBack() {
    p_current_instruction->writeBack(p_reg_file);
}
