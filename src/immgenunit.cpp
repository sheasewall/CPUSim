#include "immgenunit.h"

std::bitset<32> ImmGenUnit::signExtend(std::bitset<12> imm) {
  std::bitset<20> high_bits = mu.hardwareMaskBits<20, 32>(ZERO, 0, 20);
  if (imm.test(11)) {
    high_bits = mu.hardwareMaskBits<20, 32>(ALL_ONES, 0, 20);
  }
  return mu.concatBits<12, 20>(imm, high_bits);
}

std::bitset<32> ImmGenUnit::signExtend(std::bitset<20> imm) {
  std::bitset<12> high_bits = mu.hardwareMaskBits<12, 32>(ZERO, 0, 12);
  if (imm.test(19)) {
    high_bits = mu.hardwareMaskBits<12, 32>(ALL_ONES, 0, 12);
  }
  return mu.concatBits<20, 12>(imm, high_bits);
}

std::bitset<32> ImmGenUnit::generateLong(std::bitset<20> imm) {
  std::bitset<12> empty_low_bits = mu.hardwareMaskBits<12, 32>(ZERO, 0, 12);
  return mu.concatBits<12, 20>(empty_low_bits, imm);
}

std::bitset<32> ImmGenUnit::zeroExtend(std::bitset<5> imm) {
  std::bitset<27> empty_high_bits = mu.hardwareMaskBits<27, 32>(ZERO, 0, 27);
  return mu.concatBits<5, 27>(imm, empty_high_bits);
}