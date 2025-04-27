#include "alu.h"

std::bitset<32> ALU::bitwiseNot(std::bitset<32> val1)
{
    return hardwareNor(val1, val1);
}

std::bitset<32> ALU::negate(std::bitset<32> val)
{
    return add(bitwiseNot(val), std::bitset<32>(1));
}


std::bitset<32> ALU::bitwiseOr(std::bitset<32> val1, std::bitset<32> val2)
{
    return bitwiseNot(hardwareNor(val1, val2));
}

std::bitset<32> ALU::bitwiseAnd(std::bitset<32> val1, std::bitset<32> val2)
{
    return hardwareNor(bitwiseNot(val1), bitwiseNot(val2));
}

std::bitset<32> ALU::bitwiseNand(std::bitset<32> val1, std::bitset<32> val2)
{
    return bitwiseNot(bitwiseAnd(val1, val2));
}

std::bitset<32> ALU::bitwiseXor(std::bitset<32> val1, std::bitset<32> val2)
{
    return bitwiseAnd(bitwiseOr(val1, val2), bitwiseNand(val1, val2));
}

std::bitset<32> ALU::bitwiseXnor(std::bitset<32> val1, std::bitset<32> val2)
{
    return bitwiseNot(bitwiseXor(val1, val2));
}

std::bitset<32> ALU::add(std::bitset<32> val1, std::bitset<32> val2)
{
    std::bitset<32> preadd = bitwiseXor(val1, val2);
    std::bitset<32> carries = bitwiseAnd(val1, val2);
    std::bitset<32> carries_shifted = hardwareLeftShift(carries, std::bitset<32>(1));
    // We know the base case of carries_shifted == 0 will always reach 
    // as the hardware left shift is 0 filling, so an N-bit number
    // left shifted N times is always 0.
    if (hardwareIsEqual(carries_shifted, std::bitset<32>(0))) {
        return preadd;
    }
    return add(preadd, carries_shifted);
}

bool ALU::lessThanUnsigned(std::bitset<32> val1, std::bitset<32> val2)
{
    if (hardwareIsEqual(val1, val2)) {
        return false; 
    }

    std::bitset<32> highest_bit_val1 = hardwareRightShift(val1, std::bitset<32>(31));
    std::bitset<32> highest_bit_val2 = hardwareRightShift(val2, std::bitset<32>(31));

    std::bitset<32> high_bits_differ = bitwiseXor(highest_bit_val1, highest_bit_val2); 
    std::bitset<32> high_bits_differ_and_val1_is_greater = bitwiseAnd(high_bits_differ, highest_bit_val1);

    if (hardwareIsEqual(high_bits_differ_and_val1_is_greater, std::bitset<32>(1))) {
        return false;
    }

    std::bitset<32> high_bits_differ_and_val2_is_greater = bitwiseAnd(high_bits_differ, highest_bit_val2);
    if (hardwareIsEqual(high_bits_differ_and_val2_is_greater, std::bitset<32>(1))) {
        return true;
    }

    std::bitset<32> val1_advanced = hardwareLeftShift(val1, std::bitset<32>(1));
    std::bitset<32> val2_advanced = hardwareLeftShift(val2, std::bitset<32>(1));

    return lessThanUnsigned(val1_advanced, val2_advanced);
}

bool ALU::lessThanSigned(std::bitset<32> val1, std::bitset<32> val2) {
    std::bitset<32> highest_bit_val1 = hardwareRightShift(val1, std::bitset<32>(31));
    std::bitset<32> highest_bit_val2 = hardwareRightShift(val2, std::bitset<32>(31));

    std::bitset<32> high_bits_differ = bitwiseXor(highest_bit_val1, highest_bit_val2);
    std::bitset<32> high_bits_differ_and_val1_is_negative = bitwiseAnd(high_bits_differ, highest_bit_val1);

    if (hardwareIsEqual(high_bits_differ_and_val1_is_negative, std::bitset<32>(1))) {
        return true;
    }

    std::bitset<32> high_bits_differ_and_val2_is_negative = bitwiseAnd(high_bits_differ, highest_bit_val2);
    if (hardwareIsEqual(high_bits_differ_and_val2_is_negative, std::bitset<32>(1))) {
        return false;
    }

    std::bitset<32> val1_advanced = hardwareLeftShift(val1, std::bitset<32>(1));
    std::bitset<32> val2_advanced = hardwareLeftShift(val2, std::bitset<32>(1));

    return lessThanUnsigned(val1_advanced, val2_advanced);
}

bool ALU::greaterThanEqualUnsigned(std::bitset<32> val1, std::bitset<32> val2)
{
    if (hardwareIsEqual(val1, val2)) {
        return true; 
    }

    std::bitset<32> highest_bit_val1 = hardwareRightShift(val1, std::bitset<32>(31));
    std::bitset<32> highest_bit_val2 = hardwareRightShift(val2, std::bitset<32>(31));

    std::bitset<32> high_bits_differ = bitwiseXor(highest_bit_val1, highest_bit_val2); 
    std::bitset<32> high_bits_differ_and_val1_is_greater = bitwiseAnd(high_bits_differ, highest_bit_val1);

    if (hardwareIsEqual(high_bits_differ_and_val1_is_greater, std::bitset<32>(1))) {
        return false;
    }

    std::bitset<32> high_bits_differ_and_val2_is_greater = bitwiseAnd(high_bits_differ, highest_bit_val2);
    if (hardwareIsEqual(high_bits_differ_and_val2_is_greater, std::bitset<32>(1))) {
        return true;
    }

    std::bitset<32> val1_advanced = hardwareLeftShift(val1, std::bitset<32>(1));
    std::bitset<32> val2_advanced = hardwareLeftShift(val2, std::bitset<32>(1));

    return greaterThanEqualUnsigned(val1_advanced, val2_advanced);
}

bool ALU::greaterThanEqualSigned(std::bitset<32> val1, std::bitset<32> val2) {
    std::bitset<32> highest_bit_val1 = hardwareRightShift(val1, std::bitset<32>(31));
    std::bitset<32> highest_bit_val2 = hardwareRightShift(val2, std::bitset<32>(31));

    std::bitset<32> high_bits_differ = bitwiseXor(highest_bit_val1, highest_bit_val2);
    std::bitset<32> high_bits_differ_and_val1_is_negative = bitwiseAnd(high_bits_differ, highest_bit_val1);

    if (hardwareIsEqual(high_bits_differ_and_val1_is_negative, std::bitset<32>(1))) {
        return true;
    }

    std::bitset<32> high_bits_differ_and_val2_is_negative = bitwiseAnd(high_bits_differ, highest_bit_val2);
    if (hardwareIsEqual(high_bits_differ_and_val2_is_negative, std::bitset<32>(1))) {
        return false;
    }

    std::bitset<32> val1_advanced = hardwareLeftShift(val1, std::bitset<32>(1));
    std::bitset<32> val2_advanced = hardwareLeftShift(val2, std::bitset<32>(1));

    return greaterThanEqualUnsigned(val1_advanced, val2_advanced);
}

std::bitset<32> ALU::arithmeticRightShift(std::bitset<32> val, std::bitset<32> shamt)
{
    std::bitset<32> shamt_upper_27 = hardwareRightShift(shamt, std::bitset<32>(5));

    if (!hardwareIsEqual(shamt_upper_27, std::bitset<32>(0))) {
        return arithmeticRightShift(val, std::bitset<32>(31));
    }

    std::bitset<32> shamt_lower_5 = hardwareLeftShift(shamt, std::bitset<32>(27));
    shamt_lower_5 = hardwareRightShift(shamt_lower_5, std::bitset<32>(27));

    if (hardwareIsEqual(shamt_lower_5, std::bitset<32>(0))) {
        return val;
    }

    std::bitset<32> sign_bit = hardwareRightShift(val, std::bitset<32>(31));
    std::bitset<32> shifted_val = hardwareRightShift(val, std::bitset<32>(1));

    std::bitset<32> mask = hardwareLeftShift(sign_bit, std::bitset<32>(31));
    shifted_val = bitwiseOr(shifted_val, mask);

    std::bitset<32> neg_one = negate(std::bitset<32>(1));
    std::bitset<32> shamt_minus_one = add(shamt_lower_5, neg_one);
    return arithmeticRightShift(shifted_val, shamt_minus_one);
}

