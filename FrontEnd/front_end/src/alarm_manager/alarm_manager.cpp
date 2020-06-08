
#include "alarm_manager.h"


//ALARMS *alarms_struct;
//Alarm_state *AS;

/*
*   this function should be call in the set_up of the main sketch   
*   this function gives the initial values to the struct that carries the
*   alarm states information
*/







void init_alarm_maganement(void) 
{
    AS.HighAlarmState=false;
    AS.MediumAlarmState=false;
    AS.BuzzerOn=false;
    AS.ScreenSoundOff=false;
}

/*
*   this is a loop function that should be call in the main loop of the sketch
*   this function contains other functions that will allow a hardware action acording to the 
*   priority of the alarms
*/
void alarm_management_loop(void)
{

    check_state_alarm();
    alarm_action();

}

/*
*   this function checks if any high priority alarm is on, and update the alarm states information struct
*
*/
void check_state_alarm(void)
{
    if(alarms_struct.low_in_pressure || alarms_struct.high_in_pressure || alarms_struct.low_out_volume_tidal || alarms_struct.high_out_volume_tidal || alarms_struct.high_volume_leakage || 
        alarms_struct.system_shutdown || alarms_struct.low_battery || alarms_struct.no_battery || alarms_struct.high_temp_motor || alarms_struct.high_temp_bat|| alarms_struct.detached_oxygen_tube || alarms_struct.detached_proximal_tube == true)
    {
        AS.HighAlarmState=1;     
    }
    else
    {
        AS.HighAlarmState=0;     
    }
    
    if(alarms_struct.apnea_alarm || alarms_struct.high_breathing_rate || alarms_struct.low_breathing_rate || alarms_struct.high_in_volume_tidal || alarms_struct.no_main_supply || alarms_struct.low_peep==true)
    {
        AS.MediumAlarmState=1;     
    }
    else
    {
        AS.MediumAlarmState=0;     
    }
    
       
}

/*
*   this function depending of the priority alarm states, takes action calling other 
*   functions to activate or desactivate the peripherals of the front end hardware
*/
void alarm_action(void)
{
            //////////// HIGH PRIORITY ALARMS//////////
        if(AS.HighAlarmState == true)
        {
            REDLed(true);
            if(AS.ScreenSoundOff)
            {
                BUZZER(false);
                AS.BuzzerOn=false;
            }
            else
            {
                BUZZER(true);
                AS.BuzzerOn=true;
            }
            
        }
        else
        {
            REDLed(false);
            BUZZER(false);
            AS.BuzzerOn=false;
        }
        /////////////MEDIUM PRIORITY ALARMS////////////
        if(AS.MediumAlarmState==true)
        {
            YELLOWLed(true);
        }
        else
        {
            YELLOWLed(false);
        }

}