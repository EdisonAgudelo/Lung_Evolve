
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


#include "alarm_manager.h"
#include <Arduino.h>



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
            //Serial.print("transision");
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