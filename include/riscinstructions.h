#ifndef RISCIINSTRUCTIONS_H
#define RISCIINSTRUCTIONS_H

#include <bitset>
#include <memory>
#include "alu.h"
#include "immgen.h"
#include "registerfile.h"
#include "memoryfile.h"
#include "instructionfile.h"
#include "exceptions.h"


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
        virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };


    // Immediate operations
    struct IType : Instruction
    {
        std::bitset<7> funct7;
        std::bitset<3> funct3;
        std::bitset<5> rd;
        std::bitset<5> rs1;
        std::bitset<12> imm;

        std::bitset<32> rs1_val;
        std::bitset<32> imm_val;
        std::bitset<32> result;

        IType(std::bitset<32> instruction);
        virtual void decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen) override;
        virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
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
        virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
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
        virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
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
        virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };

    // R instructions
    // add rd,rs1,rs2
    struct Add : RType {
        Add(RType instruction) : RType(instruction) {}
        Add(std::bitset<32> instruction) : RType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // sub rd,rs1,rs2
    struct Sub : RType {
        Sub(RType instruction) : RType(instruction) {}
        Sub(std::bitset<32> instruction) : RType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // xor rd,rs1,rs2
    struct Xor : RType {
        Xor(RType instruction) : RType(instruction) {}
        Xor(std::bitset<32> instruction) : RType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // or rd,rs1,rs2
    struct Or : RType {
        Or(RType instruction) : RType(instruction) {}
        Or(std::bitset<32> instruction) : RType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // and rd,rs1,rs2
    struct And : RType {
        And(RType instruction) : RType(instruction) {}
        And(std::bitset<32> instruction) : RType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // sll rd,rs1,rs2
    struct ShiftLeftLogi : RType {
        ShiftLeftLogi(RType instruction) : RType(instruction) {}
        ShiftLeftLogi(std::bitset<32> instruction) : RType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // srl rd,rs1,rs2
    struct ShiftRightLogi : RType {
        ShiftRightLogi(RType instruction) : RType(instruction) {}
        ShiftRightLogi(std::bitset<32> instruction) : RType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // sra rd,rs1,rs2
    struct ShiftRightArith : RType {
        ShiftRightArith(RType instruction) : RType(instruction) {}
        ShiftRightArith(std::bitset<32> instruction) : RType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // slt rd,rs1,rs2
    struct SetLessThan : RType {
        SetLessThan(RType instruction) : RType(instruction) {}
        SetLessThan(std::bitset<32> instruction) : RType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // sltu rd,rs1,rs2
    struct SetLessThanUnsigned : RType {
        SetLessThanUnsigned(RType instruction) : RType(instruction) {}
        SetLessThanUnsigned(std::bitset<32> instruction) : RType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // I instructions
    // addi rd,rs1,imm
    struct AddImm : IType {
        AddImm(IType instruction) : IType(instruction) {}
        AddImm(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // xori rd,rs1,imm
    struct XorImm : IType {
        XorImm(IType instruction) : IType(instruction) {}
        XorImm(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // ori rd,rs1,imm
    struct OrImm : IType {
        OrImm(IType instruction) : IType(instruction) {}
        OrImm(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // andi rd,rs1,imm
    struct AndImm : IType {
        AndImm(IType instruction) : IType(instruction) {}
        AndImm(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // slli rd,rs1,imm
    struct ShiftLeftLogiImm : IType {
        ShiftLeftLogiImm(IType instruction) : IType(instruction) {}
        ShiftLeftLogiImm(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // srli rd,rs1,imm
    struct ShiftRightLogiImm : IType {
        ShiftRightLogiImm(IType instruction) : IType(instruction) {}
        ShiftRightLogiImm(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // srai rd,rs1,imm
    struct ShiftRightArithImm : IType {
        ShiftRightArithImm(IType instruction) : IType(instruction) {}
        ShiftRightArithImm(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // slti rd,rs1,imm
    struct SetLessThanImm : IType {
        SetLessThanImm(IType instruction) : IType(instruction) {}
        SetLessThanImm(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // sltui rd,rs1,imm
    struct SetLessThanImmUnsigned : IType {
        SetLessThanImmUnsigned(IType instruction) : IType(instruction) {}
        SetLessThanImmUnsigned(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    struct Ecall : IType {
        Ecall(IType instruction) : IType(instruction) {}
        Ecall(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    struct Ebreak : IType {
        Ebreak(IType instruction) : IType(instruction) {}
        Ebreak(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    /// Save instructions
    // sw rs2,offset(rs1)
    struct SaveWord : SType {
        SaveWord(SType instruction) : SType(instruction) {}
        SaveWord(std::bitset<32> instruction) : SType(instruction) {}
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // sh rs2,offset(rs1)
    struct SaveHalfWord : SType {
        SaveHalfWord(SType instruction) : SType(instruction) {}
        SaveHalfWord(std::bitset<32> instruction) : SType(instruction) {}
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // sb rs2,offset(rs1)
    struct SaveByte : SType {
        SaveByte(SType instruction) : SType(instruction) {}
        SaveByte(std::bitset<32> instruction) : SType(instruction) {}
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // Load instructions
    // lw rd,offset(rs1)
    struct LoadWord : IType {
        LoadWord(IType instruction) : IType(instruction) {}
        LoadWord(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // lh rd,offset(rs1)
    struct LoadHalfWord : IType {
        LoadHalfWord(IType instruction) : IType(instruction) {}
        LoadHalfWord(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // lb rd,offset(rs1)
    struct LoadByte : IType {
        LoadByte(IType instruction) : IType(instruction) {}
        LoadByte(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // lhu rd,offset(rs1)
    struct LoadUnsignedHalfWord : IType {
        LoadUnsignedHalfWord(IType instruction) : IType(instruction) {}
        LoadUnsignedHalfWord(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // lbu rd,offset(rs1)
    struct LoadUnsignedByte : IType {
        LoadUnsignedByte(IType instruction) : IType(instruction) {}
        LoadUnsignedByte(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
    };

    // B instructions
    // beq rs1,rs2,offset
    struct BranchEqual : BType {
        BranchEqual(BType instruction) : BType(instruction) {}
        BranchEqual(std::bitset<32> instruction) : BType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // bne rs1,rs2,offset
    struct BranchNotEqual : BType {
        BranchNotEqual(BType instruction) : BType(instruction) {}
        BranchNotEqual(std::bitset<32> instruction) : BType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // blt rs1,rs2,offset
    struct BranchLessThan : BType {
        BranchLessThan(BType instruction) : BType(instruction) {}
        BranchLessThan(std::bitset<32> instruction) : BType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // bltu rs1,rs2,offset
    struct BranchLessThanUnsigned : BType {
        BranchLessThanUnsigned(BType instruction) : BType(instruction) {}
        BranchLessThanUnsigned(std::bitset<32> instruction) : BType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // bge rs1,rs2,offset
    struct BranchGreaterThanEqual : BType {
        BranchGreaterThanEqual(BType instruction) : BType(instruction) {}
        BranchGreaterThanEqual(std::bitset<32> instruction) : BType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // bgeu rs1,rs2,offset
    struct BranchGreaterThanEqualUnsigned : BType {
        BranchGreaterThanEqualUnsigned(BType instruction) : BType(instruction) {}
        BranchGreaterThanEqualUnsigned(std::bitset<32> instruction) : BType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // U instructions
    // lui rd,imm
    struct LoadUpperImmediate : UType {
        LoadUpperImmediate(UType instruction) : UType(instruction) {}
        LoadUpperImmediate(std::bitset<32> instruction) : UType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // auipc rd,imm
    struct AddUpperImmedateToPC : UType {
        AddUpperImmedateToPC(UType instruction) : UType(instruction) {}
        AddUpperImmedateToPC(std::bitset<32> instruction) : UType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };

    // jal rd,offset
    struct JumpAndLink : JType {
        JumpAndLink(JType instruction) : JType(instruction) {}
        JumpAndLink(std::bitset<32> instruction) : JType(instruction) {}
        using JType::execute;
        using JType::writeBack;
    };

    // jalr rd,offset
    struct JumpAndLinkReg : IType {
        JumpAndLinkReg(IType instruction) : IType(instruction) {}
        JumpAndLinkReg(std::bitset<32> instruction) : IType(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
        void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
    };

    // fence
    // this is currently a no op as our simulator is single hart
    struct Fence : Instruction {
        Fence(std::bitset<32> instruction) : Instruction(instruction) {}
        void execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) override;
    };
}

#endif // RISCIINSTRUCTIONS_H