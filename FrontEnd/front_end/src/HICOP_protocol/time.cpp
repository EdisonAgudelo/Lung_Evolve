

#include "time.h"
#include "../hardware/hardware_interface.h"

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
