/*
    Lung Evolve Mechanical Ventilator
    Copyright (C) 2020  Edison Agudelo, Mateo Garcia, Alejandra Londoño

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see https://www.gnu.org/licenses/gpl-3.0.html.
    
    */

#include "driver_led.h"

#include "../low_level/hardware_interface.h"
#include "../time.h"

#include <stdint.h>
#include <stdbool.h>

#define NEG_LOGIC

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
#if defined(NEG_LOGIC)
    PinSetDigital(led->gpio_pin, false);
#else
    PinSetDigital(led->gpio_pin, true);
#endif
}

inline void DriverLedTurnOff(DriverLed *led)
{
    led->blink = false;
    led->state = kLedOff;
#if defined(NEG_LOGIC)
    PinSetDigital(led->gpio_pin, true);
#else
    PinSetDigital(led->gpio_pin, false);
#endif
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
            if (GetDiffTime(Millis(), led->ref_time) > led->period_ms)
            {
                led->ref_time = Millis();
#if defined(NEG_LOGIC)
                PinSetDigital(led->gpio_pin, true);
#else
                PinSetDigital(led->gpio_pin, false);    //positive logic
#endif
                led->state = kLedOff;
                led->n_times_count++;
            }
            break;

        case kLedOff:
            //transition control
            if (GetDiffTime(Millis(), led->ref_time) > led->period_ms)
            {
                led->ref_time = Millis();

                if (led->n_times_count < led->n_times_ref || led->n_times_ref == 0)
                {
#if defined(NEG_LOGIC)
                    PinSetDigital(led->gpio_pin, false);
#else
                    PinSetDigital(led->gpio_pin, true); //positive logic
#endif
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
#if defined(NEG_LOGIC)
                PinSetDigital(led->gpio_pin, false);
#else
                PinSetDigital(led->gpio_pin, true);     //positive logic
#endif
                led->n_times_count = 0;
            }
            break;

        default:
            //something was wrong, reset state machine
            led->ref_time = Millis();
#if defined(NEG_LOGIC)
            PinSetDigital(led->gpio_pin, false);
#else
            PinSetDigital(led->gpio_pin, true);
#endif
            led->state = kLedOn;
            break;
        }
    }
    else if (led->one_shoot)
    {
        if (GetDiffTime(Millis(), led->ref_time) > led->one_shoot_time)
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
#if defined(NEG_LOGIC)
    PinSetDigital(led->gpio_pin, false);
#else
    PinSetDigital(led->gpio_pin, true);
#endif
    led->blink = true;
    led->state = kLedOn;
    led->period_death_ms = shoot_period_ms - 2 * shoot_time;
    led->period_ms = shoot_time;
    led->ref_time = Millis();
    led->n_times_ref = 1;
}
