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
        virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) {}
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

        std::bitset<32> rs1_val;
        std::bitset<32> rs2_val;
        std::bitset<32> imm_val;

        std::bitset<32> result;

        BType(std::bitset<32> instruction);

        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen) override;
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override {}
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


    // Jump operations
    struct JType : Instruction
    {
        std::bitset<5> rd;
        // This value represents a multiple of 2, meaning 
        // it must be shifted left by 1 to get the actual value
        // This will be done in the execute method
        std::bitset<20> imm;

        std::bitset<32> imm_val;

        std::bitset<32> result;

        JType(std::bitset<32> instruction);

        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen) override;
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };


    struct Add : RType {
        Add(RType instruction) : RType(instruction) {}
        Add(std::bitset<32> instruction) : RType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    struct AddImm : IType {
        AddImm(IType instruction) : IType(instruction) {}
        AddImm(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    struct LoadWord : IType {
        LoadWord(IType instruction) : IType(instruction) {}
        LoadWord(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    struct SaveWord : SType {
        SaveWord(SType instruction) : SType(instruction) {}
        SaveWord(std::bitset<32> instruction) : SType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    struct BranchEqual : BType {
        BranchEqual(BType instruction) : BType(instruction) {}
        BranchEqual(std::bitset<32> instruction) : BType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    struct LoadUpperImmediate : UType {
        LoadUpperImmediate(UType instruction) : UType(instruction) {}
        LoadUpperImmediate(std::bitset<32> instruction) : UType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    struct JumpAndLink : JType {
        JumpAndLink(JType instruction) : JType(instruction) {}
        JumpAndLink(std::bitset<32> instruction) : JType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };
}

#endif // RISCIINSTRUCTIONS_H