#ifndef RISCIINSTRUCTIONS_H
#define RISCIINSTRUCTIONS_H

#include <bitset>
#include <memory>
#include "alu.h"
#include "maskingunit.hpp"
#include "immgenunit.h"
#include "registerfile.h"
#include "memoryfile.h"
#include "instructionfile.h"
#include "exceptions.h"


namespace RISC {
/// RISC-V Types
    struct Instruction
    {
        // std::bitset<7> funct7;
        // std::bitset<3> funct3;

        // std::bitset<5> rd;
        // std::bitset<5> rs1;
        // std::bitset<5> rs2;
        // std::bitset<12> imm;
        // std::bitset<20> imm_long;

        // std::bitset<32> rs1_val;
        // std::bitset<32> rs2_val;
        // std::bitset<32> imm_val;
        // std::bitset<32> result;

    public:
        virtual ~Instruction() {}
        virtual void fetch(std::bitset<32> instruction, std::shared_ptr<MaskingUnit> p_mu) = 0;
        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGenUnit> p_igu) = 0;
        virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) = 0;
        virtual void accessMemory(std::shared_ptr<MemoryFile> p_data_file) = 0;
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) = 0;
    };


    // Register-Register operation
    struct RType : Instruction
    {
        std::bitset<5> rd;
        std::bitset<5> rs1;
        std::bitset<5> rs2;

        std::bitset<32> rs1_val;
        std::bitset<32> rs2_val;
        std::bitset<32> result;

        virtual void fetch(std::bitset<32> instruction, std::shared_ptr<MaskingUnit> p_mu) override;
        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGenUnit> p_igu) override;
        virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };


    // Immediate operations
    struct IType : Instruction
    {
        std::bitset<5> rd;
        std::bitset<5> rs1;
        std::bitset<12> imm;

        std::bitset<32> rs1_val;
        std::bitset<32> imm_val;
        std::bitset<32> result;

        virtual void fetch(std::bitset<32> instruction, std::shared_ptr<MaskingUnit> p_mu) override;
        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGenUnit> p_igu) override;
        virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };


    // Store operations
    struct SType : Instruction
    {
        std::bitset<5> rs1;
        std::bitset<5> rs2;
        std::bitset<12> imm;

        std::bitset<32> rs1_val;
        std::bitset<32> rs2_val;
        std::bitset<32> imm_val;

        std::bitset<32> result;

        virtual void fetch(std::bitset<32> instruction, std::shared_ptr<MaskingUnit> p_mu) override;
        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGenUnit> p_igu) override;
        virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
        void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override {}
    };


    // Branch operations
    struct BType : Instruction
    {
        std::bitset<5> rs1;
        std::bitset<5> rs2;
        std::bitset<12> imm;

        std::bitset<32> rs1_val;
        std::bitset<32> rs2_val;
        std::bitset<32> imm_val;

        std::bitset<32> result;

        virtual void fetch(std::bitset<32> instruction, std::shared_ptr<MaskingUnit> p_mu) override;
        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGenUnit> p_igu) override;
        virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override {}
    };


    // Upper immediate operations
    struct UType : Instruction
    {
        std::bitset<5> rd;
        std::bitset<20> imm_long;

        std::bitset<32> imm_val;

        std::bitset<32> result;

        virtual void fetch(std::bitset<32> instruction, std::shared_ptr<MaskingUnit> p_mu) override;
        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGenUnit> p_igu) override;
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override {}
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };


    // Jump operations
    struct JType : Instruction
    {
        std::bitset<5> rd;
        std::bitset<20> imm_long;

        std::bitset<32> imm_val;

        std::bitset<32> result;

        virtual void fetch(std::bitset<32> instruction, std::shared_ptr<MaskingUnit> p_mu) override;
        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGenUnit> p_igu) override;
        virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };

    // R instructions
    // add rd,rs1,rs2
    struct Add : RType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // sub rd,rs1,rs2
    struct Sub : RType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // xor rd,rs1,rs2
    struct Xor : RType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // or rd,rs1,rs2
    struct Or : RType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // and rd,rs1,rs2
    struct And : RType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // sll rd,rs1,rs2
    struct ShiftLeftLogi : RType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // srl rd,rs1,rs2
    struct ShiftRightLogi : RType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // sra rd,rs1,rs2
    struct ShiftRightArith : RType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // slt rd,rs1,rs2
    struct SetLessThan : RType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // sltu rd,rs1,rs2
    struct SetLessThanUnsigned : RType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // I instructions
    // addi rd,rs1,imm
    struct AddImm : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // xori rd,rs1,imm
    struct XorImm : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // ori rd,rs1,imm
    struct OrImm : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // andi rd,rs1,imm
    struct AndImm : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // slli rd,rs1,imm
    struct ShiftLeftLogiImm : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // srli rd,rs1,imm
    struct ShiftRightLogiImm : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // srai rd,rs1,imm
    struct ShiftRightArithImm : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // slti rd,rs1,imm
    struct SetLessThanImm : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // sltui rd,rs1,imm
    struct SetLessThanImmUnsigned : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    struct Ecall : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    struct Ebreak : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    /// Save instructions
    // sw rs2,offset(rs1)
    struct SaveWord : SType {
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // sh rs2,offset(rs1)
    struct SaveHalfWord : SType {
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // sb rs2,offset(rs1)
    struct SaveByte : SType {
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // Load instructions
    // lw rd,offset(rs1)
    struct LoadWord : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // lh rd,offset(rs1)
    struct LoadHalfWord : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // lb rd,offset(rs1)
    struct LoadByte : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // lhu rd,offset(rs1)
    struct LoadUnsignedHalfWord : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // lbu rd,offset(rs1)
    struct LoadUnsignedByte : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // B instructions
    // beq rs1,rs2,offset
    struct BranchEqual : BType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // bne rs1,rs2,offset
    struct BranchNotEqual : BType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // blt rs1,rs2,offset
    struct BranchLessThan : BType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // bltu rs1,rs2,offset
    struct BranchLessThanUnsigned : BType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // bge rs1,rs2,offset
    struct BranchGreaterThanEqual : BType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // bgeu rs1,rs2,offset
    struct BranchGreaterThanEqualUnsigned : BType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // U instructions
    // lui rd,imm
    struct LoadUpperImmediate : UType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // auipc rd,imm
    struct AddUpperImmedateToPC : UType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // jal rd,offset
    struct JumpAndLink : JType {
        using JType::execute;
        using JType::writeBack;
    };

    // jalr rd,offset
    struct JumpAndLinkReg : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };

    // fence
    // this is currently a no op as our simulator is single hart
    struct Fence : IType {
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };
}

#endif // RISCIINSTRUCTIONS_H