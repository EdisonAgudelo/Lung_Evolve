


#include "time.h"
#include "drivers/hardware_interface.h"

static void TimerRoutineCallback(void);

#define TIME_SISTICK_PERIOD 250  //indicate the maximun value of timer/couter to reach 1ms for actual clk input

#define TIME_MAX_VIRTUAL_ISR 4  //this is to hold good performance

uint32_t g_milliseconds = 0;
int8_t g_timer_ignore=-1;

struct TimeStack
{
    void (*user_callback)(void);
    uint32_t count_back_time;

} time_virtual_stack[TIME_MAX_VIRTUAL_ISR];



uint32_t GetDiffTime(uint32_t actual, uint32_t prev)
{
    if(actual>=prev){
        return actual - prev;
    } else{ //when resgiter overflow
        return 0xffffffff - (prev - actual);
    }
        
}

static void TimerRoutineCallback(void)
{
    uint8_t i;
    g_milliseconds++;

    for(i=0;i<TIME_MAX_VIRTUAL_ISR;i++){
        if(time_virtual_stack[i].user_callback!=nullptr && g_timer_ignore!=i){
            time_virtual_stack[i].count_back_time--;
            if(0==time_virtual_stack[i].count_back_time){
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
    uint16_t Core_timer=Timer1msCount();
    return (g_milliseconds*1000 + (Core_timer*1000)/TIME_SISTICK_PERIOD);
}



void Delay(uint32_t delay_milliseconds)
{
    uint32_t time=Millis();
    while(GetDiffTime(Millis(),time)<delay_milliseconds);
}

void uDelay(uint32_t dalay_microseconds)
{
    uint32_t time=Micros();
    while(GetDiffTime(Micros(),time)<dalay_microseconds);
}

void TimeVirtualISRBegin(void)
{
    Timer1msISR(TimerRoutineCallback);
}

void TimeVirtualISRAdd(int id, void (*user_callback)(void), uint32_t period)
{
    g_timer_ignore = id;   //if a timer interrupt is triggered while this routine is executing, g_timer_ignore protects program to crash
    /////safe area//// 
    time_virtual_stack[id].count_back_time=period;
    time_virtual_stack[id].user_callback = user_callback;
    g_timer_ignore = -1;
}


