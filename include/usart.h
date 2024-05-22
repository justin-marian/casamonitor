/*!
 * @file usart.h
 * @brief USART Controller
 * 
 * @note ATmega328P USART Configuration - baud rate = 9600, CLK freq = 16MHz
 * The USART controller is configured to use the:
 *  8 data bits, 
 *  1 stop bit, 
 *  and no parity bit.
 *
 * The baud rate is set to 9600 using the UBRR register.
 * 
 * @link Explained in Depth, in the following link below:
 * https://www.arnabkumardas.com/arduino-tutorial/usart-programming/
 */

#ifndef USART_H
#define USART_H

#include <avr/io.h>

typedef unsigned char uint8_t;

#define             F_ATmega                    16000000UL                              // Clock Speed
#define             BAUD                        9600UL                                  // Baud Rate
#define             UBRR_VALUE                  ((F_ATmega / (BAUD * 16UL)) - 1UL)      // USART Baud Rate Register

/** @brief Initialize the USART controller */
void                USART_init                  (void);
/**
 * @brief Receive a character via USART (RX - blocking)
 *
 * @return - the received character
 */
uint8_t             USART_receive                (void);
/** 
 * @brief Transmit a character via USART (TX - blocking)
 * 
 *  @param data - the character to be transmitted
 */
void                USART_transmit              (uint8_t data);
/**
 * @brief Transmit a string via USART (TX - blocking)
 *
 * @param data - the null-terminated string to be transmitted
 */
void                USART_print                 (const uint8_t *data); 

#endif // USART_H
