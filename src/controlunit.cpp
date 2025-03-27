#include "controlunit.h"

void ControlUnit::step() {
    fetch();
    decode();
    execute();
    memoryAccess();
    writeBack();
    cycles++;
}

void ControlUnit::print(bool print_instructions, bool print_registers, bool print_data)
{
    std::cout << "Cycle: " << cycles << std::endl;
    if (print_instructions) {
        std::cout << "Instructions: " << std::endl;
        p_instruction_file->print();
    }
    if (print_registers) {
        std::cout << "Registers: " << std::endl;
        p_reg_file->print();
    }
    if (print_data) {
        std::cout << "Data: " << std::endl;
        p_data_file->print();
    }
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
            // ShiftLeftLogi
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftLeftLogi(r_instruction));
        }
        else if (r_instruction.funct3 == 0b010 && r_instruction.funct7 == 0b0000000) {
            // SetLessThan
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SetLessThan(r_instruction));
        }
        else if (r_instruction.funct3 == 0b011 && r_instruction.funct7 == 0b0000000) {
            // SetLessThanUnsigned
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SetLessThanUnsigned(r_instruction));
        }
        else if (r_instruction.funct3 == 0b100 && r_instruction.funct7 == 0b0000000) {
            // Xor
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Xor(r_instruction));
        }
        else if (r_instruction.funct3 == 0b101 && r_instruction.funct7 == 0b0000000) {
            // ShiftRightLogi 
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftRightLogi(r_instruction));
        }
        else if (r_instruction.funct3 == 0b101 && r_instruction.funct7 == 0b0100000) {
            // ShiftRightArith
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftRightArith(r_instruction));
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
            // AddImm
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::AddImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b100) {
            // XorImm
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::XorImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b110) {
            // OrImm
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::OrImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b111) {
            // AndImm
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::AndImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b010) {
            // SetLessThanImm
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SetLessThanImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b011) {
            // SetLessThanImmUnsigned
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SetLessThanImmUnsigned(i_instruction));
        }
        else if (i_instruction.funct3 == 0b001 && i_instruction.funct7 == 0b0000000) {
            // ShiftLeftLogiImm
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftLeftLogiImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b101 && i_instruction.funct7 == 0b0000000) {
            // ShiftRightLogiImm
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftRightLogiImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b101 && i_instruction.funct7 == 0b0100000) {
            // ShiftRightArithImm
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftRightArithImm(i_instruction));
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
            // LoadUnsignedHalfWord
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadUnsignedHalfWord(i_instruction));
        }
        else if (i_instruction.funct3 == 0b100) {
            // LoadUnsignedByte
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadUnsignedByte(i_instruction));
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
    case 0b1110011: {
        // System (IType)
        RISC::IType s_instruction(current_instruction);

        if (s_instruction.imm == 0b000000000000) {
            // Ecall
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Ecall(s_instruction));
        }
        else if (s_instruction.imm == 0b000000000001) {
            // Ebreak
            p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Ebreak(s_instruction));
        }
        else {
            throw std::runtime_error("Unknown imm: " + s_instruction.imm.to_string());
        }
        break;
    }
    default:
        throw std::runtime_error("Unknown opcode: " + generic_instruction.opcode.to_string());
    }

    p_current_instruction->decode(p_reg_file, p_imm_gen);
}

void ControlUnit::execute() {
    // try {
        p_current_instruction->execute(p_alu, pc);
    // } catch (const RiscTrapException& e) {
    //     std::cout << e.what() << std::endl;
    // } catch (...) {
    //     throw std::runtime_error("Error executing instruction");
    // }
}

void ControlUnit::memoryAccess() {
    p_current_instruction->accessMemory(p_data_file);
}

void ControlUnit::writeBack() {
    p_current_instruction->writeBack(p_reg_file);
}
