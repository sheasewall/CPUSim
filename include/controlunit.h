#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include "alu.h"
#include "constants.h"
#include "exceptions.h"
#include "immgenunit.h"
#include "instructionfile.h"
#include "maskingunit.hpp"
#include "memoryfile.h"
#include "registerfile.h"
#include "riscinstructions.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

class ControlUnit {
protected:
  unsigned long cycles;

  std::bitset<32> pc;
  std::shared_ptr<RISC::Instruction> p_current_instruction;

  std::shared_ptr<MaskingUnit> p_mu;
  std::shared_ptr<InstructionFile> p_instruction_file;
  std::shared_ptr<ImmGenUnit> p_igu;
  std::shared_ptr<RegisterFile> p_reg_file;
  std::shared_ptr<ALU> p_alu;
  std::shared_ptr<MemoryFile> p_data_file;

public:
  ControlUnit(std::string bin_file);
  ~ControlUnit() {}

  void step();

  // For verification only
  void signature();

private:
  void fetch();
  void decode();
  void execute();
  void memoryAccess();
  void writeBack();

  std::shared_ptr<RISC::Instruction>
  createInstruction(std::bitset<32> instruction);
};

#endif // CONTROLUNIT_H