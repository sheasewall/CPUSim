#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <sstream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

struct Instruction
{
    int opcode;

    virtual ~Instruction() {}
    virtual void execute() = 0;
};

struct NoOp : Instruction
{
    void execute() override {}
    NoOp() { opcode = 0; }
};

struct Load : Instruction
{
    Register<int>* reg;
    int value;

    Load(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry)
    {
        std::string sReg;
        ss >> value >> sReg;
        reg = val_reg_registry.at(sReg);
        opcode = 1;
    }
    Load(Register<int>* _reg, int _value) : reg(_reg), value(_value) { opcode = 1; }
    void execute() override
    {
        reg->setVal(value);
    }
};

struct Add : Instruction
{
    Register<int>* reg;
    int value;

    Add(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry)
    {
        std::string sReg;
        ss >> value >> sReg;
        reg = val_reg_registry.at(sReg);
        opcode = 2;
    }
    Add(Register<int>* _reg, int _value) : reg(_reg), value(_value) { opcode = 1; }
    void execute() override
    {
        int reg_val = reg->getVal();
        reg->setVal(reg_val + value);
    }
};

struct Jump : Instruction
{
    unsigned int address;
    Register<unsigned int>* PC;

    Jump(std::istringstream& ss, Register<unsigned int>* _PC)
    {
        ss >> address;
        PC = _PC;
        opcode = 3;
    }
    void execute() override
    {
        PC->setVal(address);
    }
};

struct JumpIf : Jump
{
    Register<bool>* compare;

    JumpIf(std::istringstream& ss, Register<unsigned int>* PC, Register<bool>* _compare)
        : Jump(ss, PC), compare(_compare)
    {
        opcode = 4;
    }
    void execute() override
    {
        if (compare->getVal())
        {
            Jump::execute();
        }
    }
};

struct Comp : Instruction
{
    Register<int>* reg1;
    Register<int>* reg2;
    Register<bool>* compare;

    Comp(std::istringstream& ss, std::unordered_map<std::string, Register<int>*> val_reg_registry, Register<bool>* _compare)
        : compare(_compare)
    {
        std::string sReg1;
        std::string sReg2;
        ss >> sReg1 >> sReg2;
        reg1 = val_reg_registry.at(sReg1);
        reg2 = val_reg_registry.at(sReg2);
        opcode = 5;
    }
    void execute() override
    {
        if (reg1->getVal() == reg2->getVal())
        {
            compare->setVal(true);
        }
        else
        {
            compare->setVal(false);
        }
    }
};

struct Write : Instruction
{
    unsigned int address;
    std::string line_data;
    std::string memory_file;

    Write() { opcode = 6; }

    Write(std::istringstream& ss, std::string _memory_file) : memory_file(_memory_file)
    {
        ss >> address;
        std::getline(ss, line_data);
        std::cout << line_data << std::endl;
        opcode = 6;
    }
    void execute() override
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

                if (lineNumber == address)
                {
                    buffer << address << line_data << "\n";
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
            buffer << address << line_data << "\n";
        }

        std::ofstream outfile(memory_file);
        if (outfile.is_open())
        {
            outfile << buffer.str();
            outfile.close();
        }
    }
};

struct WriteFrom : Write
{
    Register<int>* reg;

    WriteFrom(std::istringstream& ss, std::string memory_file, std::unordered_map<std::string, Register<int>*> val_reg_registry)
    {
        std::string sReg;
        ss >> sReg;
        reg = val_reg_registry.at(sReg);

        this->memory_file = memory_file;
        ss >> address;
        std::getline(ss, line_data);

        std::string line = line_data;
        size_t pos = line.find('%');
        if (pos != std::string::npos)
        {
            line.replace(pos, 1, std::to_string(reg->getVal()));
        }

        line_data = line;
        opcode = 7;
    }
    void execute() override
    {
        Write::execute();
    }
};

#endif // INSTRUCTION_H