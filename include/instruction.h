#ifndef INSTRUCTION_H
#define INSTRUCTION_H

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

struct JumpIf : OperandInstruction
{
    Register<unsigned int>* PC;
    Register<bool>* compare;

    JumpIf(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry, Register<unsigned int>* _PC, Register<bool>* _compare)
        : OperandInstruction(ss, val_reg_registry), PC(_PC), compare(_compare) {
    }

    void execute() override
    {
        int val = operand.getVal();
        if (val < 0) {
            throw std::out_of_range("Jump address cannot be negative.");
        }
        if (compare->getVal())
        {
            PC->setVal(val);
        }
    }
};

struct Write : OperandAndOperandInstruction
{
    std::string memory_file;

    Write(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry, std::string _memory_file)
        : OperandAndOperandInstruction(ss, val_reg_registry), memory_file(_memory_file) {
    }

    void execute() override
    {
        {
            std::ifstream infile(memory_file);
            std::stringstream buffer;
            bool found = false;

            if (infile.is_open())
            {
                std::string line;
                while (std::getline(infile, line))
                {
                    std::istringstream iss(line);
                    unsigned int lineNumber;
                    iss >> lineNumber;

                    if (lineNumber == leftOperand.getVal())
                    {
                        buffer << leftOperand.getVal() << " " << rightOperand.getVal() << "\n";
                        found = true;
                    }
                    else
                    {
                        buffer << line << "\n";
                    }
                }
                infile.close();
            }

            if (!found)
            {
                buffer << leftOperand.getVal() << " " << rightOperand.getVal() << "\n";
            }

            std::ofstream outfile(memory_file);
            if (outfile.is_open())
            {
                outfile << buffer.str();
                outfile.close();
            }
        }
    };

        // void execute() override
        // {
        //     std::fstream file(memory_file, std::ios::in | std::ios::out);
        //     bool found = false;
        //     std::streampos pos;

        //     if (!file.is_open()) {
        //         std::cerr << "Unable to open " << memory_file << std::endl;
        //         return;
        //     }

        //     std::string line;
        //     while (std::getline(file, line) && !found)
        //     {
        //         std::istringstream iss(line);
        //         int lineNumber;
        //         iss >> lineNumber;

        //         int targetLineNumber = leftOperand.getVal();
        //         int targetValue = rightOperand.getVal();

        //         if (lineNumber == targetLineNumber)
        //         {
        //             found = true;
        //             pos = file.tellg();
        //             if (file.peek() == EOF) {
        //                 file.clear();
        //                 file.seekg(-std::streamoff(line.length()), std::ios::end);
        //             }
        //             else {
        //                 file.seekg(pos - std::streamoff(line.length() + 1));
        //             }
        //             file << targetLineNumber << " " << targetValue;
        //             file.flush();
        //             break;
        //         }
        //     }

        //     if (!found)
        //     {
        //         file.clear();
        //         file.seekp(0, std::ios::end);
        //         if (file.tellp() != std::streampos(0)) {
        //             file << "\n";
        //         }
        //         file << leftOperand.getVal() << " " << rightOperand.getVal();
        //     }

        //     file.close();
        // }
};

struct Read : OperandAndRegInstruction
{
    std::string memory_file;

    Read(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry, std::string _memory_file)
        : OperandAndRegInstruction(ss, val_reg_registry), memory_file(_memory_file) {
    }

    void execute() override
    {
        std::ifstream file(memory_file);
        // When I read here, it scrambles my leftOperand
        if (!file.is_open()) {
            std::cerr << "Unable to open " << memory_file << std::endl;
            return;
        }

        std::string line;
        int targetLineNumber = leftOperand.getVal();
        bool found = false;

        while (std::getline(file, line) && !found)
        {
            std::istringstream iss(line);
            int lineNumber;
            iss >> lineNumber;

            if (lineNumber == targetLineNumber)
            {
                int value;
                iss >> value;
                rightReg->setVal(value);
                found = true;
            }
        }

        if (!found)
        {
            throw std::out_of_range("Memory address not found.");
        }
        file.close();
    }
};

#endif // INSTRUCTION_H