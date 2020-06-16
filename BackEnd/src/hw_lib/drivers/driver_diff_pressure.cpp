
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
   
#include "driver_diff_pressure.h"
#include "../low_level/hardware_interface.h"
#include "../time.h"

DiffPressure::DiffPressure(int _analog_pin) : analog_pin(_analog_pin)
{

}

DiffPressure::~DiffPressure()
{
}

void DiffPressure::Begin(void)
{
    PinConfigAnalog(analog_pin);
    prev_update_time = Millis();
}

void DiffPressure::Loop(void)
{
    int i;
    uint32_t average;

    if (DIFFPRESSURE_UPDATE_PERIDO < GetDiffTime(Millis(), prev_update_time))
    {
        prev_update_time = Millis();
        //compute an averge to try to cancel noise
        average = 0;
        for (i = 0; i < 10; i++)
            average += PinReadAnalog(analog_pin);
        
        //if an interrupt happen, this is to make sure that last value is stored until the end 
        analog_value = average/i;
    }
}

//getter
float DiffPressure::GetDiffPressure(void)
{
    return (((float)analog_value) / ((float)kACDResolution)- DIFFPRESSURE_OFFSET) * DIFFPRESSURE_CONSTANT;
}