/*!
 * @file uint8t.h
 * @brief API for string manipulation with uint8_t.
 * Preffered for byte-arrays, in special for iterations over the buffer or edit parts of it.
 * 
 * @note Simple library for string manipulation using uint8_t instead of char/unsigned char.
 * The purpose of this header file is to provide a strong pedantic type for ATmega328P system (8 bit type).
 * 
 * @link Tips and Tricks to optimize C Code:
 * https://ww1.microchip.com/downloads/en/Appnotes/doc8453.pdf
 * 
 */

#ifndef UINT8T_H
#define UINT8T_H

typedef unsigned char uint8_t;

/**
 * @brief Tokenizes a string using delimiters.
 * 
 * @param str The input string to tokenize. Pass (uint8_t*)-1 to continue tokenizing the previous string.
 * @param delimiters A string of delimiter characters.
 * @param saveptr Pointer to a uint8_t* variable which holds the state between calls.
 * @return Pointer to the next token, or (uint8_t*)-1 if there are no more tokens.
 */
uint8_t* uint8_strtok(uint8_t* str, const uint8_t* delimiters, uint8_t** saveptr);
/**
 * @brief Converts a string of digits to an integer.
 * 
 * @param str The input string representing an integer.
 * @return The converted integer value.
 */
int uint8_atoi(const uint8_t* str);
/**
 * @brief Converts a string of digits to a floating-point number.
 * 
 * @param str The input string representing a floating-point number.
 * @return The converted floating-point value.
 */
float uint8_atof(const uint8_t* str);

#endif // UINT8T_H
