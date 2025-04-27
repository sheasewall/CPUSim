#include "riscinstructions.h"

namespace RISC
{
    Instruction::Instruction(std::bitset<32> instruction)
    {
        std::string instr = instruction.to_string();

        funct7 = std::bitset<7>(instr.substr(0, 7));
        rs2 = std::bitset<5>(instr.substr(7, 5));
        rs1 = std::bitset<5>(instr.substr(12, 5));
        funct3 = std::bitset<3>(instr.substr(17, 3));
        rd = std::bitset<5>(instr.substr(20, 5));

        opcode = std::bitset<7>(instr.substr(25, 7));
    }


    // RType
    RType::RType(std::bitset<32> instruction) : Instruction(instruction)
    {
        
    }

    void RType::decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen)
    {
        std::pair<std::bitset<32>, std::bitset<32>> registers = p_reg_file->read(rs1, rs2);
        rs1_val = registers.first;
        rs2_val = registers.second;
    }

    void RType::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        pc = p_alu->add(pc, std::bitset<32>(4));
    }

    void RType::writeBack(std::shared_ptr<RegisterFile> p_reg_file)
    {
        p_reg_file->write(rd, result);
    }

    // IType
    IType::IType(std::bitset<32> instruction) : Instruction(instruction)
    {
        std::string instr = instruction.to_string();
        imm = std::bitset<12>(instr.substr(0, 12));
    }

    void IType::decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen)
    {
        std::pair<std::bitset<32>, std::bitset<32>> registers = p_reg_file->read(rs1, 0);
        rs1_val = registers.first;
        imm_val = p_imm_gen->signExtend(imm);
    }

    void IType::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        pc = p_alu->add(pc, std::bitset<32>(4));
    }

    void IType::writeBack(std::shared_ptr<RegisterFile> p_reg_file)
    {
        p_reg_file->write(rd, result);
    }

    // SType
    SType::SType(std::bitset<32> instruction) : Instruction(instruction)
    {
        std::string instr = instruction.to_string();
        std::bitset<7> imm1 = std::bitset<7>(instr.substr(0, 7));
        std::bitset<5> imm0 = std::bitset<5>(instr.substr(20, 5));
        imm = std::bitset<12>(imm1.to_string() + imm0.to_string());
    }

    void SType::decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen)
    {
        std::pair<std::bitset<32>, std::bitset<32>> registers = p_reg_file->read(rs1, rs2);
        rs1_val = registers.first;
        rs2_val = registers.second;
        imm_val = p_imm_gen->signExtend(imm);
    }

    void SType::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->add(rs1_val, imm_val);
        pc = p_alu->add(pc, std::bitset<32>(4));
    }

    // BType
    BType::BType(std::bitset<32> instruction) : Instruction(instruction)
    {
        std::string instr = instruction.to_string();
        std::bitset<1> imm3 = std::bitset<1>(instr.substr(0, 1));
        std::bitset<6> imm1 = std::bitset<6>(instr.substr(1, 6));
        std::bitset<4> imm0 = std::bitset<4>(instr.substr(20, 4));
        std::bitset<1> imm2 = std::bitset<1>(instr.substr(24, 1));
        imm = std::bitset<12>(imm3.to_string() + imm2.to_string() + imm1.to_string() + imm0.to_string());
    }

    void BType::decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen)
    {
        std::pair<std::bitset<32>, std::bitset<32>> registers = p_reg_file->read(rs1, rs2);
        rs1_val = registers.first;
        rs2_val = registers.second;
        imm_val = p_imm_gen->signExtend(imm);
    }

    void BType::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        imm_val = p_alu->hardwareLeftShift(imm_val, std::bitset<32>(1));
    }

    // UType
    UType::UType(std::bitset<32> instruction) : Instruction(instruction)
    {
        std::string instr = instruction.to_string();
        imm_long = std::bitset<20>(instr.substr(0, 20));
    }

    void UType::decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen)
    {
        imm_val = p_imm_gen->generateLong(imm_long);
    }

    void UType::writeBack(std::shared_ptr<RegisterFile> p_reg_file)
    {
        p_reg_file->write(rd, result);
    }

    // JType
    JType::JType(std::bitset<32> instruction) : Instruction(instruction)
    {
        std::string instr = instruction.to_string();
        std::bitset<1> imm3 = std::bitset<1>(instr.substr(0, 1));
        std::bitset<10> imm0 = std::bitset<10>(instr.substr(1, 10));
        std::bitset<1> imm1 = std::bitset<1>(instr.substr(11, 1));
        std::bitset<8> imm2 = std::bitset<8>(instr.substr(12, 8));
        imm_long = std::bitset<20>(imm3.to_string() + imm2.to_string() + imm1.to_string() + imm0.to_string());
    }

    void JType::decode(std::shared_ptr<RegisterFile> p_reg_file, std::shared_ptr<ImmGen> p_imm_gen)
    {
        imm_val = p_imm_gen->signExtend(imm_long);
    }

    void JType::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->add(pc, std::bitset<32>(4));
        std::bitset<32> imm_val_shifted = p_alu->add(imm_val, imm_val);
        pc = p_alu->add(pc, imm_val_shifted);
    }

    void JType::writeBack(std::shared_ptr<RegisterFile> p_reg_file)
    {
        p_reg_file->write(rd, result);
    }

    // R instructions
    void Add::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) {
        result = p_alu->add(rs1_val, rs2_val);
        RType::execute(p_alu, pc);
    }

    void Sub::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        std::bitset<32> negated = p_alu->negate(rs2_val);
        result = p_alu->add(rs1_val, negated);
        RType::execute(p_alu, pc);
    }

    void Xor::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->bitwiseXor(rs1_val, rs2_val);
        RType::execute(p_alu, pc);
    }

    void Or::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->bitwiseOr(rs1_val, rs2_val);
        RType::execute(p_alu, pc);
    }

    void And::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->bitwiseAnd(rs1_val, rs2_val);
        RType::execute(p_alu, pc);
    }

    void ShiftLeftLogi::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        // Select only the lower 5 bits as per RISC-V specs.
        rs2_val = p_alu->hardwareRightShift(p_alu->hardwareLeftShift(rs2_val, std::bitset<32>(27)), std::bitset<32>(27));
        result = p_alu->hardwareLeftShift(rs1_val, rs2_val);
        RType::execute(p_alu, pc);
    }

    void ShiftRightLogi::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        // Select only the lower 5 bits as per RISC-V specs.
        rs2_val = p_alu->hardwareRightShift(p_alu->hardwareLeftShift(rs2_val, std::bitset<32>(27)), std::bitset<32>(27));
        result = p_alu->hardwareRightShift(rs1_val, rs2_val);
        RType::execute(p_alu, pc);
    }

    void ShiftRightArith::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        // Select only the lower 5 bits as per RISC-V specs.
        rs2_val = p_alu->hardwareRightShift(p_alu->hardwareLeftShift(rs2_val, std::bitset<32>(27)), std::bitset<32>(27));
        result = p_alu->arithmeticRightShift(rs1_val, rs2_val);
        RType::execute(p_alu, pc);
    }

    void SetLessThan::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->lessThanSigned(rs1_val, rs2_val);
        RType::execute(p_alu, pc);
    }

    void SetLessThanUnsigned::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->lessThanUnsigned(rs1_val, rs2_val);
        RType::execute(p_alu, pc);
    }

    // I instructions
    void AddImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc) {
        result = p_alu->add(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }

    void XorImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->bitwiseXor(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }

    void OrImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->bitwiseOr(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }

    void AndImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->bitwiseAnd(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }

    void ShiftLeftLogiImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        // Select only the lower 5 bits as per RISC-V specs.
        // Not redundant, as upper 7 bits are used as a funct7
        imm_val = p_alu->hardwareRightShift(p_alu->hardwareLeftShift(imm_val, std::bitset<32>(27)), std::bitset<32>(27));
        result = p_alu->hardwareLeftShift(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }
    void ShiftRightLogiImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        // Select only the lower 5 bits as per RISC-V specs.
        // Not redundant, as upper 7 bits are used as a funct7
        imm_val = p_alu->hardwareRightShift(p_alu->hardwareLeftShift(imm_val, std::bitset<32>(27)), std::bitset<32>(27));
        result = p_alu->hardwareRightShift(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }
    void ShiftRightArithImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        // Select only the lower 5 bits as per RISC-V specs.
        // Not redundant, as upper 7 bits are used as a funct7
        imm_val = p_alu->hardwareRightShift(p_alu->hardwareLeftShift(imm_val, std::bitset<32>(27)), std::bitset<32>(27));
        result = p_alu->arithmeticRightShift(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }
    void SetLessThanImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        // bool to bitset<32> implicit conversion
        result = p_alu->lessThanSigned(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }
    void SetLessThanImmUnsigned::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        // bool to bitset<32> implicit conversion
        result = p_alu->lessThanUnsigned(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }

    void Ecall::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        throw EcallTrap();
    }

    void Ebreak::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        IType::execute(p_alu, pc);
        throw EbreakTrap();
    }

    // S instructions
    void SaveWord::accessMemory(std::shared_ptr<MemoryFile> p_data_file)
    {
        p_data_file->writeBytes(result, rs2_val, 4);
    }

    void SaveHalfWord::accessMemory(std::shared_ptr<MemoryFile> p_data_file)
    {
        p_data_file->writeBytes(result, rs2_val, 2);
    }

    void SaveByte::accessMemory(std::shared_ptr<MemoryFile> p_data_file)
    {
        p_data_file->writeBytes(result, rs2_val, 1);
    }

    // B instructions 
    void BranchEqual::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        BType::execute(p_alu, pc);
        if (p_alu->hardwareIsEqual(rs1_val, rs2_val)) {
            pc = p_alu->add(pc, imm_val);
        }
        else {
            pc = p_alu->add(pc, std::bitset<32>(4));
        }
    }

    void BranchNotEqual::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        BType::execute(p_alu, pc);
        if (!p_alu->hardwareIsEqual(rs1_val, rs2_val)) {
            pc = p_alu->add(pc, imm_val);
        }
        else {
            pc = p_alu->add(pc, std::bitset<32>(4));
        }
    }

    void BranchLessThan::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        BType::execute(p_alu, pc);
        if (p_alu->lessThanSigned(rs1_val, rs2_val)) {
            pc = p_alu->add(pc, imm_val);
        }
        else {
            pc = p_alu->add(pc, std::bitset<32>(4));
        }
    }

    void BranchLessThanUnsigned::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        BType::execute(p_alu, pc);
        if (p_alu->lessThanUnsigned(rs1_val, rs2_val)) {
            pc = p_alu->add(pc, imm_val);
        }
        else {
            pc = p_alu->add(pc, std::bitset<32>(4));
        }
    }

    void BranchGreaterThanEqual::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        BType::execute(p_alu, pc);
        if (p_alu->greaterThanEqualSigned(rs2_val, rs1_val)) {
            pc = p_alu->add(pc, imm_val);
        }
        else {
            pc = p_alu->add(pc, std::bitset<32>(4));
        }
    }

    void BranchGreaterThanEqualUnsigned::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        BType::execute(p_alu, pc);
        if (p_alu->greaterThanEqualUnsigned(rs2_val, rs1_val)) {
            pc = p_alu->add(pc, imm_val);
        }
        else {
            pc = p_alu->add(pc, std::bitset<32>(4));
        }
    }

    // U instructions
    void LoadUpperImmediate::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = imm_val;
        pc = p_alu->add(pc, std::bitset<32>(4));
    }

    void AddUpperImmedateToPC::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->add(pc, imm_val);
        pc = p_alu->add(pc, std::bitset<32>(4));
    }

    // Load instructions
    void LoadWord::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->add(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }

    void LoadWord::accessMemory(std::shared_ptr<MemoryFile> p_data_file)
    {
        result = p_data_file->readBytes(result, 4);
    }

    void LoadHalfWord::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->add(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }

    void LoadHalfWord::accessMemory(std::shared_ptr<MemoryFile> p_data_file)
    {
        result = p_data_file->readBytes(result, 2, true);
    }

    void LoadByte::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->add(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }

    void LoadByte::accessMemory(std::shared_ptr<MemoryFile> p_data_file)
    {
        result = p_data_file->readBytes(result, 1, true);
    }

    void LoadUnsignedHalfWord::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->add(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }

    void LoadUnsignedHalfWord::accessMemory(std::shared_ptr<MemoryFile> p_data_file)
    {
        result = p_data_file->readBytes(result, 2);
    }

    void LoadUnsignedByte::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->add(rs1_val, imm_val);
        IType::execute(p_alu, pc);
    }

    void LoadUnsignedByte::accessMemory(std::shared_ptr<MemoryFile> p_data_file)
    {
        result = p_data_file->readBytes(result, 1);
    }

    void JumpAndLinkReg::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        result = p_alu->add(pc, std::bitset<32>(4));
        std::bitset<32> offset = p_alu->add(rs1_val, imm_val);
        pc = offset;
    }

    void JumpAndLinkReg::writeBack(std::shared_ptr<RegisterFile> p_reg_file)
    {
        p_reg_file->write(rd, result);
    }

    void Fence::execute(std::shared_ptr<ALU> p_alu, std::bitset<32>& pc)
    {
        // No operation
        pc = p_alu->add(pc, std::bitset<32>(4));
    }


}