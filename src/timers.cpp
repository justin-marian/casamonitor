#include "timers.hpp"

volatile unsigned long milliseconds = 0;

/**
 * @brief Timer1 16-bit initialization
 */
void timer1_init(void)
{
    // Set Timer1 to CTC mode (Clear Timer on Compare Match)
    TCCR1B |= (1 << WGM12);
    // prescaler to 64 set
    TCCR1B |= (1 << CS11) | (1 << CS10);
    // 1ms intrerrupt
    OCR1A = TIME_1MS; // (16 * 10^6) / (64*1000) - 1 = 249
    // en timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    // init counter
    TCNT1 = 0;
    // en global interrupts
    sei();
}

//! Timer1 16-bit ISR - counts milliseconds passed
ISR(TIMER1_COMPA_vect)
{
    milliseconds++;
}

/**
 * @brief Timer1 16-bit milliseconds - Higher Precision for DHT22/INA219
 */
unsigned long timer1_millis(void)
{
    unsigned long ms;
    cli();
    ms = milliseconds;
    sei();
    return ms;
}
