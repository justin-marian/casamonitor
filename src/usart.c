#include "usart.h"

/**
 * @brief Initialize the USART controller
 *
 * How it works:
 *  - Set the BAUD rate
 *  - Enable the RX and TX channels
 *  - Set the data frame format (1 start bit, 8 data bits, 1 stop bit)
 *  - Start bit is a standard it doesn't need to be set (handled by HW)
 *  - Enable RX interrupt
 *  - Enable the USART controller
 */
void USART_init(void)
{
    // 9600 BAUD 8N1
    static uint32_t ubrr = UBRR_VALUE;                      // USART BAUD RATE
    UBRR0L = (uint8_t)(ubrr);                               // 9600 BAUD - LOW
    UBRR0H = (uint8_t)(ubrr >> 8);                          // 9600 BAUD - HIGH
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);                 // 1 START BIT, 8 DATA BITS, 1 STOP BIT
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);   // ENABLE RX - intrerrupt, TX (CHANNEL 0)
}

/**
 * @brief Receive a character via USART
 *
 * @return - the received character
 */
uint8_t USART_receive(void)
{
    while (!(UCSR0A & (1 << RXC0))) {
        ; /* Wait until the buffer is full */
    }

    /* Return the data from the buffer 
       the reception is done automatically */
    return UDR0;
}

/**
 * @brief Transmit to transmit a character via USART
 *
 * @param data - the character to be transmitted
 */
void USART_transmit(uint8_t data)
{
    while (!(UCSR0A & (1 << UDRE0))) {
        ; /* Wait until the buffer is empty */
    }

    /* Put the data into the buffer; 
       the transmission starts automatically */
    UDR0 = data;
}

/**
 * @brief Transmit a string via USART
 *
 * @param data - the null-terminated string to be transmitted
 */
void USART_print(const uint8_t *data)
{
    while (*data != '\0') {
        USART_transmit(*data++);
    }
}
