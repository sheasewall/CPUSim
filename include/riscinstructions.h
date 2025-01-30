#ifndef RISCIINSTRUCTIONS_H
#define RISCIINSTRUCTIONS_H

#include <bitset>
#include <memory>
#include "alu.h"
#include "registerfile.h"
#include "memoryfile.h"
#include "instructionfile.h"

namespace RISC {
/// RISC-V Types
    struct RISCInstruction
    {
    private:
        std::bitset<7> opcode;

    public:
        virtual ~RISCInstruction() {}
        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file) {}
        virtual void execute(std::shared_ptr<ALU> p_alu) {}
        virtual void accessMemory(std::shared_ptr<DataMemory> p_data_mem) {}
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) {}

        RISCInstruction(std::bitset<32> instruction);
    };

    // Register-Register operation
    struct RType : RISCInstruction
    {
        std::bitset<5> rd;
        std::bitset<5> rs1;
        std::bitset<5> rs2;

        std::bitset<32> rs1_val;
        std::bitset<32> rs2_val;
        std::bitset<32> result;

        RType(std::bitset<32> instruction);

        virtual void decode(std::shared_ptr<RegisterFile> reg_file) override;
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };

    // Immediate operations
    struct IType : RISCInstruction
    {
        std::bitset<5> rd;
        std::bitset<5> rs1;
        std::bitset<12> imm;

        std::bitset<32> rs1_val;
        std::bitset<32> result;

        IType(std::bitset<32> instruction);
        virtual void decode(std::shared_ptr<RegisterFile> reg_file) override;
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };

    // Store operations
    struct SType : RISCInstruction
    {
        std::bitset<5> rs1;
        std::bitset<5> rs2;
        std::bitset<12> imm;

        SType(std::bitset<32> instruction);
    };

    // Branch operations
    struct BType : RISCInstruction
    {
        std::bitset<5> rs1;
        std::bitset<5> rs2;
        // This value represents a multiple of 2, meaning 
        // it must be shifted left by 1 to get the actual value
        // This will be done in the execute method
        std::bitset<12> imm;

        BType(std::bitset<32> instruction);
    };

    // Upper immediate operations
    struct UType : RISCInstruction
    {
        std::bitset<5> rd;
        std::bitset<20> imm;

        UType(std::bitset<32> instruction);
    };

    // Jump operations
    struct JType : RISCInstruction
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