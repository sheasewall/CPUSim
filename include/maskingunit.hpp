#ifndef MASKINGUNIT_HPP
#define MASKINGUNIT_HPP

#include <bitset>

class MaskingUnit {
public:
    template <size_t M, size_t N>
    std::bitset<M> maskBits(std::bitset<N> bits, size_t index, size_t length) {
        std::bitset<M> masked_bits = 0;
        size_t j = 0;
        for (size_t i = index; i < index + length && i < N && j < M; i++, j++) {
            masked_bits.set(j, bits.test(i));
        }
        return masked_bits;
    }

    template <size_t M, size_t N>
    std::bitset<M + N> concatBits(std::bitset<M> low_bits, std::bitset<N> high_bits) {
        std::bitset<M + N> concatted_bits = 0;
        size_t j = 0;
        for (size_t i = 0; i < M && j < M + N; i++, j++) {
            concatted_bits.set(j, low_bits.test(i));
        }
        for (size_t i = 0; i < N && j < M + N; i++, j++) {
            concatted_bits.set(j, high_bits.test(i));
        }
        return concatted_bits;
    }

};

#endif // MASKINGUNIT_HPP