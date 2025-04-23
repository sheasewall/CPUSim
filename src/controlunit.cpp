#include "controlunit.h"

void ControlUnit::step() {
    cycles++;

    std::shared_ptr<RISC::Instruction> p_to_decode = p_to_decode_next;
    std::shared_ptr<RISC::Instruction> p_to_execute = p_to_execute_next;
    std::shared_ptr<RISC::Instruction> p_to_memory_access = p_to_memory_access_next;
    std::shared_ptr<RISC::Instruction> p_to_write_back = p_to_write_back_next;

    if (!should_stall_for_execute) {
        to_decode = fetch(p_instruction_file, pc);
        p_to_decode_next = std::shared_ptr<RISC::Instruction>(new RISC::Instruction(to_decode)); // Notice how FETCH and EXECUTE both use pc
        should_stall_for_execute = true;
    }
    else {
        p_to_decode_next = nullptr;
    }

    if (p_to_decode) {
        decode(p_to_decode, to_decode, p_reg_file, p_imm_gen); // Notice how DECODE and WRITE_BACK both use reg_file
    }
    p_to_execute_next = p_to_decode;

    if (p_to_execute) {
        execute(p_to_execute, p_alu, pc);
    }
    p_to_memory_access_next = p_to_execute;

    if (p_to_memory_access) {
        memoryAccess(p_to_memory_access, p_data_file);
        should_stall_for_execute = false;
    }
    p_to_write_back_next = p_to_memory_access;

    if (p_to_write_back) {
        writeBack(p_to_write_back, p_reg_file);
    }
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

std::bitset<32> ControlUnit::fetch(std::shared_ptr<InstructionFile> _p_instruction_file, std::bitset<32> _pc)
{
    return _p_instruction_file->read(_pc);
}

void ControlUnit::decode(std::shared_ptr<RISC::Instruction>& _p_current_instruction, std::bitset<32> _current_instruction,
    std::shared_ptr<RegisterFile> _p_reg_file, std::shared_ptr<ImmGen> _p_imm_gen) {
    RISC::Instruction generic_instruction(_current_instruction);

    switch (generic_instruction.opcode.to_ulong()) {
    case 0b0110011: {
        // RType
        RISC::RType r_instruction(_current_instruction);
        if (r_instruction.funct3 == 0b000 && r_instruction.funct7 == 0b0000000) {
            // Add
            _p_current_instruction = std::make_shared<RISC::Add>(r_instruction);
        }
        else if (r_instruction.funct3 == 0b000 && r_instruction.funct7 == 0b0100000) {
            // Sub
            _p_current_instruction = std::make_shared<RISC::Sub>(r_instruction);
        }
        else if (r_instruction.funct3 == 0b001 && r_instruction.funct7 == 0b0000000) {
            // ShiftLeftLogi
            _p_current_instruction = std::make_shared<RISC::ShiftLeftLogi>(r_instruction);
        }
        else if (r_instruction.funct3 == 0b010 && r_instruction.funct7 == 0b0000000) {
            // SetLessThan
            _p_current_instruction = std::make_shared<RISC::SetLessThan>(r_instruction);
        }
        else if (r_instruction.funct3 == 0b011 && r_instruction.funct7 == 0b0000000) {
            // SetLessThanUnsigned
            _p_current_instruction = std::make_shared<RISC::SetLessThanUnsigned>(r_instruction);
        }
        else if (r_instruction.funct3 == 0b100 && r_instruction.funct7 == 0b0000000) {
            // Xor
            _p_current_instruction = std::make_shared<RISC::Xor>(r_instruction);
        }
        else if (r_instruction.funct3 == 0b101 && r_instruction.funct7 == 0b0000000) {
            // ShiftRightLogi 
            _p_current_instruction = std::make_shared<RISC::ShiftRightLogi>(r_instruction);
        }
        else if (r_instruction.funct3 == 0b101 && r_instruction.funct7 == 0b0100000) {
            // ShiftRightArith
            _p_current_instruction = std::make_shared<RISC::ShiftRightArith>(r_instruction);
        }
        else if (r_instruction.funct3 == 0b110 && r_instruction.funct7 == 0b0000000) {
            // Or
            _p_current_instruction = std::make_shared<RISC::Or>(r_instruction);
        }
        else if (r_instruction.funct3 == 0b111 && r_instruction.funct7 == 0b0000000) {
            // And
            _p_current_instruction = std::make_shared<RISC::And>(r_instruction);
        }
        else {
            throw std::runtime_error("Unknown funct3/funct7 combination: " + r_instruction.funct3.to_string() + "/" + r_instruction.funct7.to_string());
        }
        break;
    }
    case 0b0010011: {
        // IType
        RISC::IType i_instruction(_current_instruction);
        if (i_instruction.funct3 == 0b000) {
            // AddImm
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::AddImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b100) {
            // XorImm
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::XorImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b110) {
            // OrImm
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::OrImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b111) {
            // AndImm
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::AndImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b010) {
            // SetLessThanImm
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SetLessThanImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b011) {
            // SetLessThanImmUnsigned
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SetLessThanImmUnsigned(i_instruction));
        }
        else if (i_instruction.funct3 == 0b001 && i_instruction.funct7 == 0b0000000) {
            // ShiftLeftLogiImm
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftLeftLogiImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b101 && i_instruction.funct7 == 0b0000000) {
            // ShiftRightLogiImm
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftRightLogiImm(i_instruction));
        }
        else if (i_instruction.funct3 == 0b101 && i_instruction.funct7 == 0b0100000) {
            // ShiftRightArithImm
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::ShiftRightArithImm(i_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3: " + i_instruction.funct3.to_string());
        }
        break;
    }
    case 0b0000011: {
        // IType (Load)
        RISC::IType i_instruction(_current_instruction);
        if (i_instruction.funct3 == 0b010) {
            // LoadWord
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadWord(i_instruction));
        }
        else if (i_instruction.funct3 == 0b001) {
            // LoadHalfWord
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadHalfWord(i_instruction));
        }
        else if (i_instruction.funct3 == 0b000) {
            // LoadByte
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadByte(i_instruction));
        }
        else if (i_instruction.funct3 == 0b101) {
            // LoadUnsignedHalfWord
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadUnsignedHalfWord(i_instruction));
        }
        else if (i_instruction.funct3 == 0b100) {
            // LoadUnsignedByte
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadUnsignedByte(i_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3: " + i_instruction.funct3.to_string());
        }
        break;
    }
    case 0b0100011: {
        // SType 
        RISC::SType s_instruction(_current_instruction);
        if (s_instruction.funct3 == 0b000) {
            // SaveByte
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SaveByte(s_instruction));
        }
        else if (s_instruction.funct3 == 0b001) {
            // SaveHalfWord
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SaveHalfWord(s_instruction));
        }
        else if (s_instruction.funct3 == 0b010) {
            // SaveWord
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::SaveWord(s_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3: " + s_instruction.funct3.to_string());
        }
        break;
    }
    case 0b1100011: {
        // BType
        RISC::BType b_instruction(_current_instruction);
        if (b_instruction.funct3 == 0b000) {
            // BranchEqual
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchEqual(b_instruction));
        }
        else if (b_instruction.funct3 == 0b001) {
            // BranchNotEqual
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchNotEqual(b_instruction));
        }
        else if (b_instruction.funct3 == 0b100) {
            // BranchLessThan
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchLessThan(b_instruction));
        }
        else if (b_instruction.funct3 == 0b101) {
            // BranchGreaterThanOrEqual
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchGreaterThanEqual(b_instruction));
        }
        else if (b_instruction.funct3 == 0b110) {
            // BranchLessThanUnsigned
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchLessThanUnsigned(b_instruction));
        }
        else if (b_instruction.funct3 == 0b111) {
            // BranchGreaterThanOrEqualUnsigned
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::BranchGreaterThanEqualUnsigned(b_instruction));
        }
        else {
            throw std::runtime_error("Unknown funct3: " + b_instruction.funct3.to_string());
        }
        break;
    }
    case 0b0110111: {
        // UType (LoadUpperImmediate)
        _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::LoadUpperImmediate(_current_instruction));
        break;
    }
    case 0b0010111: {
        // UType (AUIPC)
        _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::AddUpperImmedateToPC(_current_instruction));
        break;
    }
    case 0b1101111: {
        // JType (JumpAndLink)
        _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::JumpAndLink(_current_instruction));
        break;
    }
    case 0b1100111: {
        // IType (JumpAndLinkRegister)
        _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::JumpAndLinkReg(_current_instruction));
        break;
    }
    case 0b1110011: {
        // System (IType)
        RISC::IType s_instruction(_current_instruction);

        if (s_instruction.imm == 0b000000000000) {
            // Ecall
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Ecall(s_instruction));
        }
        else if (s_instruction.imm == 0b000000000001) {
            // Ebreak
            _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Ebreak(s_instruction));
        }
        else {
            throw std::runtime_error("Unknown imm: " + s_instruction.imm.to_string());
        }
        break;
    }
    case 0b0001111: {
        // Fence
        _p_current_instruction = std::unique_ptr<RISC::Instruction>(new RISC::Fence(_current_instruction));
        break;
    }
    default:
        throw std::runtime_error("Unknown opcode: " + generic_instruction.opcode.to_string());
    }

    _p_current_instruction->decode(_p_reg_file, _p_imm_gen);
}

void ControlUnit::execute(std::shared_ptr<RISC::Instruction> _p_current_instruction, std::shared_ptr<ALU> _p_alu, std::bitset<32>& _pc) {
    _p_current_instruction->execute(_p_alu, _pc);
}

void ControlUnit::memoryAccess(std::shared_ptr<RISC::Instruction> _p_current_instruction, std::shared_ptr<MemoryFile> _p_data_file) {
    _p_current_instruction->accessMemory(_p_data_file);
}

void ControlUnit::writeBack(std::shared_ptr<RISC::Instruction> _p_current_instruction, std::shared_ptr<RegisterFile> _p_reg_file) {
    _p_current_instruction->writeBack(_p_reg_file);
}
