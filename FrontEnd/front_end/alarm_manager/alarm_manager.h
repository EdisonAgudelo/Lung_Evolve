#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H


#define TIME_ON_BUZZER 2000


typedef struct 
{
    bool HighAlarmState;
    bool MediumAlarmState;
    bool BuzzerOn;
    bool ScreenSoundOff;
} Alarm_state;

extern ALARMS alarms_struct; 
unsigned long start_counter;

void init_alarm_maganement(void);
void alarm_management_loop(void);
void check_state_alarm(void);
void alarm_action(void);


#endif /*ALARM_MANAGER_H*/