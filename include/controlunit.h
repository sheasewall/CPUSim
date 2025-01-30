#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include "register.h"
#include "instruction.h"
#include "alu.h"
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

class ControlUnit {
    // Pipeline registers definitions
    struct IFID {
        std::bitset<32> instruction;
        std::bitset<32> pc;
    };

    struct IDEX {
        std::bitset<32> pc;
        std::bitset<32> imm;
        std::bitset<32> reg1;
        std::bitset<32> reg2;
        std::bitset<5> write_reg;
    };

    struct EXMEM {
        std::bitset<32> result;
        std::bitset<32> reg2;
        std::bitset<32> jump_target;
        std::bitset<5> write_reg;
    };

    struct MEMWB {
        std::bitset<32> result;
        std::bitset<32> read_data;
        std::bitset<5> write_reg;
    };

    // Memory file definitions
    class InstructionMemory {
        std::map<std::bitset<32>, std::bitset<32>> instructions;
    public:
        InstructionMemory(std::string instruction_file) {
            std::ifstream file(instruction_file);
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string address;
                std::string instruction;
                iss >> address >> instruction;
                instructions.insert({ std::bitset<32>(std::stoi(address)), std::bitset<32>(std::stoi(instruction)) });
            }
        }

        std::bitset<32> read(std::bitset<32> address) {
            return instructions.at(address);
        }
    };

    class RegisterFile {
        std::map<std::bitset<5>, std::bitset<32>> registers;

    public:
        RegisterFile() {
            for (int i = 0; i < 32; i++) {
                registers.insert({ std::bitset<5>(i), std::bitset<32>(0) });
            }
        }

        std::pair<std::bitset<32>, std::bitset<32>> read(std::bitset<5> reg1, std::bitset<5> reg2) {
            return std::pair{ registers.at(reg1), registers.at(reg2) };
        }

        void write(std::bitset<5> reg, std::bitset<32> value) {
            registers.at(reg) = value;
        }
    };

    class DataMemory {
        std::map<std::bitset<32>, std::bitset<8>> data;

    public:
        DataMemory(std::string memory_file) {
            std::ifstream file(memory_file);
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string address;
                std::string value;
                iss >> address >> value;
                data.insert({ std::bitset<32>(std::stoi(address)), std::bitset<8>(std::stoi(value)) });
            }
        }

        std::bitset<8> read(std::bitset<32> address) {
            return data.at(address);
        }

        void write(std::bitset<32> address, std::bitset<8> value) {
            data.at(address) = value;
        }
    };
    std::bitset<32> pc;

    InstructionMemory instruction_memory;
    IFID if_id;
    RegisterFile register_file;
    IDEX id_ex;
    ALU alu;
    EXMEM ex_mem;
    DataMemory data_memory;
    MEMWB mem_wb;
    
public:
    ControlUnit(std::string _memory_file);
    ~ControlUnit() {}

    void step();

private:
    void readMemory();

    void fetch();
    void decode();
    void execute();
    void memoryAccess();
    void writeBack();
};

#endif // CONTROLUNIT_H