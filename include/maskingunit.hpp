#ifndef MASKINGUNIT_HPP
#define MASKINGUNIT_HPP

#include <bitset>

/// \brief Provides bit masking and concatenation utilities.
///
/// The MaskingUnit class offers methods for extracting and combining bit 
/// fields using std::bitset. These utilities are useful for instruction 
/// decoding and immediate value generation. Intended to represent some
/// very small piece of hardware, permitting it to be independently 
/// included within separate units. 
class MaskingUnit {
public:
  /// \brief Extracts a sequence of bits from the input bitset.
  ///
  /// Copies \p length bits starting from position \p index in \p bits into a
  /// new bitset of size \c M. Bits are copied in order, starting from the
  /// least significant bit.
  ///
  /// \tparam M The size of the output bitset.
  /// \tparam N The size of the input bitset.
  /// \param bits The input bitset to extract bits from.
  /// \param index The starting bit index in \p bits.
  /// \param length The number of bits to extract.
  /// \returns A bitset of size \c M containing the extracted bits.
  template <size_t M, size_t N>
  static std::bitset<M> hardwareMaskBits(std::bitset<N> bits, size_t index, size_t length) {
    std::bitset<M> masked_bits = 0;
    size_t j = 0;
    for (size_t i = index; i < index + length && i < N && j < M; i++, j++) {
      masked_bits.set(j, bits.test(i));
    }
    return masked_bits;
  }

  /// \brief Concatenates two bitsets into a single bitset.
  ///
  /// The lower bits are taken from \p low_bits and the higher bits from
  /// \p high_bits. The resulting bitset has size \c M + N, where the least
  /// significant bits come from \p low_bits.
  ///
  /// \tparam M The size of the lower bitset.
  /// \tparam N The size of the higher bitset.
  /// \param low_bits The lower portion of the result.
  /// \param high_bits The higher portion of the result.
  /// \returns A bitset of size \c M + N containing the concatenated bits.
  template <size_t M, size_t N>
  static std::bitset<M + N> concatBits(std::bitset<M> low_bits,
                                std::bitset<N> high_bits) {
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