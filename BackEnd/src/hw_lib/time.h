
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

#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>


//it gets the enlapsed time between two time references. This avoid issues related to uint32_t counter over flow 
uint32_t GetDiffTime(uint32_t actual, uint32_t prev);

double GetDiffTimeUs(double actual, double prev);

//it retuns the enlapsed time from the system power on in milliseconds. It has a maximum values of 49 days before counter reset. 
uint32_t Millis(void);

//it retuns the enlapsed time from the system power on in microseconds. It has a maximum values of 1 hour before counter reset. 
uint32_t Micros(void);

double MicrosDouble(void);

//It stucks main fw flow during a fixed time
void uDelay(uint32_t dalay_microseconds);

//It stucks main fw flow during a fixed time
void Delay(uint32_t delay_milliseconds);

//init timer for generate 1 ms interrupt.
void TimeVirtualISRBegin(void);

//this is a feature that allow accuracy timing for user application
void TimeVirtualISRAdd(int id, void (*user_callback)(void), uint32_t period);

#endif