/*!
 * @file pinout.h
 * @brief Pinout Definitions
 * 
 * @note The pinout definitions for the ATmega328P microcontroller.
 * And macros for the parsing of the USART commands.
 * 
 * @link Explained in Depth, in the following link below:
 * https://www.arduino.cc/en/Hacking/PinMapping168
 */

#ifndef PINOUT_H
#define PINOUT_H

#define             ARG_LEN_MAX                 32UL                                    // Max ARG lenght for USART COM
#define             I2C_LEN_MAX                 100UL                                   // Max Current for Cooler/LED Dimmer
#define             USART_LEN_MAX               128UL                                   // Max CMD lenght for USART COM

#define             TIMER_INA219                10UL                                    // Max Time LED PWM inc brightness (IR Motion Sensor)
#define             TIMER_IR                    10UL                                    // Max Time IR Motion Sensor
#define             TIMER_DHT22                 1000UL                                  // Max Time DHT Sensor

#define             FADE_LIGHT                  15UL                                    // Time Fade Light
#define             LOW_LIGHT                   0UL                                     // LOW Light PWM - Slider
#define             HIGH_LIGHT                  255UL                                   // HIGH Light PWM - Slider
#define             TEMP_WANTED                 25.0F                                   // Temperature Wanted - Slider

#define             LEN_SHORT                   16UL                                    // Short String Size
#define             LEN_CHAR                    1UL                                     // Char Size

#define             FIN                         0x00U                                   // Frame Init '\0'
#define             CR                          0x0DU                                   // Carriage Return '\r'
#define             LF                          0x0AU                                   // Line Feed '\n'
#define             HASH                        0x23U                                   // Hash '#' Node-RED Separator
#define             DOLLAR                      0x24U                                   // Dollar Sign '$' Node-RED Separator

typedef enum CASA_MONITOR {
    FUNC_FORCE_LIGHT = 0,
    FUNC_LOW_LIGHT,
    FUNC_HIGH_LIGHT,
    FUNC_TEMP_WANTED,
    FUNC_FORCE_COOLER
} func_type_t;

static const uint8_t ir_mov_pin = 8U;        // Motion Sensor Pin (Digital)    
static const uint8_t dimmer_pin = 9U;        // Dimmer Pin (PWM)
static const uint8_t cooler_pin = 10U;       // Cooler Pin (Digital)
static const uint8_t dht_pin =    12U;       // DHT Sensor Pin (Digital) 
static const uint8_t on_off_pin = 14U;       // Switch Pin ON/OFF (Analog) A0

#endif // PINOUT_H
