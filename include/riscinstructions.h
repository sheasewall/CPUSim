#ifndef RISCIINSTRUCTIONS_H
#define RISCIINSTRUCTIONS_H

#include <bitset>
#include <memory>
#include "alu.h"
#include "immgen.h"
#include "registerfile.h"
#include "memoryfile.h"
#include "instructionfile.h"

namespace RISC {
/// RISC-V Types
    struct Instruction
    {
        std::bitset<7> opcode;

    public:
        virtual ~Instruction() {}
        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen) {}
        virtual void execute(std::shared_ptr<ALU> p_alu) {}
        virtual void accessMemory(std::shared_ptr<MemoryFile> p_data_file) {}
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) {}

        Instruction(std::bitset<32> instruction);
    };


    // Register-Register operation
    struct RType : Instruction
    {
        std::bitset<7> funct7;
        std::bitset<3> funct3;

        std::bitset<5> rd;
        std::bitset<5> rs1;
        std::bitset<5> rs2;

        std::bitset<32> rs1_val;
        std::bitset<32> rs2_val;
        std::bitset<32> result;

        RType(std::bitset<32> instruction);

        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen) override;
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };

    struct Add : RType
    {
        Add(RType instruction) : RType(instruction) {}
        Add(std::bitset<32> instruction) : RType(instruction) {}

        void execute(std::shared_ptr<ALU> p_alu) override {
            result = p_alu->add(rs1_val, rs2_val);
        }
    };


    // Immediate operations
    struct IType : Instruction
    {
        std::bitset<3> funct3;
        std::bitset<5> rd;
        std::bitset<5> rs1;
        std::bitset<12> imm;

        std::bitset<32> rs1_val;
        std::bitset<32> imm_val;
        std::bitset<32> result;

        IType(std::bitset<32> instruction);
        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen) override;
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };

    // Add with immediate
    struct AddI : IType
    {
        AddI(IType instruction) : IType(instruction) {}
        AddI(std::bitset<32> instruction) : IType(instruction) {}

        void execute(std::shared_ptr<ALU> p_alu) override
        {
            result = p_alu->add(rs1_val, imm_val);
        }
    };

    // Load word 
    struct LoadW : IType
    {
        LoadW(IType instruction) : IType(instruction) {}
        LoadW(std::bitset<32> instruction) : IType(instruction) {}

        void execute(std::shared_ptr<ALU> p_alu) override
        {
            result = p_alu->add(rs1_val, imm_val);
        }

        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override
        {
            result = p_data_file->readWord(result);
        }
    };


    // Store operations
    struct SType : Instruction
    {
        std::bitset<3> funct3;
        std::bitset<5> rs1;
        std::bitset<5> rs2;
        std::bitset<12> imm;

        std::bitset<32> rs1_val;
        std::bitset<32> rs2_val;
        std::bitset<32> imm_val;

        std::bitset<32> result;

        SType(std::bitset<32> instruction);

        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen) override;
        // How do I want to write to memory? 
        // Technically, depending on the width
        // I need 1, 2, or 4 addresses
        // I mean really I just need the 1 address
        // and then to increment
        // But it needs to rollover, so I need some 
        // amount of logic overhead
    };

    // Save word
    struct SaveW : SType
    {
        SaveW(SType instruction) : SType(instruction) {}
        SaveW(std::bitset<32> instruction) : SType(instruction) {}

        void execute(std::shared_ptr<ALU> p_alu) override
        {
            result = p_alu->add(rs1_val, imm_val);
        }

        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override
        {
            p_data_file->writeWord(result, rs2_val);
        }
    };


    // Branch operations
    struct BType : Instruction
    {
        std::bitset<3> funct3;
        std::bitset<5> rs1;
        std::bitset<5> rs2;
        // This value represents a multiple of 2, meaning 
        // it must be shifted left by 1 to get the actual value
        // This will be done in the execute method
        std::bitset<12> imm;

        BType(std::bitset<32> instruction);
    };


    // Upper immediate operations
    struct UType : Instruction
    {
        std::bitset<5> rd;
        std::bitset<20> imm;

        std::bitset<32> imm_val;

        std::bitset<32> result;

        UType(std::bitset<32> instruction);

        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen) override;
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };

    struct LUI : UType
    {
        LUI(UType instruction) : UType(instruction) {}
        LUI(std::bitset<32> instruction) : UType(instruction) {}

        void execute(std::shared_ptr<ALU> p_alu) override
        {
            result = imm_val;
        }
    };


    // Jump operations
    struct JType : Instruction
    {
        std::bitset<5> rd;
        // This value represents a multiple of 2, meaning 
        // it must be shifted left by 1 to get the actual value
        // This will be done in the execute method
        std::bitset<20> imm;

        JType(std::bitset<32> instruction);
    };
}

#endif // RISCIINSTRUCTIONS_H