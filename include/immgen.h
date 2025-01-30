#ifndef IMMGEN_H
#define IMMGEN_H

#include <bitset>

struct ImmGen {
    // Add your methods here
    static std::bitset<32> signExtend(std::bitset<12> imm_val) {
        std::bitset<32> imm = imm_val.to_ulong();
        if (imm.test(11)) {
            for (std::size_t i = 12; i <= 31; i++) {
                imm.set(i);
            }
        }
        return imm;
    }
};

#endif // IMMGEN_H