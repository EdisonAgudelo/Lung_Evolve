
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
   
#include <stdint.h>
#include <stdbool.h>
#include "driver_led.h"
#include <Arduino.h>

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
    led->ref_time = millis();
    led->period_death_ms = 0;
    led->n_times_ref = 0;
}

void DriverLedNBlink(DriverLed *led, uint32_t blink_period, int n_count)
{
    led->blink = true;
    led->period_death_ms = blink_period / 2;
    led->period_ms = blink_period / (2 * n_count);
    led->ref_time = millis();
    led->n_times_ref = n_count;
}

inline void DriverLedTurnOn(DriverLed *led)
{
    led->blink = false;
    led->state = kLedOn;
    set_pin(led->gpio_pin);
}

inline void DriverLedTurnOff(DriverLed *led)
{
    led->blink = false;
    led->state = kLedOff;
    clear_pin(led->gpio_pin);
}

////////////////////////////////////////////////////////////

void DriverLedLoop(DriverLed *led)
{
    if (led->blink)
    {
        //Serial.print("entra a led loop\n");
        switch (led->state)
        {
        case kLedOn:

            //transition control
            if (millis() - led->ref_time > led->period_ms)
            {
                led->ref_time = millis();
                clear_pin(led->gpio_pin); //positive logic
                led->state = kLedOff;
                led->n_times_count++;
            }
            break;

        case kLedOff:
            //transition control
            if (millis() - led->ref_time > led->period_ms)
            {
                led->ref_time = millis();

                if (led->n_times_count < led->n_times_ref || led->n_times_ref == 0)
                {
                    set_pin(led->gpio_pin); //positive logic
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
            if (millis() - led->ref_time > led->period_death_ms)
            {
                led->ref_time = millis();
                led->state = kLedOn;
                set_pin(led->gpio_pin); //positive logic
                led->n_times_count = 0;
            }
            break;

        default:
            //something was wrong, reset state machine
            led->ref_time = millis();
            set_pin(led->gpio_pin);
            led->state = kLedOn;
            break;
        }
    }
    else if (led->one_shoot)
    {
        if ((millis() - led->ref_time) > led->one_shoot_time)
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
    led->ref_time = millis();
    led->one_shoot_time = shoot_time_ms;
}

void DriverLedTShoot(DriverLed *led, uint32_t shoot_period_ms, uint32_t shoot_time)
{
    set_pin(led->gpio_pin);
    led->blink = true;
    led->state = kLedOn;
    led->period_death_ms = shoot_period_ms - 2*shoot_time;
    led->period_ms = shoot_time;
    led->ref_time = millis();
    led->n_times_ref = 1;   
}

void set_pin(int gpio)
{
  digitalWrite(gpio,HIGH);
}

void clear_pin(int gpio)
{
  digitalWrite(gpio,LOW);
}