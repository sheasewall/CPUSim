#ifndef RISCIINSTRUCTIONS_H
#define RISCIINSTRUCTIONS_H

#include "alu.h"
#include "constants.h"
#include "exceptions.h"
#include "immgenunit.h"
#include "instructionfile.h"
#include "maskingunit.hpp"
#include "memoryfile.h"
#include "registerfile.h"
#include <bitset>
#include <memory>

namespace RISC {
class Instruction {

public:
  virtual ~Instruction() {}
  virtual void fetch(std::bitset<32> instruction,
                     std::shared_ptr<MaskingUnit> p_mu) = 0;
  virtual void decode(std::shared_ptr<RegisterFile> p_reg_file,
                      std::shared_ptr<ImmGenUnit> p_igu) = 0;
  virtual void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) = 0;
  virtual void accessMemory(std::shared_ptr<MemoryFile> p_data_file) = 0;
  virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) = 0;
};

/*
=========================
    RType Instructions
=========================
*/

class RType : public Instruction {
public:
  std::bitset<5> rd;
  std::bitset<5> rs1;
  std::bitset<5> rs2;

  std::bitset<32> rs1_val;
  std::bitset<32> rs2_val;
  std::bitset<32> result;

  virtual void fetch(std::bitset<32> instruction,
                     std::shared_ptr<MaskingUnit> p_mu) override;
  virtual void decode(std::shared_ptr<RegisterFile> p_reg_file,
                      std::shared_ptr<ImmGenUnit> p_igu) override;
  virtual void execute(std::shared_ptr<ALU> p_alu,
                       std::bitset<32> &pc) override;
  virtual void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
  virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
};

// add rd,rs1,rs2
class Add : public RType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// sub rd,rs1,rs2
class Sub : public RType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// xor rd,rs1,rs2
class Xor : public RType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// or rd,rs1,rs2
class Or : public RType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// and rd,rs1,rs2
class And : public RType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// sll rd,rs1,rs2
class ShiftLeftLogi : public RType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// srl rd,rs1,rs2
class ShiftRightLogi : public RType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// sra rd,rs1,rs2
class ShiftRightArith : public RType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// slt rd,rs1,rs2
class SetLessThan : public RType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// sltu rd,rs1,rs2
class SetLessThanUnsigned : public RType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

/*
=========================
    IType Instructions
=========================
*/

class IType : public Instruction {
public:
  std::bitset<5> rd;
  std::bitset<5> rs1;
  std::bitset<12> imm;

  std::bitset<32> rs1_val;
  std::bitset<32> imm_val;
  std::bitset<32> result;

  virtual void fetch(std::bitset<32> instruction,
                     std::shared_ptr<MaskingUnit> p_mu) override;
  virtual void decode(std::shared_ptr<RegisterFile> p_reg_file,
                      std::shared_ptr<ImmGenUnit> p_igu) override;
  virtual void execute(std::shared_ptr<ALU> p_alu,
                       std::bitset<32> &pc) override;
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
  virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
};

// addi rd,rs1,imm
class AddImm : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// xori rd,rs1,imm
class XorImm : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// ori rd,rs1,imm
class OrImm : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// andi rd,rs1,imm
class AndImm : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// slli rd,rs1,imm
class ShiftLeftLogiImm : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// srli rd,rs1,imm
class ShiftRightLogiImm : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// srai rd,rs1,imm
class ShiftRightArithImm : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// slti rd,rs1,imm
class SetLessThanImm : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// sltui rd,rs1,imm
class SetLessThanImmUnsigned : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// lw rd,offset(rs1)
class LoadWord : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
};

// lh rd,offset(rs1)
class LoadHalfWord : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
};

// lb rd,offset(rs1)
class LoadByte : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
};

// lhu rd,offset(rs1)
class LoadUnsignedHalfWord : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
};

// lbu rd,offset(rs1)
class LoadUnsignedByte : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
};

// jalr rd,offset
class JumpAndLinkReg : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
  void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
};

// ecall
class Ecall : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// ebreak
class Ebreak : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// fence
// this is currently a no op as our simulator is single hart
class Fence : public IType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

/*
=========================
    SType Instructions
=========================
*/

class SType : public Instruction {
public:
  std::bitset<5> rs1;
  std::bitset<5> rs2;
  std::bitset<12> imm;

  std::bitset<32> rs1_val;
  std::bitset<32> rs2_val;
  std::bitset<32> imm_val;

  std::bitset<32> result;

  virtual void fetch(std::bitset<32> instruction,
                     std::shared_ptr<MaskingUnit> p_mu) override;
  virtual void decode(std::shared_ptr<RegisterFile> p_reg_file,
                      std::shared_ptr<ImmGenUnit> p_igu) override;
  virtual void execute(std::shared_ptr<ALU> p_alu,
                       std::bitset<32> &pc) override;
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
  void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override {}
};

// sw rs2,offset(rs1)
class SaveWord : public SType {
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
};

// sh rs2,offset(rs1)
class SaveHalfWord : public SType {
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
};

// sb rs2,offset(rs1)
class SaveByte : public SType {
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override;
};

/*
=========================
    BType Instructions
=========================
*/

class BType : public Instruction {
public:
  std::bitset<5> rs1;
  std::bitset<5> rs2;
  std::bitset<12> imm;

  std::bitset<32> rs1_val;
  std::bitset<32> rs2_val;
  std::bitset<32> imm_val;

  std::bitset<32> result;

  virtual void fetch(std::bitset<32> instruction,
                     std::shared_ptr<MaskingUnit> p_mu) override;
  virtual void decode(std::shared_ptr<RegisterFile> p_reg_file,
                      std::shared_ptr<ImmGenUnit> p_igu) override;
  virtual void execute(std::shared_ptr<ALU> p_alu,
                       std::bitset<32> &pc) override;
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
  virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override {}
};

// beq rs1,rs2,offset
class BranchEqual : public BType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// bne rs1,rs2,offset
class BranchNotEqual : public BType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// blt rs1,rs2,offset
class BranchLessThan : public BType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// bltu rs1,rs2,offset
class BranchLessThanUnsigned : public BType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// bge rs1,rs2,offset
class BranchGreaterThanEqual : public BType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// bgeu rs1,rs2,offset
class BranchGreaterThanEqualUnsigned : public BType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

/*
=========================
    UType Instructions
=========================
*/

class UType : public Instruction {
public:
  std::bitset<5> rd;
  std::bitset<20> imm_long;

  std::bitset<32> imm_val;

  std::bitset<32> result;

  virtual void fetch(std::bitset<32> instruction,
                     std::shared_ptr<MaskingUnit> p_mu) override;
  virtual void decode(std::shared_ptr<RegisterFile> p_reg_file,
                      std::shared_ptr<ImmGenUnit> p_igu) override;
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override {}
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
  virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
};

// lui rd,imm
class LoadUpperImmediate : public UType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

// auipc rd,imm
class AddUpperImmedateToPC : public UType {
  void execute(std::shared_ptr<ALU> p_alu, std::bitset<32> &pc) override;
};

/*
=========================
    JType Instructions
=========================
*/

class JType : public Instruction {
public:
  std::bitset<5> rd;
  std::bitset<20> imm_long;

  std::bitset<32> imm_val;

  std::bitset<32> result;

  virtual void fetch(std::bitset<32> instruction,
                     std::shared_ptr<MaskingUnit> p_mu) override;
  virtual void decode(std::shared_ptr<RegisterFile> p_reg_file,
                      std::shared_ptr<ImmGenUnit> p_igu) override;
  virtual void execute(std::shared_ptr<ALU> p_alu,
                       std::bitset<32> &pc) override;
  void accessMemory(std::shared_ptr<MemoryFile> p_data_file) override {}
  virtual void writeBack(std::shared_ptr<RegisterFile> p_reg_file) override;
};

// jal rd,offset
class JumpAndLink : public JType {};

} // namespace RISC

#endif // RISCIINSTRUCTIONS_H