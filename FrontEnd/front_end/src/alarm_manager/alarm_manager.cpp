
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
    if(alarms_struct.LowInspP || alarms_struct.HighInspP || alarms_struct.VteNotAchived || alarms_struct.VteOv || alarms_struct.PatientLeaks || 
        alarms_struct.ShutDown || alarms_struct.LowBattery || alarms_struct.NoBattery | alarms_struct.HighTemp | alarms_struct.NoOxygen == true)
    {
        AS.HighAlarmState=1;     
    }
    else
    {
        AS.HighAlarmState=0;     
    }
    
    if(alarms_struct.ApneaAlarm || alarms_struct.HighBreathRate || alarms_struct.LowVte || alarms_struct.HighVti || alarms_struct.BackUpOn || alarms_struct.UnderPeep==true)
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