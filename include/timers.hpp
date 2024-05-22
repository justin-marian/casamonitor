/*!
 * @file timers.hpp
 * @brief Timer configuration and utility functions for AVR.
 * 
 * @note This file provides the initialization and utility functions for
 * Timer1 on AVR uC. Timer1 is configured to generate a 1 ms time base, 
 * allowing for ms precision timing functions.
 * 
 * @link: Follow the link below to see about timers0/1/2 on AVR uC:
 * https://ocw.cs.pub.ro/courses/pm/lab/lab3-2023-2024
 */

#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define                 F_Atmega                    16000000UL
#define                 MILLIS                      1000UL
#define                 PRESCALER                   64UL
#define                 TIME_1MS                    ((F_Atmega / (MILLIS * PRESCALER)) - 1)

//! @brief Milliseconds counter - Timer1 16-bit
extern volatile unsigned long milliseconds;

// Higher Precision for DHT22/INA219

/** @brief Timer1 16-bit initialization */
void                    timer1_init                  (void);
/** @brief Timer1 16-bit milliseconds */
unsigned long           timer1_millis                (void);

#endif // TIMER_H
