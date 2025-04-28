#include "riscinstructions.h"

namespace RISC {
/*
=========================
    RType Instructions
=========================
*/

void RType::fetch(std::bitset<32> instruction,
                  std::shared_ptr<MaskingUnit> p_mu) {
  rs2 = p_mu->maskBits<5, 32>(instruction, 20, 5);
  rs1 = p_mu->maskBits<5, 32>(instruction, 15, 5);
  rd = p_mu->maskBits<5, 32>(instruction, 7, 5);
}

void RType::decode(std::shared_ptr<RegisterFile> p_reg_file,
                   std::shared_ptr<ImmGenUnit> p_igu) {
  std::pair<std::bitset<32>, std::bitset<32>> registers =
      p_reg_file->read(rs1, rs2);
  rs1_val = registers.first;
  rs2_val = registers.second;
}

void RType::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  pc = p_alu->add(pc, std::bitset<32>(4));
}

void RType::writeBack(std::shared_ptr<RegisterFile> p_reg_file) {
  p_reg_file->write(rd, result);
}

void Add::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->add(rs1_val, rs2_val);
  RType::execute(p_alu, pc);
}

void Sub::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  std::bitset<32> negated = p_alu->negate(rs2_val);
  result = p_alu->add(rs1_val, negated);
  RType::execute(p_alu, pc);
}

void Xor::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->bitwiseXor(rs1_val, rs2_val);
  RType::execute(p_alu, pc);
}

void Or::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->bitwiseOr(rs1_val, rs2_val);
  RType::execute(p_alu, pc);
}

void And::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->bitwiseAnd(rs1_val, rs2_val);
  RType::execute(p_alu, pc);
}

void ShiftLeftLogi::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  // Select only the lower 5 bits as per RISC-V specs.
  rs2_val = p_alu->hardwareRightShift(
      p_alu->hardwareLeftShift(rs2_val, std::bitset<32>(27)),
      std::bitset<32>(27));
  result = p_alu->hardwareLeftShift(rs1_val, rs2_val);
  RType::execute(p_alu, pc);
}

void ShiftRightLogi::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  // Select only the lower 5 bits as per RISC-V specs.
  rs2_val = p_alu->hardwareRightShift(
      p_alu->hardwareLeftShift(rs2_val, std::bitset<32>(27)),
      std::bitset<32>(27));
  result = p_alu->hardwareRightShift(rs1_val, rs2_val);
  RType::execute(p_alu, pc);
}

void ShiftRightArith::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  // Select only the lower 5 bits as per RISC-V specs.
  rs2_val = p_alu->hardwareRightShift(
      p_alu->hardwareLeftShift(rs2_val, std::bitset<32>(27)),
      std::bitset<32>(27));
  result = p_alu->arithmeticRightShift(rs1_val, rs2_val);
  RType::execute(p_alu, pc);
}

void SetLessThan::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->lessThanSigned(rs1_val, rs2_val);
  RType::execute(p_alu, pc);
}

void SetLessThanUnsigned::execute(std::shared_ptr<ALU> p_alu,
                                  std::bitset<32> &pc) {
  result = p_alu->lessThanUnsigned(rs1_val, rs2_val);
  RType::execute(p_alu, pc);
}

/*
=========================
    IType Instructions
=========================
*/

void IType::fetch(std::bitset<32> instruction,
                  std::shared_ptr<MaskingUnit> p_mu) {
  rs1 = p_mu->maskBits<5, 32>(instruction, 15, 5);
  rd = p_mu->maskBits<5, 32>(instruction, 7, 5);
  imm = p_mu->maskBits<12, 32>(instruction, 20, 12);
}

void IType::decode(std::shared_ptr<RegisterFile> p_reg_file,
                   std::shared_ptr<ImmGenUnit> p_igu) {
  std::pair<std::bitset<32>, std::bitset<32>> registers =
      p_reg_file->read(rs1, 0);
  rs1_val = registers.first;
  imm_val = p_igu->signExtend(imm);
}

void IType::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  pc = p_alu->add(pc, std::bitset<32>(4));
}

void IType::writeBack(std::shared_ptr<RegisterFile> p_reg_file) {
  p_reg_file->write(rd, result);
}

void AddImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->add(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}

void XorImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->bitwiseXor(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}

void OrImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->bitwiseOr(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}

void AndImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->bitwiseAnd(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}

void ShiftLeftLogiImm::execute(std::shared_ptr<ALU> p_alu,
                               std::bitset<32> &pc) {
  // Select only the lower 5 bits as per RISC-V specs.
  // Not redundant, as upper 7 bits are used as a funct7
  imm_val = p_alu->hardwareRightShift(
      p_alu->hardwareLeftShift(imm_val, std::bitset<32>(27)),
      std::bitset<32>(27));
  result = p_alu->hardwareLeftShift(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}
void ShiftRightLogiImm::execute(std::shared_ptr<ALU> p_alu,
                                std::bitset<32> &pc) {
  // Select only the lower 5 bits as per RISC-V specs.
  // Not redundant, as upper 7 bits are used as a funct7
  imm_val = p_alu->hardwareRightShift(
      p_alu->hardwareLeftShift(imm_val, std::bitset<32>(27)),
      std::bitset<32>(27));
  result = p_alu->hardwareRightShift(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}
void ShiftRightArithImm::execute(std::shared_ptr<ALU> p_alu,
                                 std::bitset<32> &pc) {
  // Select only the lower 5 bits as per RISC-V specs.
  // Not redundant, as upper 7 bits are used as a funct7
  imm_val = p_alu->hardwareRightShift(
      p_alu->hardwareLeftShift(imm_val, std::bitset<32>(27)),
      std::bitset<32>(27));
  result = p_alu->arithmeticRightShift(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}
void SetLessThanImm::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  // bool to bitset<32> implicit conversion
  result = p_alu->lessThanSigned(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}
void SetLessThanImmUnsigned::execute(std::shared_ptr<ALU> p_alu,
                                     std::bitset<32> &pc) {
  // bool to bitset<32> implicit conversion
  result = p_alu->lessThanUnsigned(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}

void LoadWord::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->add(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}

void LoadWord::accessMemory(std::shared_ptr<MemoryFile> p_data_file) {
  result = p_data_file->readBytes(result, 4);
}

void LoadHalfWord::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->add(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}

void LoadHalfWord::accessMemory(std::shared_ptr<MemoryFile> p_data_file) {
  result = p_data_file->readBytes(result, 2, true);
}

void LoadByte::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->add(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}

void LoadByte::accessMemory(std::shared_ptr<MemoryFile> p_data_file) {
  result = p_data_file->readBytes(result, 1, true);
}

void LoadUnsignedHalfWord::execute(std::shared_ptr<ALU> p_alu,
                                   std::bitset<32> &pc) {
  result = p_alu->add(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}

void LoadUnsignedHalfWord::accessMemory(
    std::shared_ptr<MemoryFile> p_data_file) {
  result = p_data_file->readBytes(result, 2);
}

void LoadUnsignedByte::execute(std::shared_ptr<ALU> p_alu,
                               std::bitset<32> &pc) {
  result = p_alu->add(rs1_val, imm_val);
  IType::execute(p_alu, pc);
}

void LoadUnsignedByte::accessMemory(std::shared_ptr<MemoryFile> p_data_file) {
  result = p_data_file->readBytes(result, 1);
}

void JumpAndLinkReg::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->add(pc, std::bitset<32>(4));
  std::bitset<32> offset = p_alu->add(rs1_val, imm_val);
  pc = offset;
}

void JumpAndLinkReg::writeBack(std::shared_ptr<RegisterFile> p_reg_file) {
  p_reg_file->write(rd, result);
}

/*
=========================
    SType Instructions
=========================
*/

void SType::fetch(std::bitset<32> instruction,
                  std::shared_ptr<MaskingUnit> p_mu) {
  rs2 = p_mu->maskBits<5, 32>(instruction, 20, 5);
  rs1 = p_mu->maskBits<5, 32>(instruction, 15, 5);

  std::bitset<7> imm1 = p_mu->maskBits<7, 32>(instruction, 25, 7);
  std::bitset<5> imm0 = p_mu->maskBits<5, 32>(instruction, 7, 5);
  imm = p_mu->concatBits<5, 7>(imm0, imm1);
}

void SType::decode(std::shared_ptr<RegisterFile> p_reg_file,
                   std::shared_ptr<ImmGenUnit> p_igu) {
  std::pair<std::bitset<32>, std::bitset<32>> registers =
      p_reg_file->read(rs1, rs2);
  rs1_val = registers.first;
  rs2_val = registers.second;
  imm_val = p_igu->signExtend(imm);
}

void SType::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->add(rs1_val, imm_val);
  pc = p_alu->add(pc, std::bitset<32>(4));
}

void SaveWord::accessMemory(std::shared_ptr<MemoryFile> p_data_file) {
  p_data_file->writeBytes(result, rs2_val, 4);
}

void SaveHalfWord::accessMemory(std::shared_ptr<MemoryFile> p_data_file) {
  p_data_file->writeBytes(result, rs2_val, 2);
}

void SaveByte::accessMemory(std::shared_ptr<MemoryFile> p_data_file) {
  p_data_file->writeBytes(result, rs2_val, 1);
}

/*
=========================
    BType Instructions
=========================
*/

void BType::fetch(std::bitset<32> instruction,
                  std::shared_ptr<MaskingUnit> p_mu) {
  rs2 = p_mu->maskBits<5, 32>(instruction, 20, 5);
  rs1 = p_mu->maskBits<5, 32>(instruction, 15, 5);

  std::bitset<1> imm3 = p_mu->maskBits<1, 32>(instruction, 31, 1);
  std::bitset<6> imm1 = p_mu->maskBits<6, 32>(instruction, 25, 6);
  std::bitset<4> imm0 = p_mu->maskBits<4, 32>(instruction, 8, 4);
  std::bitset<1> imm2 = p_mu->maskBits<1, 32>(instruction, 7, 1);
  std::bitset<10> low_bits = p_mu->concatBits<4, 6>(imm0, imm1);
  std::bitset<2> high_bits = p_mu->concatBits<1, 1>(imm2, imm3);
  imm = p_mu->concatBits<10, 2>(low_bits, high_bits);
}

void BType::decode(std::shared_ptr<RegisterFile> p_reg_file,
                   std::shared_ptr<ImmGenUnit> p_igu) {
  std::pair<std::bitset<32>, std::bitset<32>> registers =
      p_reg_file->read(rs1, rs2);
  rs1_val = registers.first;
  rs2_val = registers.second;
  imm_val = p_igu->signExtend(imm);
}

void BType::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  imm_val = p_alu->hardwareLeftShift(imm_val, std::bitset<32>(1));
}

void BranchEqual::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  BType::execute(p_alu, pc);
  if (p_alu->hardwareIsEqual(rs1_val, rs2_val)) {
    pc = p_alu->add(pc, imm_val);
  } else {
    pc = p_alu->add(pc, std::bitset<32>(4));
  }
}

void BranchNotEqual::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  BType::execute(p_alu, pc);
  if (!p_alu->hardwareIsEqual(rs1_val, rs2_val)) {
    pc = p_alu->add(pc, imm_val);
  } else {
    pc = p_alu->add(pc, std::bitset<32>(4));
  }
}

void BranchLessThan::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  BType::execute(p_alu, pc);
  if (p_alu->lessThanSigned(rs1_val, rs2_val)) {
    pc = p_alu->add(pc, imm_val);
  } else {
    pc = p_alu->add(pc, std::bitset<32>(4));
  }
}

void BranchLessThanUnsigned::execute(std::shared_ptr<ALU> p_alu,
                                     std::bitset<32> &pc) {
  BType::execute(p_alu, pc);
  if (p_alu->lessThanUnsigned(rs1_val, rs2_val)) {
    pc = p_alu->add(pc, imm_val);
  } else {
    pc = p_alu->add(pc, std::bitset<32>(4));
  }
}

void BranchGreaterThanEqual::execute(std::shared_ptr<ALU> p_alu,
                                     std::bitset<32> &pc) {
  BType::execute(p_alu, pc);
  if (p_alu->greaterThanEqualSigned(rs2_val, rs1_val)) {
    pc = p_alu->add(pc, imm_val);
  } else {
    pc = p_alu->add(pc, std::bitset<32>(4));
  }
}

void BranchGreaterThanEqualUnsigned::execute(std::shared_ptr<ALU> p_alu,
                                             std::bitset<32> &pc) {
  BType::execute(p_alu, pc);
  if (p_alu->greaterThanEqualUnsigned(rs2_val, rs1_val)) {
    pc = p_alu->add(pc, imm_val);
  } else {
    pc = p_alu->add(pc, std::bitset<32>(4));
  }
}

/*
=========================
    UType Instructions
=========================
*/

void UType::fetch(std::bitset<32> instruction,
                  std::shared_ptr<MaskingUnit> p_mu) {
  rd = p_mu->maskBits<5, 32>(instruction, 7, 5);
  imm_long = p_mu->maskBits<20, 32>(instruction, 12, 20);
}

void UType::decode(std::shared_ptr<RegisterFile> p_reg_file,
                   std::shared_ptr<ImmGenUnit> p_igu) {
  imm_val = p_igu->generateLong(imm_long);
}

void UType::writeBack(std::shared_ptr<RegisterFile> p_reg_file) {
  p_reg_file->write(rd, result);
}

void LoadUpperImmediate::execute(std::shared_ptr<ALU> p_alu,
                                 std::bitset<32> &pc) {
  result = imm_val;
  pc = p_alu->add(pc, std::bitset<32>(4));
}

void AddUpperImmedateToPC::execute(std::shared_ptr<ALU> p_alu,
                                   std::bitset<32> &pc) {
  result = p_alu->add(pc, imm_val);
  pc = p_alu->add(pc, std::bitset<32>(4));
}

/*
=========================
    JType Instructions
=========================
*/

void JType::fetch(std::bitset<32> instruction,
                  std::shared_ptr<MaskingUnit> p_mu) {
  rd = p_mu->maskBits<5, 32>(instruction, 7, 5);

  std::bitset<1> imm3 = p_mu->maskBits<1, 32>(instruction, 31, 1);
  std::bitset<10> imm0 = p_mu->maskBits<10, 32>(instruction, 21, 10);
  std::bitset<1> imm1 = p_mu->maskBits<1, 32>(instruction, 20, 1);
  std::bitset<8> imm2 = p_mu->maskBits<8, 32>(instruction, 12, 8);
  std::bitset<11> low_bits = p_mu->concatBits<10, 1>(imm0, imm1);
  std::bitset<9> high_bits = p_mu->concatBits<8, 1>(imm2, imm3);
  imm_long = p_mu->concatBits<11, 9>(low_bits, high_bits);
}

void JType::decode(std::shared_ptr<RegisterFile> p_reg_file,
                   std::shared_ptr<ImmGenUnit> p_igu) {
  imm_val = p_igu->signExtend(imm_long);
}

void JType::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  result = p_alu->add(pc, std::bitset<32>(4));
  std::bitset<32> imm_val_shifted = p_alu->add(imm_val, imm_val);
  pc = p_alu->add(pc, imm_val_shifted);
}

void JType::writeBack(std::shared_ptr<RegisterFile> p_reg_file) {
  p_reg_file->write(rd, result);
}

/*
=========================
   Misc. Instructions
=========================
*/

void Ecall::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  throw EcallTrap();
}

void Ebreak::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  IType::execute(p_alu, pc);
  throw EbreakTrap();
}

void Fence::execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) {
  // No operation
  pc = p_alu->add(pc, std::bitset<32>(4));
}
} // namespace RISC