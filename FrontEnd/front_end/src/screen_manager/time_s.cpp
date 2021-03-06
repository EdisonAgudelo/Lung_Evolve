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

#include "time_s.h"
#include "../hardware/hardware_interface.h"
#include <Arduino.h>

static void TimerRoutineCallback(void);

#define TIME_SISTICK_PERIOD 16000 //indicate the maximun value of timer/couter to reach 1ms for actual clk input

#define TIME_MAX_VIRTUAL_ISR 4 //this is to hold good performance

volatile uint32_t g_milliseconds = 0;
volatile int8_t g_timer_ignore = -1;

struct TimeStack
{
    void (*user_callback)(void);
    uint32_t count_back_time;

} time_virtual_stack[TIME_MAX_VIRTUAL_ISR];

volatile bool time_out = false;

static void StopDelay(void)
{
    time_out = true;
}

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

double GetDiffTimeUs(double actual, double prev)
{
    if (actual >= prev)
    {
        return (actual - prev);
    }
    else
    { //when resgiter overflow
        return (((double)0xffffffff)/(1000000.0) - (prev - actual));
    }
}

static void TimerRoutineCallback(void)
{
    uint8_t i;
    g_milliseconds++;

    for (i = 0; i < TIME_MAX_VIRTUAL_ISR; i++)
    {
        if (time_virtual_stack[i].user_callback != nullptr && g_timer_ignore != i)
        {
            time_virtual_stack[i].count_back_time--;
            if (0 == time_virtual_stack[i].count_back_time)
            {
                time_virtual_stack[i].user_callback();
                time_virtual_stack[i].user_callback = nullptr;
            }
        }
    }
}

uint32_t Millis(void)
{
    return g_milliseconds;
}

uint32_t Micros(void)
{
    uint16_t Core_timer = Timer1msCount();
    return (g_milliseconds * 1000 + (Core_timer * 1000) / TIME_SISTICK_PERIOD);
}

double MicrosDouble(void)
{
    double Core_timer = (double)Timer1msCount();
    return (((double)g_milliseconds) * 1000.0 + (Core_timer * 1000.0) /((double) TIME_SISTICK_PERIOD));
}

void Delay(uint32_t delay_milliseconds)
{
    time_out = false;
    TimeVirtualISRAdd(3,StopDelay,delay_milliseconds);
    while(1)
    {
        if(time_out)
            break;
    }
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

void TimeVirtualISRBegin(void)
{
    Timer1msISR(TimerRoutineCallback);
}

void TimeVirtualISRAdd(int id, void (*user_callback)(void), uint32_t period)
{
    g_timer_ignore = id; //if a timer interrupt is triggered while this routine is executing, g_timer_ignore protects program to crash
    /////safe area////
    time_virtual_stack[id].count_back_time = period;
    time_virtual_stack[id].user_callback = user_callback;
    g_timer_ignore = -1;
}