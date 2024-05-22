/*!
 * @file buff_atomic.hpp
 * @brief Atomic buffer implementation.
 * 
 * @note This file provides an implementation of a circular FIFO buffer
 * with atomic operations to ensure thread-safe access in a
 * interrupt-driven environment.
 * It includes a simple spinlock mechanism for atomicity.
 * 
 * @link: Follow the link below to see more about atomic operations:
 * https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
 */

#ifndef BUFF_ATOMIC_H
#define BUFF_ATOMIC_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#define                 FIFO_SIZE                       128U

//! atomicty with the values
typedef volatile uint8_t spinlock_t;

typedef struct PACKED {
    uint8_t buffer[FIFO_SIZE];
    spinlock_t head;
    spinlock_t tail;
} buff_t;

/** @brief Initializes the buffer. */
void buff_init(buff_t* buff);

/** @brief Checks if the buffer is empty. */
uint8_t buff_is_empty(buff_t* buff);
/** @brief Checks if the buffer is full. */
uint8_t buff_is_full(buff_t* buff);

/** @brief Puts a data byte into the buffer. */
uint8_t buff_put(buff_t* buff, uint8_t data);
/** @brief Gets a data byte from the buffer. */
uint8_t buff_get(buff_t* buff, uint8_t* data);

/** @brief Initializes the spinlock. */
void spinlock_init(spinlock_t* lock);
/** @brief Acquires the spinlock. */
void spinlock_acquire(spinlock_t* lock);
/** @brief Releases the spinlock. */
void spinlock_release(spinlock_t* lock);

#endif // BUFF_ATOMIC_H
