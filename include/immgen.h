#ifndef IMMGEN_H
#define IMMGEN_H

#include <bitset>

// This model should be reviewed. It can be potentially use a lot of 
// ALU functionality to achieve what it does here. There needs to be 
// some C++ specific type conversion that must be addressed somehow.
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

    static std::bitset<32> signExtend(std::bitset<20> imm_val) {
        std::bitset<32> imm = imm_val.to_ulong();
        if (imm.test(19)) {
            for (std::size_t i = 20; i <= 31; i++) {
                imm.set(i);
            }
        }
        return imm;
    }

    static std::bitset<32> generateLong(std::bitset<20> imm_val) {
        std::bitset<32> imm = 0;
        for (std::size_t i = 31; i >= 12; i--) {
            imm.set(i, imm_val.test(i - 12));
        }

        return imm;
    }

    static std::bitset<32> zeroExtend(std::bitset<5> imm_val) {
        return std::bitset<32>(imm_val.to_ulong());
    }
};

#endif // IMMGEN_H