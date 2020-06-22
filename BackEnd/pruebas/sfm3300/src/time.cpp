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

#include "time.h"
#include "hardware_interface.h"


uint32_t GetDiffTime(uint32_t actual, uint32_t prev)
{
    if (actual >= prev)
    {
        return (actual - prev);
    }
    else
    { //when resgiter overflow
        return (0xffffffff - (prev - actual));
    }
}


uint32_t Millis(void)
{
    return millis();
}

uint32_t Micros(void)
{
   return micros();
}


void Delay(uint32_t delay_milliseconds)
{
delay(delay_milliseconds);
}

void uDelay(uint32_t dalay_microseconds)
{
    volatile uint8_t i;
    uint32_t time=Micros();
    
    /*
    dalay_microseconds*=0.9;
    while(GetDiffTime(Micros(),time)<dalay_microseconds)
    {
        i++;
    };*/
    
    do 
    {
        if(dalay_microseconds>0xff)
        {
            delayMicroseconds(0xff-0x4);
            dalay_microseconds-=0xff;
        }
        else
        {
            delayMicroseconds((uint8_t)dalay_microseconds);
            dalay_microseconds=0;
        }

        /* code */
    }while(dalay_microseconds!=0);
    
}

