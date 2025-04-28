#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <bitset>

const std::bitset<32> ZERO = 0x0000'0000;
const std::bitset<32> ONE = 0x0000'0001;
const std::bitset<32> FOUR = 0x0000'0004;
const std::bitset<32> TWENTY_SEVEN = 0x0000'001B;
const std::bitset<32> THIRTY_ONE = 0x0000'001F;
const std::bitset<32> ALL_ONES = 0xFFFF'FFFF;

const std::bitset<5> REG_ZERO = 0b00000;

#endif // CONSTANTS_H