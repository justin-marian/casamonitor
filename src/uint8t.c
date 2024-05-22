#include "uint8t.h"

/**
 * @brief Tokenizes a string using delimiters.
 * 
 * @param input The input string to tokenize. Pass (uint8_t*)-1 to continue tokenizing the previous string.
 * @param delimiters A string of delimiter characters.
 * @param saveptr Pointer to a uint8_t* variable which holds the state between calls.
 * @return Pointer to the next token, or (uint8_t*)-1 if there are no more tokens.
 */
uint8_t* uint8_strtok(uint8_t* input, const uint8_t* delimiters, uint8_t** saveptr) {
    uint8_t* token;
    uint8_t* pos;

    if (input == (uint8_t*)-1) {
        input = *saveptr;
    }

    // Skip leading delimiters
    while (*input) {
        int is_delim = 0;
        for (pos = (uint8_t*)delimiters; *pos; pos++) {
            if (*input == *pos) {
                is_delim = 1;
                break;
            }
        }
        if (!is_delim) {
            break;
        }
        input++;
    }

    if (*input == '\0') {
        *saveptr = input;
        return (uint8_t*)-1;
    }

    token = input;
    while (*input) {
        int is_delim = 0;
        for (pos = (uint8_t*)delimiters; *pos; pos++) {
            if (*input == *pos) {
                is_delim = 1;
                break;
            }
        }
        if (is_delim) {
            break;
        }
        input++;
    }

    if (*input) {
        *input = '\0';
        input++;
    }

    *saveptr = input;
    return token;
}

/**
 * @brief Converts a string of digits to an integer.
 * 
 * @param input The input string representing an integer.
 * @return The converted integer value.
 */
int uint8_atoi(const uint8_t* input) {
    int result = 0;
    int sign = 1;

    if (*input == '-') {
        sign = -1;
        input++;
    }

    while (*input) {
        if (*input >= '0' && *input <= '9') {
            result = result * 10 + (*input - '0');
        }
        input++;
    }

    return sign * result;
}

/**
 * @brief Converts a string of digits to a floating-point number.
 * 
 * @param input The input string representing a floating-point number.
 * @return The converted floating-point value.
 */
float uint8_atof(const uint8_t* input) {
    float result = 0.0;
    float fraction = 0.1;
    int sign = 1;
    int decimal = 0;

    if (*input == '-') {
        sign = -1;
        input++;
    }

    while (*input) {
        if (*input == '.') {
            decimal = 1;
            input++;
            continue;
        }
        if (*input >= '0' && *input <= '9') {
            if (decimal) {
                result += (*input - '0') * fraction;
                fraction /= 10;
            } else {
                result = result * 10 + (*input - '0');
            }
        }
        input++;
    }

    return sign * result;
}
