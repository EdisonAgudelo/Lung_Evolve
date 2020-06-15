
#include "alarm_manager.h"
#include <Arduino.h>


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
    AS.prevstate1=0;
    AS.prevstate2=0;
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
    DriverLedLoop(&RED_led);
    DriverLedLoop(&YELLOW_led);

}

/*
*   this function checks if any high priority alarm is on, and update the alarm states information struct
*
*/
void check_state_alarm(void)
{
    
    if(alarms_struct.low_in_pressure || alarms_struct.high_in_pressure  || alarms_struct.high_volume_leakage || 
        alarms_struct.low_battery || alarms_struct.no_battery || alarms_struct.high_temp_motor || alarms_struct.high_temp_bat|| 
        alarms_struct.detached_oxygen_tube || alarms_struct.detached_proximal_tube || alarms_struct.high_out_pressure || alarms_struct.low_out_pressure == true)
    {
        AS.HighAlarmState=1;  
        if(AS.prevstate1==0)   
        {
            AS.transition1=true;
            AS.prevstate1=1;
            Serial.print("transision");
        }
        else
        {
            AS.transition1=false;
        }
        
        //Serial.print("high");
    }
    else
    {
        AS.HighAlarmState=0;    
        AS.prevstate1=0; 
    }
    
    if(alarms_struct.apnea_alarm || alarms_struct.high_breathing_rate || alarms_struct.low_breathing_rate || alarms_struct.high_in_volume_tidal || alarms_struct.low_in_volume_tidal || alarms_struct.near_low_in_volume_tidal ||
     alarms_struct.no_main_supply || alarms_struct.low_peep || alarms_struct.high_ie_ratio || alarms_struct. low_ie_ratio==true)
    {
        AS.MediumAlarmState=1; 
        if(AS.prevstate2==0)   
        {
            AS.transition2=true;
            AS.prevstate2=1;
        }    
        else
        {
            AS.transition2=false;
        }
        
    }
    else
    {
        AS.MediumAlarmState=0;     
        AS.prevstate2=0;
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
            //Serial.print("RLED");
            REDLed(true,AS.transition1);
            //digitalWrite(10,HIGH);
            
            if(AS.ScreenSoundOff)
            {
                //Serial.print(" off ");
                BUZZER(false);
                AS.BuzzerOn=false;
            }
            else
            {
                //Serial.print("ON");
                BUZZER(true);
                AS.BuzzerOn=true;
            }
            
        }
        else
        {
            REDLed(false,AS.transition1);
            //digitalWrite(10,LOW);
            BUZZER(false);
            AS.BuzzerOn=false;
        }
        /////////////MEDIUM PRIORITY ALARMS////////////
        if(AS.MediumAlarmState==true)
        {
            YELLOWLed(true,AS.transition2);
            //digitalWrite(9,HIGH);
        }
        else
        {
            YELLOWLed(false,AS.transition2);
            //digitalWrite(9,LOW);;
        }
        

}