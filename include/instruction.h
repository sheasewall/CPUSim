#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "fileiounit.hpp"
#include "register.h"

#include <sstream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>

using Reg = Register<int>*;

struct Operand
{
    int val;
    Reg reg;

    Operand() {}

    Operand(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry)
    {
        std::string source;
        ss >> source;
        try
        {
            val = std::stoi(source);
            reg = nullptr;
        }
        catch (std::invalid_argument)
        {
            reg = val_reg_registry.at(source);
        }
    }

    int getVal()
    {
        if (reg != nullptr)
        {
            return reg->getVal();
        }
        return val;
    }
};

struct Instruction
{
    virtual ~Instruction() {}
    virtual void execute() = 0;
};

/// RISC-V Types

// Register-Register operation
struct RType : Instruction 
{
    std::bitset<7> opcode;
    std::bitset<5> rd;
    std::bitset<3> funct3;
    std::bitset<5> rs1;
    std::bitset<5> rs2;
    std::bitset<7> funct7;

    RType(std::bitset<32> instruction)
    {
        std::string instr = instruction.to_string();

        funct7 = std::bitset<7>(instr.substr(0, 7));
        rs2 = std::bitset<5>(instr.substr(7, 5));
        rs1 = std::bitset<5>(instr.substr(12, 5));
        funct3 = std::bitset<3>(instr.substr(17, 3));
        rd = std::bitset<5>(instr.substr(20, 5));
        opcode = std::bitset<7>(instr.substr(25, 7));
    }

    void execute() override {}
};

// Immediate operations
struct IType : Instruction 
{
    std::bitset<7> opcode;
    std::bitset<5> rd;
    std::bitset<3> funct3;
    std::bitset<5> rs1;
    std::bitset<12> imm;

    IType(std::bitset<32> instruction)
    {
        std::string instr = instruction.to_string();

        imm = std::bitset<12>(instr.substr(0, 12));
        rs1 = std::bitset<5>(instr.substr(12, 5));
        funct3 = std::bitset<3>(instr.substr(17, 3));
        rd = std::bitset<5>(instr.substr(20, 5));
        opcode = std::bitset<7>(instr.substr(25, 7));
    }

    void execute() override {}
};

// Store operations
struct SType : Instruction 
{
    std::bitset<7> opcode;
    std::bitset<3> funct3;
    std::bitset<5> rs1;
    std::bitset<5> rs2;
    std::bitset<12> imm;

    SType(std::bitset<32> instruction)
    {
        std::string instr = instruction.to_string();

        std::bitset<7> imm1 = std::bitset<7>(instr.substr(0, 7));
        rs2 = std::bitset<5>(instr.substr(7, 5));
        rs1 = std::bitset<5>(instr.substr(12, 5));
        funct3 = std::bitset<3>(instr.substr(17, 3));
        std::bitset<5> imm0 = std::bitset<5>(instr.substr(20, 5));
        opcode = std::bitset<7>(instr.substr(25, 7));

        imm = std::bitset<12>(imm1.to_string() + imm0.to_string());
    }

    void execute() override {}
};

// Branch operations
struct BType : Instruction 
{
    std::bitset<7> opcode;
    std::bitset<3> funct3;
    std::bitset<5> rs1;
    std::bitset<5> rs2;
    // This value represents a multiple of 2, meaning 
    // it must be shifted left by 1 to get the actual value
    // This will be done in the execute method
    std::bitset<12> imm; 

    BType(std::bitset<32> instruction)
    {
        std::string instr = instruction.to_string();

        std::bitset<1> imm3 = std::bitset<1>(instr.substr(0, 1));
        std::bitset<6> imm1 = std::bitset<6>(instr.substr(1, 6));
        rs2 = std::bitset<5>(instr.substr(7, 5));
        rs1 = std::bitset<5>(instr.substr(12, 5));
        funct3 = std::bitset<3>(instr.substr(17, 3));
        std::bitset<4> imm0 = std::bitset<4>(instr.substr(20, 4));
        std::bitset<1> imm2 = std::bitset<1>(instr.substr(24, 1));
        opcode = std::bitset<7>(instr.substr(25, 7));

        imm = std::bitset<12>(imm3.to_string() + imm2.to_string() + imm1.to_string() + imm0.to_string());
    }

    void execute() override {}
};

// Upper immediate operations
struct UType : Instruction 
{
    std::bitset<7> opcode;
    std::bitset<5> rd;
    std::bitset<20> imm;

    UType(std::bitset<32> instruction)
    {
        std::string instr = instruction.to_string();

        imm = std::bitset<20>(instr.substr(0, 20));
        rd = std::bitset<5>(instr.substr(20, 5));
        opcode = std::bitset<7>(instr.substr(25, 7));
    }

    void execute() override {}
};

// Jump operations
struct JType : Instruction 
{
    std::bitset<7> opcode;
    std::bitset<5> rd;
    // This value represents a multiple of 2, meaning 
    // it must be shifted left by 1 to get the actual value
    // This will be done in the execute method
    std::bitset<20> imm; 

    JType(std::bitset<32> instruction)
    {
        std::string instr = instruction.to_string();

        std::bitset<1> imm3 = std::bitset<1>(instr.substr(0, 1));
        std::bitset<10> imm0 = std::bitset<10>(instr.substr(1, 10));
        std::bitset<1> imm1 = std::bitset<1>(instr.substr(11, 1));
        std::bitset<8> imm2 = std::bitset<8>(instr.substr(12, 8));
        rd = std::bitset<5>(instr.substr(20, 5));
        opcode = std::bitset<7>(instr.substr(25, 7));

        imm = std::bitset<20>(imm3.to_string() + imm2.to_string() + imm1.to_string() + imm0.to_string());
    }

    void execute() override {}
};

///

struct OperandInstruction : Instruction
{
    Operand operand;

    OperandInstruction(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry)
    {
        operand = Operand(ss, val_reg_registry);
    }
};

struct OperandAndRegInstruction : OperandInstruction
{
    // I want to alias this rather than copy it, but 
    // using a & reference was incorrect usage
    Operand leftOperand = operand;
    Reg rightReg;

    OperandAndRegInstruction(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry)
        : OperandInstruction(ss, val_reg_registry)
    {
        std::string sRightReg;
        ss >> sRightReg;
        rightReg = val_reg_registry.at(sRightReg);
    }
};

struct OperandAndOperandInstruction : OperandInstruction
{
    // Aliasing leftReg to OperandInstruction::reg
    Operand& leftOperand = operand;
    Operand rightOperand;

    OperandAndOperandInstruction(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry)
        : OperandInstruction(ss, val_reg_registry)
    {
        rightOperand = Operand(ss, val_reg_registry);
    }
};

///

struct DoNothing : Instruction
{
    DoNothing() {}
    void execute() override {}
};

struct Move : OperandAndRegInstruction
{
    Move(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry)
        : OperandAndRegInstruction(ss, val_reg_registry) {
    }

    void execute() override
    {
        rightReg->setVal(leftOperand.getVal());
    }
};

struct Add : OperandAndRegInstruction
{
    Add(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry)
        : OperandAndRegInstruction(ss, val_reg_registry) {
    }

    void execute() override
    {
        rightReg->setVal(leftOperand.getVal() + rightReg->getVal());
    }
};

struct LessThanComp : OperandAndOperandInstruction
{
    Register<bool>* compare;

    LessThanComp(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry, Register<bool>* _compare)
        : OperandAndOperandInstruction(ss, val_reg_registry), compare(_compare) {
    }

    void execute() override
    {
        if (leftOperand.getVal() < rightOperand.getVal())
        {
            compare->setVal(true);
        }
        else
        {
            compare->setVal(false);
        }
    }
};

struct Jump : OperandInstruction
{
    Register<unsigned int>* PC;

    Jump(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry, Register<unsigned int>* _PC)
        : OperandInstruction(ss, val_reg_registry), PC(_PC) {
    }

    void execute() override
    {
        int val = operand.getVal();
        if (val < 0) {
            throw std::out_of_range("Jump address cannot be negative.");
        }
        PC->setVal(val);
    }
};

struct JumpIf : Jump
{
    Register<bool>* compare;

    JumpIf(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry, Register<unsigned int>* _PC, Register<bool>* _compare)
        : Jump(ss, val_reg_registry, _PC), compare(_compare) {
    }

    void execute() override
    {
        if (compare->getVal())
        {
            Jump::execute();
        }
    }
};

struct Clear : OperandInstruction
{
    std::string memory_file;

    Clear(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry, std::string _memory_file)
        : OperandInstruction(ss, val_reg_registry), memory_file(_memory_file) {
    }

    void execute() override {
        try {
            FileIOUnit::clearLineFromFile(memory_file, operand.getVal());
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

struct Write : OperandAndOperandInstruction {
    std::string memory_file;

    Write(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry, std::string _memory_file)
        : OperandAndOperandInstruction(ss, val_reg_registry), memory_file(_memory_file) {
    }

    void execute() override {
        try {
            FileIOUnit::writeLineToFile<int>(memory_file, leftOperand.getVal(), rightOperand.getVal());
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

struct Read : OperandAndRegInstruction {
    std::string memory_file;

    Read(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry, std::string _memory_file)
        : OperandAndRegInstruction(ss, val_reg_registry), memory_file(_memory_file) {
    }

    void execute() override {
        try {
            rightReg->setVal(FileIOUnit::readLineFromFile(memory_file, leftOperand.getVal()));
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

struct Append : OperandAndOperandInstruction {
    std::string memory_file;

    Append(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry, std::string _memory_file)
        : OperandAndOperandInstruction(ss, val_reg_registry), memory_file(_memory_file) {
    }

    void execute() override {
        try {
            FileIOUnit::appendToFile<char>(memory_file, leftOperand.getVal(), char(rightOperand.getVal()));
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

struct ReadChar : OperandAndRegInstruction {
    std::string memory_file;

    ReadChar(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry, std::string _memory_file)
        : OperandAndRegInstruction(ss, val_reg_registry), memory_file(_memory_file) {
    }

    void execute() override {
        try {
            rightReg->setVal(int(FileIOUnit::destructiveRead(memory_file, leftOperand.getVal())));
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

#endif // INSTRUCTION_H