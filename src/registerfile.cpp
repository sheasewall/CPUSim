#include "registerfile.h"

RegisterFile::RegisterFile(std::string _memory_file) : File(_memory_file) {
  for (int i = 0; i < 32; i++) {
    data.insert({std::bitset<5>(i), std::bitset<32>(0)});
  }
}

std::pair<std::bitset<32>, std::bitset<32>>
RegisterFile::read(std::bitset<5> reg1, std::bitset<5> reg2) {
  return std::pair<std::bitset<32>, std::bitset<32>>{data.at(reg1),
                                                     data.at(reg2)};
}

void RegisterFile::write(std::bitset<5> reg, std::bitset<32> value) {
  if (reg == std::bitset<5>(0)) {
    return;
  }
  data.at(reg) = value;
}