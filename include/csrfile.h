#ifndef CSRFILE_H
#define CSRFILE_H

#include "file.hpp"

#include <bitset>
#include <iostream>
#include <map>

// Include for docker container compilation
#include <cmath>

// This is essentially a register file with 12 bit 
// addresses instead of 5 bit. 
class CSRFile : public File<12, 32> {
public:
    CSRFile(std::string _memory_file = "csr") : File(_memory_file) {
        for (int i = 0; i < std::pow(2, 12); i++) {
            data.insert({ std::bitset<12>(i), std::bitset<32>(0) });
        }
    }

    std::bitset<32> read(std::bitset<12> csr) {
        return data.at(csr);
    }

    void write(std::bitset<12> reg, std::bitset<32> value) {
        data.at(reg) = value;
    }
};

#endif // CSRFILE_H