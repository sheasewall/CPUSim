#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "fileiounit.hpp"

#include <sstream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

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