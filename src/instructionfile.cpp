#include "instructionfile.h"

InstructionFile::InstructionFile(std::string _memory_file)
    : File(_memory_file) {}

std::bitset<32> InstructionFile::read(std::bitset<32> address) {
  u_int32_t address_long = address.to_ulong();

  std::bitset<32> instruction = 0;
  for (int i = 0; i < 4; i++) {
    // Little Endian
    std::bitset<8> instr_word = data.at(address_long + i);
    instruction |= instr_word.to_ulong() << (i * 8);
  }
  return instruction;
}