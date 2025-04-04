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
    MemoryFile(std::string _memory_file = "mem") : File(_memory_file) {}

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

    // I chose to do this within the memory file as opposed to
    // within the ALU or the ImmGen in order to avoid exposing
    // the ALU in the ReadMemory stage of the pipeline. 
    // Whether or not this is actually something I should care
    // about, I am not entirely sure. Either way, I implement it
    // here using the simplest of logic in order to justify 
    // the fact that the sign extender could theoretically exist
    // within the 'memory chip'.
    static std::bitset<32> signExtendByteCircuit(std::bitset<32> value) {
        if (value.test(7)) {
            value |= 0xFFFFFF00;
        }
        else {
            value &= 0x000000FF;
        }
        return value;
    }

    static std::bitset<32> signExtendHalfWordCircuit(std::bitset<32> value) {
        if (value.test(15)) {
            value |= 0xFFFF0000;
        }
        else {
            value &= 0x0000FFFF;
        }
        return value;
    }

    std::bitset<32> readBytes(std::bitset<32> address, unsigned int N) {
        std::bitset<32> bytes;
        std::bitset<32> current_address = address;

        for (int i = 0; i < N; i++) {
            bytes = bytes | (std::bitset<32>(data[current_address].to_ulong()) << (i * 8));
            current_address = incrementAddress(current_address);
        }

        return bytes;
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



    void print(std::string prefix = "") {
        for (auto& datum : data) {
            std::cout << "Address " << std::setw(8) << std::setfill('0') << std::dec << datum.first.to_ulong() << ": " << std::setw(2) << std::setfill('0') << std::hex << datum.second.to_ulong() << std::endl;
        }
    }

    void dump() { 
        File::dump(1, "data");
    }

    std::string signature() {
        int i = 0;
        std::stringstream stream;
        std::bitset<32> final_memory;
        for (auto& datum : data) {
            std::bitset<8> mem = datum.second;
            final_memory |= mem.to_ulong() << (i * 8);
            i++;
            if (i == 4) {
                stream << std::setw(8) << std::setfill('0') << std::hex << final_memory.to_ulong() << std::endl;
                final_memory = 0;
                i = 0;
            }
        }
        return stream.str();
    }
};

#endif // MEMORYFILE_H