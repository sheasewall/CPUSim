#ifndef FILE_HPP
#define FILE_HPP

// Include necessary headers
#include <iostream>
#include <map>
#include <bitset>
#include <fstream>
#include <sstream>

template <unsigned int K, unsigned int V>
class File {
protected:
    std::string memory_file;

    std::map<std::bitset<K>, std::bitset<V>, 
        std::function<bool(const std::bitset<K>&, const std::bitset<K>&)>> data;

public:
    File(std::string _memory_file) : memory_file(_memory_file), data([](const std::bitset<K>& lhs, const std::bitset<K>& rhs) {
            return lhs.to_ulong() < rhs.to_ulong();
        }) {
        std::ifstream file(memory_file, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open memory file: " + memory_file);
        }

        u_int32_t address = 0;
        char val;
        while (file.read(reinterpret_cast<char*>(&val), 1)) {
            try {
                std::bitset<K> address_bitset = std::bitset<K>(address);
                std::bitset<V> data_byte_bitset = std::bitset<V>(val);
                data.insert({ address_bitset, data_byte_bitset });
                address++;
            }
            catch (const std::invalid_argument& e) {
                throw std::runtime_error("Invalid address or value at address: " + std::to_string(address));
            }
            catch (const std::out_of_range& e) {
                throw std::runtime_error("Address or value out of range at address: " + std::to_string(address));
            }
        }
    }

    void print(std::string prefix = "") {
        for (auto& datum : data) {
            std::cout << prefix << std::setw(K) << std::setfill(' ') << std::dec << datum.first.to_ulong() << ": " << std::setw(V) << std::setfill('0') << std::hex << datum.second.to_ulong() << std::endl;
        }
    }

    void dump() {
        std::string dump_memory_file = memory_file + ".dump";
        std::ofstream file(dump_memory_file);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open memory file: " + dump_memory_file);
        }

        // This should be ordered
        for (auto& datum : data) {
            auto value = datum.second.to_ulong();
            file.write(reinterpret_cast<const char*>(&value), 1);
        }
    }
};

#endif // FILE_HPP