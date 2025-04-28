#ifndef MEMORYFILE_H
#define MEMORYFILE_H

#include "file.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class MemoryFile : public File<32, 8> {
public:
  MemoryFile(std::string _memory_file = "mem") : File(_memory_file) {}

  std::bitset<32> readBytes(std::bitset<32> address, unsigned int N,
                            bool sign_extend = false) {
    uint8_t current_byte = 0;
    uint32_t current_address = address.to_ulong();
    uint32_t value = 0;

    for (int i = 0; i < N; i++) {
      current_byte = data[current_address].to_ulong();
      value |= current_byte << (i * 8);
      current_address++;
    }

    if (sign_extend) {
      if (value & (0b1 << (8 * N - 1))) {
        value |= (0xFFFFFFFF << (8 * N));
      }
    } else {
      value &= (0xFFFFFFFF >> (8 * (4 - N)));
    }

    return std::bitset<32>(value);
  }

  void writeBytes(std::bitset<32> address, std::bitset<32> _value,
                  unsigned int N) {
    uint8_t current_byte = 0;
    uint32_t current_address = address.to_ulong();
    uint32_t value = _value.to_ulong();

    for (int i = 0; i < N; i++) {
      current_byte = value >> (i * 8) & 0xFF;
      data[current_address] = current_byte;
      current_address++;
    }
  }

  std::string signature() {
    // read until 0x6f5ca309
    // then start writing
    // until 0x6f5ca309
    // then stop writing
    int i = 0;
    bool should_write = false;
    std::stringstream stream;
    std::bitset<32> final_memory;
    for (auto &datum : data) {
      std::bitset<8> mem = datum.second;
      final_memory |= mem.to_ulong() << (i * 8);
      i++;
      if (i == 4) {
        if (final_memory == 0x6f5ca309) {
          if (should_write) {
            stream << std::setw(8) << std::setfill('0') << std::hex
                   << final_memory.to_ulong() << std::endl;
            break;
          }
          should_write = true;
        }
        if (should_write) {
          stream << std::setw(8) << std::setfill('0') << std::hex
                 << final_memory.to_ulong() << std::endl;
        }
        final_memory = 0;
        i = 0;
      }
    }
    return stream.str();
  }
};

#endif // MEMORYFILE_H