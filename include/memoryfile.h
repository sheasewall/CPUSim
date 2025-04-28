#ifndef MEMORYFILE_H
#define MEMORYFILE_H

#include "file.hpp"

#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class MemoryFile : public File<32, 8> {
public:
  MemoryFile(std::string _memory_file = "mem");

  std::bitset<32> readBytes(std::bitset<32> address, unsigned int N,
                            bool sign_extend = false);

  void writeBytes(std::bitset<32> address, std::bitset<32> _value,
                  unsigned int N);

  std::string signature();
};

#endif // MEMORYFILE_H