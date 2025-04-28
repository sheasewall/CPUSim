#ifndef FILE_HPP
#define FILE_HPP

#include <bitset>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

template <unsigned int K, unsigned int V> class File {
protected:
  std::string memory_file;

  std::map<std::bitset<K>, std::bitset<V>,
           std::function<bool(const std::bitset<K> &, const std::bitset<K> &)>>
      data;

public:
  File(std::string _memory_file = "");

  void load(std::string save_file);
  void
  set_data(std::vector<std::pair<std::bitset<K>, std::bitset<V>>> new_data);
  void print(std::string prefix = "");
  void dump(std::streamsize size, std::string filename = "");
  std::bitset<V> read(std::bitset<K> address);
};

template <unsigned int K, unsigned int V>
File<K, V>::File(std::string _memory_file)
    : memory_file(_memory_file),
      data([](const std::bitset<K> &lhs, const std::bitset<K> &rhs) {
        return lhs.to_ulong() < rhs.to_ulong();
      }) {
  std::ifstream file(memory_file, std::ios::binary);

  bool should_load = true;
  if (!file.is_open()) {
    should_load = false;
  }

  if (should_load) {
    u_int32_t address = 0;
    char val;
    while (file.read(reinterpret_cast<char *>(&val), 1)) {
      try {
        std::bitset<K> address_bitset = std::bitset<K>(address);
        std::bitset<V> data_byte_bitset = std::bitset<V>(val);
        data.insert({address_bitset, data_byte_bitset});
        address++;
      } catch (const std::invalid_argument &e) {
        throw std::runtime_error("Invalid address or value at address: " +
                                 std::to_string(address));
      } catch (const std::out_of_range &e) {
        throw std::runtime_error("Address or value out of range at address: " +
                                 std::to_string(address));
      }
    }
  }
}

template <unsigned int K, unsigned int V>
void File<K, V>::load(std::string save_file) {
  std::ifstream file(save_file, std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("Could not open memory file: " + memory_file);
  }

  std::vector<std::pair<std::bitset<K>, std::bitset<V>>> new_data;
  u_int32_t address = 0;
  char val;
  while (file.read(reinterpret_cast<char *>(&val), 1)) {
    try {
      std::bitset<K> address_bitset = std::bitset<K>(address);
      std::bitset<V> data_byte_bitset = std::bitset<V>(val);
      new_data.insert({address_bitset, data_byte_bitset});
      address++;
    } catch (const std::invalid_argument &e) {
      throw std::runtime_error("Invalid address or value at address: " +
                               std::to_string(address));
    } catch (const std::out_of_range &e) {
      throw std::runtime_error("Address or value out of range at address: " +
                               std::to_string(address));
    }
  }

  set_data(new_data);
}

template <unsigned int K, unsigned int V>
void File<K, V>::set_data(
    std::vector<std::pair<std::bitset<K>, std::bitset<V>>> new_data) {
  data.clear();
  for (auto &datum : new_data) {
    data.insert(datum);
  }
}

template <unsigned int K, unsigned int V>
void File<K, V>::print(std::string prefix) {
  for (auto &datum : data) {
    std::cout << prefix << std::setw(K) << std::setfill(' ') << std::dec
              << datum.first.to_ulong() << ": " << std::setw(V)
              << std::setfill('0') << std::hex << datum.second.to_ulong()
              << std::endl;
  }
}

template <unsigned int K, unsigned int V>
void File<K, V>::dump(std::streamsize size, std::string filename) {
  if (filename.empty()) {
    filename = memory_file;
  }

  std::ofstream file(filename, std::ios::app | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open/create memory file: " + filename);
  }

  // This should be ordered
  for (auto &datum : data) {
    auto value = datum.second.to_ulong();
    file.write(reinterpret_cast<const char *>(&value), size);
  }
}

template <unsigned int K, unsigned int V>
std::bitset<V> File<K, V>::read(std::bitset<K> address) {
  auto it = data.find(address);
  if (it != data.end()) {
    return it->second;
  } else {
    throw std::runtime_error("Address not found in memory: " +
                             address.to_string());
  }
}

#endif // FILE_HPP