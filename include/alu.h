#ifndef ALU_H
#define ALU_H

#include <bitset>
#include <limits>
#include <stdexcept>

#include "maskingunit.hpp"

/**
 * @class ALU
 * @brief A class representing an Arithmetic Logic Unit (ALU) with various
 * bitwise and arithmetic operations.
 * @details
 * This ALU is modeled with the following following capabilities:
 * * bitwise nor over two 32-bit values
 * * left-shifting a 32-bit value, (a 32-bit value) number of times
 * * right-shifting a 32-bit value, (a 32-bit value) number of times
 * * bitwise comparing two 32-bit values (outputting a 1-bit value)
 * These capabilities are meant to model hardware functionality.
 *
 * Additionally, all basic bitwise operations, performed over 1..2 32-bit values
 * are implemented using only the NOR "hardware" circuit. That is:
 * *  ~
 * *  |
 * *  &
 * * ~&
 * *  ^
 * * ~^
 *
 * Two's complement addition and negation are also implemented using
 * only the "hardware" functionalities and bitwise operations.
 * Some conditional logic is used in the implementation, and it is
 * not entirely clear to me how this should be modelled, but right now
 * it is a boolean check.
 */
class ALU {
public:
  /**
   * @name Constructors and Destructors
   * @{
   */

  /**
   * @brief Default constructor for the ALU class.
   */
  ALU() {}

  /**
   * @brief Destructor for the ALU class.
   */
  ~ALU() {}

  /** @} */ // end of Constructors and Destructors

  /**
   * @name Hardware Operations
   * @{
   */

  /**
   * @brief Performs a bitwise NOR operation on two 32-bit bitsets.
   * @param val1 The first 32-bit bitset.
   * @param val2 The second 32-bit bitset.
   * @return The result of the NOR operation.
   */
  static std::bitset<32> hardwareNor(std::bitset<32> val1,
                                     std::bitset<32> val2) {
    return ~(val1 | val2);
  }

  /**
   * @brief Performs a left shift operation on a 32-bit bitset.
   * @param val The 32-bit bitset to be shifted.
   * @param shamt The amount to shift by, as a 32-bit bitset.
   * @return The result of the left shift operation.
   */
  static std::bitset<32> hardwareLeftShift(std::bitset<32> val,
                                           std::bitset<32> shamt) {
    return val << shamt.to_ulong();
  }

  /**
   * @brief Performs a right shift operation on a 32-bit bitset.
   * @param val The 32-bit bitset to be shifted.
   * @param shamt The amount to shift by, as a 32-bit bitset.
   * @return The result of the right shift operation.
   */
  static std::bitset<32> hardwareRightShift(std::bitset<32> val,
                                            std::bitset<32> shamt) {
    return val >> shamt.to_ulong();
  }

  /**
   * @brief Checks if two 32-bit bitsets are equal.
   * @param val1 The first 32-bit bitset.
   * @param val2 The second 32-bit bitset.
   * @return True if the bitsets are equal, false otherwise.
   */
  static bool hardwareIsEqual(std::bitset<32> val1, std::bitset<32> val2) {
    return val1 == val2;
  }

  /** @} */ // end of Hardware Operations

  /**
   * @name Bitwise Operations
   * @{
   */

  /**
   * @brief Performs a bitwise NOT operation on a 32-bit bitset.
   * @param val1 The 32-bit bitset.
   * @return The result of the NOT operation.
   */
  static std::bitset<32> bitwiseNot(std::bitset<32> val1);

  /**
   * @brief Performs a bitwise OR operation on two 32-bit bitsets.
   * @param val1 The first 32-bit bitset.
   * @param val2 The second 32-bit bitset.
   * @return The result of the OR operation.
   */
  static std::bitset<32> bitwiseOr(std::bitset<32> val1, std::bitset<32> val2);

  /**
   * @brief Performs a bitwise AND operation on two 32-bit bitsets.
   * @param val1 The first 32-bit bitset.
   * @param val2 The second 32-bit bitset.
   * @return The result of the AND operation.
   */
  static std::bitset<32> bitwiseAnd(std::bitset<32> val1, std::bitset<32> val2);

  /**
   * @brief Performs a bitwise NAND operation on two 32-bit bitsets.
   * @param val1 The first 32-bit bitset.
   * @param val2 The second 32-bit bitset.
   * @return The result of the NAND operation.
   */
  static std::bitset<32> bitwiseNand(std::bitset<32> val1,
                                     std::bitset<32> val2);

  /**
   * @brief Performs a bitwise XOR operation on two 32-bit bitsets.
   * @param val1 The first 32-bit bitset.
   * @param val2 The second 32-bit bitset.
   * @return The result of the XOR operation.
   */
  static std::bitset<32> bitwiseXor(std::bitset<32> val1, std::bitset<32> val2);

  /**
   * @brief Performs a bitwise XNOR operation on two 32-bit bitsets.
   * @param val1 The first 32-bit bitset.
   * @param val2 The second 32-bit bitset.
   * @return The result of the XNOR operation.
   */
  static std::bitset<32> bitwiseXnor(std::bitset<32> val1,
                                     std::bitset<32> val2);

  /** @} */ // end of Bitwise Operations

  /**
   * @name Arithmetic Operations
   * @{
   */

  /**
   * @brief Negates a 32-bit bitset.
   * @param val The 32-bit bitset to negate.
   * @return The negated bitset.
   */
  static std::bitset<32> negate(std::bitset<32> val);

  /**
   * @brief Adds two 32-bit bitsets using a recursive algorithm.
   * @details
   * The implementation does not address any specific bits individually.
   * This was done to avoid bit indexing. It does, however, use conditional
   * logic with hardwareIsEqual performing the comparison.
   * @param val1 The first 32-bit bitset.
   * @param val2 The second 32-bit bitset.
   * @return The result of the addition.
   */
  static std::bitset<32> add(std::bitset<32> val1, std::bitset<32> val2);

  static bool lessThanUnsigned(std::bitset<32> val1, std::bitset<32> val2);
  static bool lessThanSigned(std::bitset<32> val1, std::bitset<32> val2);

  static bool greaterThanEqualUnsigned(std::bitset<32> val1,
                                       std::bitset<32> val2);
  static bool greaterThanEqualSigned(std::bitset<32> val1,
                                     std::bitset<32> val2);

  static std::bitset<32> arithmeticRightShift(std::bitset<32> val,
                                              std::bitset<32> shamt);

  /** @} */ // end of Arithmetic Operations
};

#endif // ALU_H