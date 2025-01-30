#ifndef MEMORYFILE_H
#define MEMORYFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class DataMemory {
    std::unordered_map<std::bitset<32>, std::bitset<8>> data;

public:
    DataMemory(std::string memory_file) {
        std::ifstream file(memory_file);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open memory file: " + memory_file);
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string address;
            std::string value;
            if (!(iss >> address >> value)) {
                throw std::runtime_error("Error reading line: " + line);
            }

            try {
                std::bitset<32> address_bitset = std::bitset<32>(std::stoul(address));
                std::bitset<8> data_byte_bitset = std::bitset<8>(value);
                data.insert({ address_bitset, data_byte_bitset });
            }
            catch (const std::invalid_argument& e) {
                throw std::runtime_error("Invalid address or value in line: " + line);
            }
            catch (const std::out_of_range& e) {
                throw std::runtime_error("Address or value out of range in line: " + line);
            }
        }
    }

    void print() {
        for (auto& datum : data) {
            std::cout << "Address " << datum.first.to_ulong() << ": " << datum.second << std::endl;
        }
    }

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

    // Write functions
    // Has similar dependencies as read functions
    // Half word and byte writes must have options 
    // to sign extend or not

    void writeWord(std::bitset<32> address, std::bitset<32> value) {
        std::bitset<8> current_byte;
        std::bitset<32> current_address = address;

        for (int i = 3; 0 <= i; i--) {
            current_byte = std::bitset<8>(value.to_string().substr(i * 8, 8));
            data[current_address] = current_byte;
            current_address = incrementAddress(current_address);
        }
    }
};

#endif // MEMORYFILE_H