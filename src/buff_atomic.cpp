#include "buff_atomic.hpp"

/**
 * @brief Initialize the buffer
 *
 * @param buff Pointer to the buffer to be initialized
 */
void buff_init(buff_t* buff)
{
    buff->head = 0;
    buff->tail = 0;
}

/**
 * @brief Add an element to the buffer (Producer)
 *
 * Used by the producer - ISR - to add new data to the buffer.
 *
 * @param buff Pointer to the buffer
 * @param data The data to be added to the buffer
 * @return 1 if the data was added, 0 if the buffer is full
 */
uint8_t buff_put(buff_t* buff, uint8_t data)
{
    uint8_t next = (buff->head + 1) % FIFO_SIZE;
    if (next == buff->tail) {
        return 0; // Buffer is full
    }
    buff->buffer[buff->head] = data;
    buff->head = next;
    return 1;
}

/**
 * @brief Get an element from the buffer (Consumer)
 * 
 * Used by the consumer - process_data_sensors - to retrieve data from the buffer.
 *
 * @param buff Pointer to the buffer
 * @param data Pointer to the variable where the retrieved data will be stored
 * @return 1 if data was retrieved, 0 if the buffer is empty
 */
uint8_t buff_get(buff_t* buff, uint8_t* data)
{
    if (buff->head == buff->tail) {
        return 0; // Buffer is empty
    }
    *data = buff->buffer[buff->tail];
    buff->tail = (buff->tail + 1) % FIFO_SIZE;
    return 1;
}

/**
 * @brief Check if the buffer is empty
 * 
 * If the head and tail indices are the same, the buffer is empty.
 *
 * @param buff Pointer to the buffer
 * @return 1 if the buffer is empty, 0 otherwise
 */
uint8_t buff_is_empty(buff_t* buff)
{
    return buff->head == buff->tail;
}

/**
 * @brief Check if the buffer is full
 *
 * If the buffer is full by calculating the next index of the head,
 * and comparing it to the tail index.
 *
 * @param buff Pointer to the buffer
 * @return 1 if the buffer is full, 0 otherwise
 */
uint8_t buff_is_full(buff_t* buff)
{
    return ((buff->head + 1) % FIFO_SIZE) == buff->tail;
}

/**
 * @brief Initialize the spinlock
 *
 * Spinlock by setting its value to 0.
 * ENSURE mtually exclusive access to shared resources.
 *
 * @param lock Pointer to the spinlock to be initialized
 */
void spinlock_init(spinlock_t* lock)
{
    *lock = 0;
}

/**
 * @brief Acquire the spinlock
 *
 * Spinlock by using an atomic test-and-set operation.
 * Attempts to set the lock until it is successful,
 * ENSURE exclusive access to the buffer.
 *
 * Protect critical sections of code that access the buffer. (producer-consumer)
 *
 * @param lock Pointer to the spinlock to be acquired
 */
void spinlock_acquire(spinlock_t* lock)
{
    while (__atomic_test_and_set(lock, __ATOMIC_ACQUIRE));
}

/**
 * @brief Release the spinlock
 *
 * Spinlock by using an atomic clear operation.
 * Some part of the code can acquire the lock and access the buffer.
 * ENSURE exclusive access to the buffer.
 * 
 * Release the lock after the operation is complete.
 * 
 * @param lock Pointer to the spinlock to be released
 */
void spinlock_release(spinlock_t* lock)
{
    __atomic_clear(lock, __ATOMIC_RELEASE);
}
