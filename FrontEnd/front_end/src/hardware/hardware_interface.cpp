
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

#include "hardware_interface.h"
#include <Arduino.h>

void (*uart_callback)(void)=nullptr;
void (*timer1m_callback)(void) = nullptr;

////////// Serial interface ////////
void UartConfigCallback(void (*callback)(void))
{
  uart_callback = callback;
}

void serialEvent1() {
  if(uart_callback!=nullptr)
    uart_callback();
}

void UartBegin(uint32_t baudrate)
{
  Serial1.begin(baudrate);
}

void UartWrite(uint8_t data)
{
  Serial1.write(data);
}

uint8_t UartRead(void)
{
  return Serial1.read();
}

bool UartAvailable(void){
  return Serial1.available();
}


//////////// Timers Interfacer /////////

//return the actual count of dedicated timer for timing aplication
uint16_t Timer1msCount(void)
{
  //return (uint16_t)TCNT2;
  return (((uint16_t)TCNT3H) << 8) + (uint16_t)TCNT3L;
}

//set an interrupt for each time that dedicated time for timing aplication reach 1ms period
void Timer1msISR(void (*callback)(void))
{
  timer1m_callback = callback;
  uint16_t compare_value = 16000;

  TCCR3A = 0x00; // mode 4, no outputs pin
  TCCR3B = 0x09; //preescaler value = 1. Ignore Input compare

  //this register define PWM frecuency
  OCR3AH = (compare_value >> 8) & 0x00ff; //config calculated compare value
  OCR3AL = (compare_value)&0x00ff;        //config calculated compare value

  TIMSK3 = 0x2; // intterrupt in compare match A
}

ISR(TIMER3_COMPA_vect)
{
  timer1m_callback();
}