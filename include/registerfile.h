#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include "file.hpp"

#include <bitset>
#include <iostream>
#include <map>

class RegisterFile : public File<5, 32> {
public:
  RegisterFile(std::string _memory_file = "reg");

  std::pair<std::bitset<32>, std::bitset<32>> read(std::bitset<5> reg1,
                                                   std::bitset<5> reg2);

  void write(std::bitset<5> reg, std::bitset<32> value);
};

#endif // REGISTERFILE_H