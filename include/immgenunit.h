#ifndef IMMGENUNIT_H
#define IMMGENUNIT_H

#include <bitset>

#include "maskingunit.hpp"

struct ImmGenUnit {
  const std::bitset<32> all_ones = 0xFFFFFFFF;
  const std::bitset<32> all_zeroes = 0x00000000;

  MaskingUnit mu;

  std::bitset<32> signExtend(std::bitset<12> imm) {
    std::bitset<20> high_bits = mu.maskBits<20, 32>(all_zeroes, 0, 20);
    if (imm.test(11)) {
      high_bits = mu.maskBits<20, 32>(all_ones, 0, 20);
      ;
    }
    return mu.concatBits<12, 20>(imm, high_bits);
  }

  std::bitset<32> signExtend(std::bitset<20> imm) {
    std::bitset<12> high_bits = mu.maskBits<12, 32>(all_zeroes, 0, 12);
    if (imm.test(19)) {
      high_bits = mu.maskBits<12, 32>(all_ones, 0, 12);
    }
    return mu.concatBits<20, 12>(imm, high_bits);
  }

  std::bitset<32> generateLong(std::bitset<20> imm) {
    std::bitset<12> empty_low_bits = mu.maskBits<12, 32>(all_zeroes, 0, 12);

    return mu.concatBits<12, 20>(empty_low_bits, imm);
  }

  std::bitset<32> zeroExtend(std::bitset<5> imm) {
    std::bitset<27> empty_high_bits = mu.maskBits<27, 32>(all_zeroes, 0, 27);

    return mu.concatBits<5, 27>(imm, empty_high_bits);
  }
};

#endif // IMMGENUNIT_H