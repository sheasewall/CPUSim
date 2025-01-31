#ifndef IMMGEN_H
#define IMMGEN_H

#include <bitset>

struct ImmGen {
    static std::bitset<32> signExtend(std::bitset<12> imm_val) {
        std::bitset<32> imm = imm_val.to_ulong();
        if (imm.test(11)) {
            for (std::size_t i = 12; i <= 31; i++) {
                imm.set(i);
            }
        }
        return imm;
    }

    static std::bitset<32> generateLong(std::bitset<20> imm_val) {
        std::bitset<32> imm = 0;
        imm.set(31, imm_val.test(19));
        imm.set(30, imm_val.test(18));


        for (std::size_t i = 31; i >= 12; i--) {
            imm.set(i, imm_val.test(i - 12));
        }

        return imm;
    }
};

#endif // IMMGEN_H