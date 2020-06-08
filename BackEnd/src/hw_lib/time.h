
#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>


//it gets the enlapsed time between two time references. This avoid issues related to uint32_t counter over flow 
uint32_t GetDiffTime(uint32_t actual, uint32_t prev);

//it retuns the enlapsed time from the system power on in milliseconds. It has a maximum values of 49 days before counter reset. 
uint32_t Millis(void);

//it retuns the enlapsed time from the system power on in microseconds. It has a maximum values of 1 hour before counter reset. 
uint32_t Micros(void);

//It stucks main fw flow during a fixed time
void uDelay(uint32_t dalay_microseconds);

//It stucks main fw flow during a fixed time
void Delay(uint32_t delay_milliseconds);

//init timer for generate 1 ms interrupt.
void TimeVirtualISRBegin(void);

//this is a feature that allow accuracy timing for user application
void TimeVirtualISRAdd(int id, void (*user_callback)(void), uint32_t period);

#endif