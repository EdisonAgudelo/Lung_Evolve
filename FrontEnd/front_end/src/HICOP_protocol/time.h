
#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

uint32_t GetDiffTime(uint32_t actual, uint32_t prev);

uint32_t Millis(void);



void uDelay(uint32_t dalay_microseconds);

void Delay(uint32_t delay_milliseconds);

void TimeVirtualISRBegin(void);

void TimeVirtualISRAdd(int id, void (*user_callback)(void), uint32_t period);

#endif