#include "controlunit.h"

void ControlUnit::step() {
    fetch();
    decode();
    execute();
    memoryAccess();
    writeBack();
    cycles++;
}

void ControlUnit::fetch() {
    p_current_instruction = createInstruction(p_instruction_file->read(pc));
}

void ControlUnit::decode() {
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

std::shared_ptr<RISC::Instruction> ControlUnit::createInstruction(std::bitset<32> instruction)
{
    std::string instr = instruction.to_string();
    std::bitset<7> opcode = std::bitset<7>(instr.substr(25, 7));
    std::bitset<3> funct3 = std::bitset<3>(instr.substr(17, 3));
    std::bitset<7> funct7 = std::bitset<7>(instr.substr(0, 7));
    std::shared_ptr<RISC::Instruction> p_instruction;

    switch (opcode.to_ulong()) {
    case 0b0110011: {
        // RType
        if (funct3 == 0b000 && funct7 == 0b0000000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Add(instruction));
        }
        else if (funct3 == 0b000 && funct7 == 0b0100000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Sub(instruction));
        }
        else if (funct3 == 0b001 && funct7 == 0b0000000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftLeftLogi(instruction));
        }
        else if (funct3 == 0b010 && funct7 == 0b0000000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SetLessThan(instruction));
        }
        else if (funct3 == 0b011 && funct7 == 0b0000000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SetLessThanUnsigned(instruction));
        }
        else if (funct3 == 0b100 && funct7 == 0b0000000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Xor(instruction));
        }
        else if (funct3 == 0b101 && funct7 == 0b0000000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftRightLogi(instruction));
        }
        else if (funct3 == 0b101 && funct7 == 0b0100000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftRightArith(instruction));
        }
        else if (funct3 == 0b110 && funct7 == 0b0000000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Or(instruction));
        }
        else if (funct3 == 0b111 && funct7 == 0b0000000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::And(instruction));
        }
        else {
            throw std::runtime_error("Unknown R-type instruction: " + instr);
        }
        break;
    }
    case 0b0010011: {
        // IType
        if (funct3 == 0b000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::AddImm(instruction));
        }
        else if (funct3 == 0b100) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::XorImm(instruction));
        }
        else if (funct3 == 0b110) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::OrImm(instruction));
        }
        else if (funct3 == 0b111) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::AndImm(instruction));
        }
        else if (funct3 == 0b010) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SetLessThanImm(instruction));
        }
        else if (funct3 == 0b011) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SetLessThanImmUnsigned(instruction));
        }
        else if (funct3 == 0b001 && funct7 == 0b0000000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftLeftLogiImm(instruction));
        }
        else if (funct3 == 0b101 && funct7 == 0b0000000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftRightLogiImm(instruction));
        }
        else if (funct3 == 0b101 && funct7 == 0b0100000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftRightArithImm(instruction));
        }
        else {
            throw std::runtime_error("Unknown I-type instruction: " + instr);
        }
        break;
    }
    case 0b0000011: {
        // IType (Load)
        if (funct3 == 0b010) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadWord(instruction));
        }
        else if (funct3 == 0b001) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadHalfWord(instruction));
        }
        else if (funct3 == 0b000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadByte(instruction));
        }
        else if (funct3 == 0b101) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadUnsignedHalfWord(instruction));
        }
        else if (funct3 == 0b100) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadUnsignedByte(instruction));
        }
        else {
            throw std::runtime_error("Unknown I-type load instruction: " + instr);
        }
        break;
    }
    case 0b0100011: {
        // SType 
        if (funct3 == 0b000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SaveByte(instruction));
        }
        else if (funct3 == 0b001) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SaveHalfWord(instruction));
        }
        else if (funct3 == 0b010) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SaveWord(instruction));
        }
        else {
            throw std::runtime_error("Unknown S-type instruction: " + instr);
        }
        break;
    }
    case 0b1100011: {
        // BType
        if (funct3 == 0b000) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchEqual(instruction));
        }
        else if (funct3 == 0b001) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchNotEqual(instruction));
        }
        else if (funct3 == 0b100) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchLessThan(instruction));
        }
        else if (funct3 == 0b101) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchGreaterThanEqual(instruction));
        }
        else if (funct3 == 0b110) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchLessThanUnsigned(instruction));
        }
        else if (funct3 == 0b111) {
            p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchGreaterThanEqualUnsigned(instruction));
        }
        else {
            throw std::runtime_error("Unknown B-type instruction: " + instr);
        }
        break;
    }
    case 0b0110111: {
        // UType (LoadUpperImmediate)
        p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadUpperImmediate(instruction));
        break;
    }
    case 0b0010111: {
        // UType (AUIPC)
        p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::AddUpperImmedateToPC(instruction));
        break;
    }
    case 0b1101111: {
        // JType (JumpAndLink)
        p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::JumpAndLink(instruction));
        break;
    }
    case 0b1100111: {
        // IType (JumpAndLinkRegister)
        p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::JumpAndLinkReg(instruction));
        break;
    }
    case 0b1110011: {
        // System (IType)
        std::bitset<12> funct12 = std::bitset<12>(instr.substr(0, 12));
        if (funct3 == 000) {
            if (funct12 == 0b000000000000) {
                // Ecall
                p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Ecall(instruction));
            }
            else if (funct12 == 0b000000000001) {
                // Ebreak
                p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Ebreak(instruction));
            }
        }
        else {
            throw std::runtime_error("Unknown system instruction: " + instruction.to_string());
        }
        break;
    }
    case 0b0001111: {
        // IType (Fence)
        p_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Fence(instruction));
        break;
    }
    default:
        throw std::runtime_error("Unknown instruction: " + instr);
    }

    return p_instruction;
}

void ControlUnit::signature()
{
    std::ofstream signature_file("DUT-rv32sim.signature");
    std::string signature = p_data_file->signature();

    std::stringstream ss(signature);
    std::string line;
    int number_of_lines = 0;
    while (std::getline(ss, line)) {
        number_of_lines++;
    }
    signature_file << p_data_file->signature();
    // insert empty lines to reach a multiple of 4 (lines)
    for (int i = number_of_lines % 4; (i > 0) && (i < 4); i++) {
        signature_file << "00000000\n";
    }
}