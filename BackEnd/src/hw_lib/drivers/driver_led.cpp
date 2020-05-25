

#include "driver_led.h"

#include "hardware_interface.h"
#include "../time.h"

#include <stdint.h>
#include <stdbool.h>


void DriverLedInit(DriverLed *led, int gpio_output)
{
    led->gpio_pin = gpio_output;
    led->blink = false;
    led->one_shoot = false;
}

/////////////////////////////////////////////////////////////////

void DriverLedBlink(DriverLed *led, uint32_t blink_period)
{
    led->blink = true;
    led->period_ms = blink_period / 2; //for improve computation. Only makes this one time.
    //refer too led loop for understand
    led->ref_time = Millis();
    led->period_death_ms = 0;
    led->n_times_ref = 0;
}

void DriverLedNBlink(DriverLed *led, uint32_t blink_period, int n_count)
{
    led->blink = true;
    led->period_death_ms = blink_period / 2;
    led->period_ms = blink_period / (2 * n_count);
    led->ref_time = Millis();
    led->n_times_ref = n_count;
}

inline void DriverLedTurnOn(DriverLed *led)
{
    led->blink = false;
    led->state = kLedOn;
    PinSetDigital(led->gpio_pin, true);
}

inline void DriverLedTurnOff(DriverLed *led)
{
    led->blink = false;
    led->state = kLedOff;
    PinSetDigital(led->gpio_pin, false);
}

////////////////////////////////////////////////////////////

void DriverLedLoop(DriverLed *led)
{
    if (led->blink)
    {
        switch (led->state)
        {
        case kLedOn:

            //transition control
            if (Millis() - led->ref_time > led->period_ms)
            {
                led->ref_time = Millis();
                PinSetDigital(led->gpio_pin, false); //positive logic
                led->state = kLedOff;
                led->n_times_count++;
            }
            break;

        case kLedOff:
            //transition control
            if (Millis() - led->ref_time > led->period_ms)
            {
                led->ref_time = Millis();

                if (led->n_times_count < led->n_times_ref || led->n_times_ref == 0)
                {
                    PinSetDigital(led->gpio_pin, true); //positive logic
                    led->state = kLedOn;
                }
                else
                {
                    led->state = kLedDeath;
                }
            }
            break;

        case kLedDeath:
            //do nothing
            if (Millis() - led->ref_time > led->period_death_ms)
            {
                led->ref_time = Millis();
                led->state = kLedOn;
                PinSetDigital(led->gpio_pin, true); //positive logic
                led->n_times_count = 0;
            }
            break;

        default:
            //something was wrong, reset state machine
            led->ref_time = Millis();
            PinSetDigital(led->gpio_pin, true);
            led->state = kLedOn;
            break;
        }
    }
    else if (led->one_shoot)
    {
        if ((Millis() - led->ref_time) > led->one_shoot_time)
        {
            led->one_shoot = false;
            DriverLedTurnOff(led);
        }
    }
}

void DriverLedShoot(DriverLed *led, uint32_t shoot_time_ms)
{
    led->one_shoot = true;
    DriverLedTurnOn(led);
    led->ref_time = Millis();
    led->one_shoot_time = shoot_time_ms;
}

void DriverLedTShoot(DriverLed *led, uint32_t shoot_period_ms, uint32_t shoot_time)
{
    PinSetDigital(led->gpio_pin, true);
    led->blink = true;
    led->state = kLedOn;
    led->period_death_ms = shoot_period_ms - 2*shoot_time;
    led->period_ms = shoot_time;
    led->ref_time = Millis();
    led->n_times_ref = 1;   
}
