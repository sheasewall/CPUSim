#include "riscinstructions.h"

using namespace RISC;

RISCInstruction::RISCInstruction(std::bitset<32> instruction)
{
    std::string instr = instruction.to_string();

    opcode = std::bitset<7>(instr.substr(25, 7));
}


// RType
RType::RType(std::bitset<32> instruction) : RISCInstruction(instruction)
{
    std::string instr = instruction.to_string();

    rs2 = std::bitset<5>(instr.substr(7, 5));
    rs1 = std::bitset<5>(instr.substr(12, 5));
    rd = std::bitset<5>(instr.substr(20, 5));
}

void RType::decode(std::shared_ptr<RegisterFile> reg_file)
{
    std::pair<std::bitset<32>, std::bitset<32>> registers = reg_file->read(rs1, rs2);
    rs1_val = registers.first;
    rs2_val = registers.second;
}

void RType::writeBack(std::shared_ptr<RegisterFile> p_reg_file)
{
    p_reg_file->write(rd, result);
}

// Add RType
struct Add : RType
{
    Add(std::bitset<32> instruction) : RType(instruction) {}

    void execute(std::shared_ptr<ALU> p_alu) override
    {
        result = p_alu->add(rs1_val, rs2_val);
    }
};


// IType
IType::IType(std::bitset<32> instruction) : RISCInstruction(instruction)
{
    std::string instr = instruction.to_string();

    imm = std::bitset<12>(instr.substr(0, 12));
    rs1 = std::bitset<5>(instr.substr(12, 5));
    rd = std::bitset<5>(instr.substr(20, 5));
}

void RISC::IType::decode(std::shared_ptr<RegisterFile> reg_file)
{
    std::pair<std::bitset<32>, std::bitset<32>> registers = reg_file->read(rs1, 0);
    rs1_val = registers.first;
}

void RISC::IType::writeBack(std::shared_ptr<RegisterFile> p_reg_file)
{
    p_reg_file->write(rd, result);
}

// Addi IType
struct Addi : IType
{
    Addi(std::bitset<32> instruction) : IType(instruction) {}

    void execute(std::shared_ptr<ALU> p_alu) override
    {
        result = p_alu->addImmediate(rs1_val, imm);
    }
};


// SType
SType::SType(std::bitset<32> instruction) : RISCInstruction(instruction)
{
    std::string instr = instruction.to_string();

    std::bitset<7> imm1 = std::bitset<7>(instr.substr(0, 7));
    rs2 = std::bitset<5>(instr.substr(7, 5));
    rs1 = std::bitset<5>(instr.substr(12, 5));
    std::bitset<5> imm0 = std::bitset<5>(instr.substr(20, 5));

    imm = std::bitset<12>(imm1.to_string() + imm0.to_string());
}


// BType
BType::BType(std::bitset<32> instruction) : RISCInstruction(instruction)
{
    std::string instr = instruction.to_string();

    std::bitset<1> imm3 = std::bitset<1>(instr.substr(0, 1));
    std::bitset<6> imm1 = std::bitset<6>(instr.substr(1, 6));
    rs2 = std::bitset<5>(instr.substr(7, 5));
    rs1 = std::bitset<5>(instr.substr(12, 5));
    std::bitset<4> imm0 = std::bitset<4>(instr.substr(20, 4));
    std::bitset<1> imm2 = std::bitset<1>(instr.substr(24, 1));

    imm = std::bitset<12>(imm3.to_string() + imm2.to_string() + imm1.to_string() + imm0.to_string());
}


// UType
UType::UType(std::bitset<32> instruction) : RISCInstruction(instruction)
{
    std::string instr = instruction.to_string();

    imm = std::bitset<20>(instr.substr(0, 20));
    rd = std::bitset<5>(instr.substr(20, 5));
}


// JType
JType::JType(std::bitset<32> instruction) : RISCInstruction(instruction)
{
    std::string instr = instruction.to_string();

    std::bitset<1> imm3 = std::bitset<1>(instr.substr(0, 1));
    std::bitset<10> imm0 = std::bitset<10>(instr.substr(1, 10));
    std::bitset<1> imm1 = std::bitset<1>(instr.substr(11, 1));
    std::bitset<8> imm2 = std::bitset<8>(instr.substr(12, 8));
    rd = std::bitset<5>(instr.substr(20, 5));

    imm = std::bitset<20>(imm3.to_string() + imm2.to_string() + imm1.to_string() + imm0.to_string());
}
