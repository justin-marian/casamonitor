#include <avr/io.h>

extern "C" {
    #include "usart.h"
    #include "uint8t.h"
}

#include "regbit.h"
#include "pinout.h"
#include "timers.hpp"
#include "buff_atomic.hpp"

#include "DHT.h"
#include "Adafruit_INA219.h"

//========CONFIG INIT VALS========//
static uint8_t low_light = LOW_LIGHT;                           // 0 - 255 (MIN) - PWM
static uint8_t high_light = HIGH_LIGHT;                         // 0 - 255 (MAX) - PWM
static float wanted_temp = TEMP_WANTED;                         // Celsius - TEMPERATURE wanted - USART
//======END CONFIG INIT VALS======//

//========MONITOR VARS========//
static int status_on_off = 0;                                   // ON/OFF SWITCH - HIGH/LOW
static int time_light_on = 0;                                   // IR MOTION SENSOR - LIGHT ON
//----------------------------------
static float i2c_volt = 0;                                      // INA219 - BUS VOLTAGE
static int pwm_light = 0;                                       // PWM LIGHT - DIMMER
//----------------------------------
static uint8_t manual_light_on = 0;                             // MANUAL LIGHT ON/OFF - SLIDER / SWITCH
static uint8_t manual_cooler_on = 0;                            // MANUAL COOLER ON/OFF - SLIDER
//========END MONITOR VARS========//

//========TIMER LEN========//
static unsigned long timer_dht22 = 0;                           //  - DHT22 (temperature/humidity)
static unsigned long timer_ina219 = 0;                          //  - INA219 (current/power/voltage)
//========END TIMER LEN========//

//========FADE LIGHT PWM/TIMERS========//
static int fade_target = 0;                                     // target light value [0-255]
static unsigned long last_fade_update = 0;                      // last update time
static unsigned long fade_interval = FADE_LIGHT;                // 15ms - smooth transition
//========END FADE LIGHT PWM/TIMERS========//

//========USART CMDS BUFF========//
// (requires a spinlock for atomicity)
static buff_t usart_fifo = buff_t();                            // USART FIFO buffer - DHT22 / IR / INA219
static spinlock_t usart_lock = spinlock_t();                    // USART spinlock - atomicity
//========END USART CMDS BUFF========//

//========CURRENT BUFF I2C========//
// (doesn't require a spinlock - half-duplex communication,
// no other i2c devices on the bus, no need for atomicity)
static uint8_t i2c_size = 0;                                    // I2C buffer size - current values
static uint8_t i2c_buff[I2C_LEN_MAX] = { 0 };                   // I2C buffer - current/power/voltage
//========END CURRENT BUFF USART========//

//========SENSORS========//
DHT dht(dht_pin, DHT22);
Adafruit_INA219 ina219;
//========END SENSORS========//

/*
 * @brief Similar to the problem Producer-Consumer (1-1): 
 * USART RX interrupt is the producer. (puts data into the buffer)
 * main loop is the consumer. (gets data from the buffer)
 */
ISR(USART_RX_vect)
{
    uint8_t recv_byte = UDR0;

    if (recv_byte != CR && recv_byte != LF) {
        spinlock_acquire(&usart_lock);
        buff_put(&usart_fifo, recv_byte);
        spinlock_release(&usart_lock);
    }
}

void setup(void)
{
    // Initialize USART channel
    USART_init();
    buff_init(&usart_fifo);
    spinlock_init(&usart_lock);

    // Initialize sensors
    dht.begin();
    ina219.begin();

    // IR Motion Sensor Pin (Digital)
    CLR_BIT(DDRB, DDB0);  // Set pin 8 (PB0) as input
    // Dimmer Pin (PWM)
    SET_BIT(DDRB, DDB1);  // Set pin 9 (PB1/OC1A) as output
    // Cooler Pin (Digital)
    SET_BIT(DDRB, DDB2);  // Set pin 10 (PB2) as output
    // Switch Pin ON/OFF (Analog)
    CLR_BIT(DDRC, DDC0);  // Set pin A0 (PC0) as input
    SET_BIT(PORTC, PC0);  // Enable pull-up resistor on pin A0 (PC0)

    // Initialize output states
    CLR_BIT(PORTB, PORTB2);  // Set cooler pin low

    timer1_init();
}

/*
 * @brief Get the average current value.
 * 
 * @return The average current value.
 */
float getCurrentAverage(void)
{
    float sum = 0.0;
    for(uint8_t i = 0; i < i2c_size; ++i) {
        sum += i2c_buff[i];
    }
    return (i2c_size > 0) ? (sum / i2c_size) : 0.0;
}

/*
 * @brief Send the information string via USART to Node-RED. 
 *
 * The string contains the following information:
 * - Temperature
 * - Humidity
 * - Light On/Off
 * - Voltage
 * - Power
 * - Current
 */
void send_to_nodered(void)
{
    //* SENSOR BUFFERS *//

    // DHT22 SENSOR
    static char buff_temp[LEN_SHORT] = { 0 };
    static char buff_hum[LEN_SHORT] = { 0 };
    // IR DS30P1 SENSOR - LIGHT ON/OFF / SLIDER NODE-RED
    static char buff_light[2 * LEN_CHAR] = { 0 }; // + '\0'
    // INA219 SENSOR
    static char buff_volt[LEN_SHORT] = { 0 };
    static char buff_pow[LEN_SHORT] = { 0 };
    static char buff_curr[LEN_SHORT] = { 0 };

    //* DHT22 SENSOR
    dtostrf(dht.readTemperature(), 6, 2, buff_temp);
    dtostrf(dht.readHumidity(), 6, 2, buff_hum);
    //* IR DS30P1 SENSOR - LIGHT ON/OFF / SLIDER NODE-RED
    bool on_off = time_light_on || manual_light_on || status_on_off;
    snprintf(buff_light, sizeof(buff_light), "%d", on_off);
    //* INA219 SENSOR
    dtostrf(i2c_volt, 6, 2, buff_volt);
    dtostrf(i2c_volt * getCurrentAverage() / 1000, 6, 2, buff_pow);
    dtostrf(getCurrentAverage(), 6, 2, buff_curr);

    // Static message buffer
    static char nodered_data[USART_LEN_MAX] = { 0 };
    snprintf(nodered_data, sizeof(nodered_data), "%s$%s$%s$%s$%s$%s\n",
             buff_temp, buff_hum,
             buff_light,
             buff_volt, buff_pow, buff_curr);

    // Send the concatenated string over USART
    USART_print((const uint8_t *)nodered_data);
}

/*
 * @brief Fade the light to a specific value with a delay.
 */
void fade_light_to(int target)
{
    fade_target = target;
}

/*
 * @brief Fade the light to a specific value with a delay.
 * Similar to a dimmer switch, smooth transition.
 * 
 * @param target The target light value.
 */
void update_fade_light(void)
{
    unsigned long current_time = timer1_millis();
    if (current_time - last_fade_update >= fade_interval) {
        if (pwm_light != fade_target) {
            pwm_light += (fade_target > pwm_light) ? 1 : -1;
            analogWrite(dimmer_pin, pwm_light);
        }
        last_fade_update = current_time;
    }
}

/*
 * @brief Process the command string received via USART.
 */
void exec_func(uint8_t* buff_cmds)
{
    uint8_t *args[ARG_LEN_MAX] = { 0 };
    uint8_t arg_idx = 0;

    // Tokenize the command string
    uint8_t* token = uint8_strtok(buff_cmds, (const uint8_t*)"$#", &buff_cmds);
    while (token != (uint8_t*)-1 && arg_idx < ARG_LEN_MAX) {
        args[arg_idx++] = token;
        token = uint8_strtok((uint8_t*)-1, (const uint8_t*)"$#", &buff_cmds);
    }

    // Ensure that there is at least one argument
    if (arg_idx == 0) return;

    // Monitor executes the function based on the first argument
    func_type_t casa_monitor = (func_type_t)uint8_atoi(args[0]);

    // Execute the function based on the monitor
    switch (casa_monitor)
    {
        case FUNC_FORCE_LIGHT: {
            if (arg_idx > 1) { manual_light_on = uint8_atoi(args[1]); }
            break;
        }
        case FUNC_LOW_LIGHT: {
            if (arg_idx > 1) { low_light = uint8_atoi(args[1]); }
            break;
        }
        case FUNC_HIGH_LIGHT: {
            if (arg_idx > 1) { high_light = uint8_atoi(args[1]); }
            break;
        }
        case FUNC_TEMP_WANTED: {
            if (arg_idx > 1) { wanted_temp = uint8_atof(args[1]); }
            break;
        }
        case FUNC_FORCE_COOLER: {
            if (arg_idx > 1) { manual_cooler_on = uint8_atoi(args[1]); }
            break;
        }
        default: {
            USART_print((const uint8_t*)"Invalid command...\r\n");
            break;
        }
    }
}

/*
 * @brief Await serial data from the USART channel.
 */
void process_data_sensors(void)
{
    uint8_t cmd = 0;
    static uint8_t cmd_buffer[USART_LEN_MAX] = { 0 };
    static uint8_t cmd_index = 0;

    while (!buff_is_empty(&usart_fifo)) {
        spinlock_acquire(&usart_lock);
        if (buff_get(&usart_fifo, &cmd)) {
            // Ignore special characters: CR, LF
            if (cmd == CR || cmd == LF) {
                spinlock_release(&usart_lock);
                continue;
            }

            // Check for end of command
            if (cmd == HASH) {
                cmd_buffer[cmd_index] = FIN;
                exec_func(cmd_buffer); 
                cmd_index = 0;
            } else {
                if (cmd_index < USART_LEN_MAX - 1) {
                    cmd_buffer[cmd_index++] = cmd;
                }
            }
        }
        spinlock_release(&usart_lock);
    }
}

void loop(void)
{
    // Read the IR motion sensor
    process_data_sensors();
    // Read the bus voltage
    i2c_volt = ina219.getBusVoltage_V();
    // Read the status of the on/off switch
    status_on_off = GET_BIT(PINC, PC0);

    // IR is ACTIVE - CHECK MOTION
    if (GET_BIT(PINB, PINB0)) {
        time_light_on = TIMER_IR;
    }

    // TIMER DHT22 - CHECK TEMPERATURE/HUMIDITY
    if ((timer1_millis() - timer_dht22) > TIMER_DHT22)
    {
        send_to_nodered();
        
        // Light ON/OFF - Fade
        if (time_light_on > 0) {
            fade_light_to(high_light);
            time_light_on--;
        } else if (manual_light_on || status_on_off == HIGH) {
            fade_light_to(high_light);
        } else {
            fade_light_to(low_light);
        }

        // Cooler ON/OFF
        if (dht.readTemperature() > wanted_temp || manual_cooler_on) {
            SET_BIT(PORTB, PORTB2); // ON
        } else {
            CLR_BIT(PORTB, PORTB2); // OFF
        }
        
        timer_dht22 = timer1_millis();
    }

    // TIMER INA219 - CHECK CURRENT/POWER/VOLTAGE
    if ((timer1_millis() - timer_ina219) > TIMER_INA219)
    {
        float current = ina219.getCurrent_mA() * -1;

        if (i2c_size < I2C_LEN_MAX) {
            i2c_buff[i2c_size++] = current;
        } else {
            for (uint8_t i = 1; i < I2C_LEN_MAX; ++i) {
                i2c_buff[i - 1] = i2c_buff[i];
            }
            i2c_buff[I2C_LEN_MAX - 1] = current;
        }
        
        timer_ina219 = timer1_millis();
    } 

    update_fade_light();
}
