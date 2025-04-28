#ifndef IMMGENUNIT_H
#define IMMGENUNIT_H

#include "constants.h"
#include "maskingunit.hpp"
#include <bitset>

struct ImmGenUnit {
  MaskingUnit mu;

  std::bitset<32> signExtend(std::bitset<12> imm);
  std::bitset<32> signExtend(std::bitset<20> imm);
  std::bitset<32> generateLong(std::bitset<20> imm);
  std::bitset<32> zeroExtend(std::bitset<5> imm);
};

#endif // IMMGENUNIT_H