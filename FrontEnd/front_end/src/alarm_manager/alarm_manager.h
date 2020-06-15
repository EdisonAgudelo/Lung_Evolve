#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H

#include "../data_bus/data_bus.h"
#include "../hardware/hardware_front.h"




typedef struct 
{
    bool HighAlarmState;
    bool MediumAlarmState;
    bool BuzzerOn;
    bool ScreenSoundOff;
    bool transition1;
    bool transition2;
    int prevstate1;
    int prevstate2;
} Alarm_state;

volatile extern Alarm_state AS;

//unsigned long start_counter;

void init_alarm_maganement(void) ;
void alarm_management_loop(void);
void check_state_alarm(void);
void alarm_action(void);


#endif /*ALARM_MANAGER_H*/