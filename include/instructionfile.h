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
  InstructionFile(std::string _memory_file);

  std::bitset<32> read(std::bitset<32> address);
};

#endif // INSTRUCTIONFILE_H