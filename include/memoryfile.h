#ifndef MEMORYFILE_H
#define MEMORYFILE_H

#include "file.hpp"

#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class MemoryFile : public File<32, 8> {
public:
    MemoryFile(std::string _memory_file) : File(_memory_file) {}

    // Could be handled by the ALU or an "add circuit"
    static std::bitset<32> incrementAddress(std::bitset<32> addr) {
        std::bitset<1> carry = 1;
        std::bitset<32> result;

        for (int i = 0; i < 32; i++) {
            result.set(i, addr.test(i) ^ carry.any());
            carry = addr.test(i) & carry.any();
        }

        return result;
    }

    // Read functions
    // Performs address incrementing operations,
    // which could be done by ALU. But I'm not sure
    // I want to introduce that dependency. 
    // Also uses string manipulation, which could be
    // replaced with bit manipulation
    std::bitset<32> readWord(std::bitset<32> address) {
        std::bitset<8> b1 = data.at(address);
        std::bitset<8> b2 = data.at(incrementAddress(address));
        std::bitset<8> b3 = data.at(incrementAddress(address));
        std::bitset<8> b4 = data.at(incrementAddress(address));

        return std::bitset<32>(b4.to_string() + b3.to_string() + b2.to_string() + b1.to_string());
    }

    std::bitset<32> readHalfWord(std::bitset<32> address) {
        std::bitset<8> b1 = data.at(address);
        std::bitset<8> b2 = data.at(incrementAddress(address));

        return std::bitset<32>(b2.to_string() + b1.to_string());
    }

    std::bitset<32> readByte(std::bitset<32> address) {
        return std::bitset<32>(data.at(address).to_string());
    }

    // This is dumb
    std::bitset<32> readUpperHalfWord(std::bitset<32> address) {
        std::bitset<8> b1 = data.at(address);
        std::bitset<8> b2 = data.at(incrementAddress(address));

        return std::bitset<32>(b2.to_string() + b1.to_string() + "0000000000000000");
    }

    std::bitset<32> readUpperByte(std::bitset<32> address) {
        return std::bitset<32>(data.at(address).to_string() + "00000000000000000000000000000000");
    }

    // Write functions
    // Has similar dependencies as read functions

    template <unsigned int N>
    void writeBytes(std::bitset<32> address, std::bitset<N * 8> value) {
        std::bitset<8> current_byte;
        std::bitset<32> current_address = address;

        for (int i = N - 1; 0 <= i; i--) {
            current_byte = std::bitset<8>(value.to_string().substr(i * 8, 8));
            data[current_address] = current_byte;
            current_address = incrementAddress(current_address);
        }
    }

    void writeWord(std::bitset<32> address, std::bitset<32> value) {
        writeBytes<4>(address, value);
    }

    void writeHalfWord(std::bitset<32> address, std::bitset<32> value) {
        std::bitset<16> half_word = std::bitset<16>(value.to_string().substr(16, 16));
        writeBytes<2>(address, half_word);
    }

    void writeByte(std::bitset<32> address, std::bitset<32> value) {
        std::bitset<8> byte = std::bitset<8>(value.to_string().substr(24, 8));
        writeBytes<1>(address, byte);
    }

    void print() {
        File::print("Address ");
    }
};

#endif // MEMORYFILE_H