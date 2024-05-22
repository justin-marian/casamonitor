/*!
 * @file regbit.h
 * @brief Bit manipulation macros.
 * 
 * @note This header file provides macros for bit manipulation.
 * 
 * @link Working with bit-level registers:
 * https://ocw.cs.pub.ro/courses/pm/tutorial/biti
 */

#ifndef REGBIT_H
#define REGBIT_H

/** @brief Selecting a bit B from a register R */
#define                 BIT(B)                                        (1 << (B))
/** @brief Set a bit => 0->1, 1->1 (unchanged) */
#define                 SET_BIT(PORT, B)                              ((PORT) |= BIT(B))
/** @brief Clear a bit => 1->0, 0->0 (unchanged) */
#define                 CLR_BIT(PORT, B)                              ((PORT) &= ~BIT(B))
/** @brief Toggle a bit => 1->0, 0->1 */
#define                 TOG_BIT(PORT, B)                              ((PORT) ^= BIT(B))
/** @brief Get the value of a bit (returns 0 or 1) */
#define                 GET_BIT(PORT, B)                              (((PORT) & BIT(B))) >> (B)
/** @brief Set a bit to a specific value (VAL should be 0 or 1) */
#define                 SET_BIT_VAL(PORT, B, VAL)                     ((PORT) = ((PORT) & ~BIT(B)) | ((VAL) << (B)))
/** @brief Set bits specified by a mask to a specific value */
#define                 SET_BIT_VAL_MASK(PORT, VAL, MASK)             ((PORT) = ((PORT) & ~(MASK)) | ((VAL) & (MASK)))
/** @brief Set bits specified by a mask to the inverted value */
#define                 SET_BIT_VAL_MASK_INV(PORT, VAL, MASK)         ((PORT) = ((PORT) & ~(MASK)) | (~(VAL) & (MASK)))

#endif // REGBIT_H
