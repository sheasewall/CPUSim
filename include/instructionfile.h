#ifndef INSTRUCTIONFILE_H
#define INSTRUCTIONFILE_H

#include "file.hpp"

#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class InstructionFile : public File<32, 8> {
public:
  InstructionFile(std::string _memory_file) : File(_memory_file) {}

  std::bitset<32> read(std::bitset<32> address) {
    u_int32_t address_long = address.to_ulong();

    std::bitset<32> instruction = 0;
    for (int i = 0; i < 4; i++) {
      // Little Endian
      std::bitset<8> instr_word = data.at(address_long + i);
      instruction |= instr_word.to_ulong() << (i * 8);
    }
    return instruction;
  }
};

#endif // INSTRUCTIONFILE_H