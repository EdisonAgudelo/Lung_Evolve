
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

#ifndef _HARDWARE_INTERFACE_H_
#define _HARDWARE_INTERFACE_H_


#include <stdint.h>



void UartConfigCallback(void (*callback)(void));
void UartBegin(uint32_t baudrate);
bool UartAvailable(void);
void UartWrite(uint8_t data);
uint8_t UartRead(void);
//this configure one timer to has 1ms interrupt period
void Timer1msISR(void (*callback)(void));

//this return de actual count of timer who is generating 1ms interrupt
uint16_t Timer1msCount(void);


#endif