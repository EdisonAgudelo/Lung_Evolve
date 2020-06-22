
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

#ifndef _DRIVER_LED_H    
#define _DRIVER_LED_H

    #include <stdbool.h>
    #include <stdint.h>
    
   enum DriverLedEstates
    {
        kLedOn,
        kLedOff,
        kLedDeath,
    };
    
    typedef struct
    {
        int gpio_pin;
        enum DriverLedEstates state;
        bool blink;
        bool one_shoot;
        uint32_t one_shoot_time;
        uint32_t period_ms;
        uint32_t ref_time;

        //for blink n times
        uint32_t period_death_ms; 
        uint8_t n_times_count;
        uint8_t n_times_ref;
    } DriverLed;
    
    //constructor
    void DriverLedInit(DriverLed *led, int gpio_output);
    
    //methods

    void DriverLedNBlink(DriverLed *led, uint32_t blink_period, int n_count);
    void DriverLedBlink(DriverLed *led, uint32_t blink_period_ms);

    void DriverLedTShoot(DriverLed *led, uint32_t shoot_period_ms, uint32_t shoot_time);
    void DriverLedShoot(DriverLed *led, uint32_t shoot_time_ms);
    void DriverLedTurnOn(DriverLed *led);
    void DriverLedTurnOff(DriverLed *led);
    
    void DriverLedLoop(DriverLed *led);   
    
#endif 
