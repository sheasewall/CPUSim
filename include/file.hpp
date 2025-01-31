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
        std::cout << "File<" << K << ", " << V << "> created" << std::endl;

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
                std::bitset<K> address_bitset = std::bitset<K>(std::stoull(address));
                std::bitset<V> data_byte_bitset = std::bitset<V>(value);
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

    void print(std::string prefix = "") {
        for (auto& datum : data) {
            std::cout << prefix << datum.first.to_ulong() << ": " << datum.second << std::endl;
        }
    }

    void dump() {
        std::string dump_memory_file = memory_file + ".dump";
        std::ofstream file(dump_memory_file);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open memory file: " + dump_memory_file);
        }

        for (auto& datum : data) {
            file << datum.first.to_ulong() << " " << datum.second << std::endl;
        }
    }
};

#endif // FILE_HPP