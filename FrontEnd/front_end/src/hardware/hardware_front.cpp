
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

#include "hardware_front.h"


#include <Arduino.h>


//frontend peripheral pins
#define Ypin 9 
#define Rpin 11
#define BUZZERpin 10
#define RELEpin 7



//duty cycle front end peripherals 

const int DUTY_BUZZER= 128;


DriverLed RED_led;
DriverLed YELLOW_led;


/*
*   this function should be call in the set_up of the main sketch   
*   this function configures the peripherals as an input or output
*   and initialize the objects to use the driver_led.h library
*/
void init_hardware_front(void)
{
    pinMode(Ypin,OUTPUT);
    pinMode(Rpin,OUTPUT);
    pinMode(BUZZERpin,OUTPUT);
    pinMode(RELEpin,OUTPUT);
    //TCCR1B = TCCR1B & B11111000 | B00000101;

    DriverLedInit(&RED_led, Rpin);
    DriverLedInit(&YELLOW_led, Ypin);

    RELE(false);

    //DriverLedBlink(&RED_led,1000);

    
}

/*
*   depending of the action that recieves as an argument, this function
*   activates or desactivates the blinking of the led for the medium priority alarms
*
*/

void YELLOWLed(bool action,bool transition)
{
    uint32_t blink_period=200;
    if(action)
    {
        if(transition)
        {
            DriverLedBlink(&YELLOW_led, blink_period);
            
        }
        else
        {
            
        }
        
        
    }
    else
    {
        //digitalWrite(Ypin,LOW);
        DriverLedTurnOff(&YELLOW_led);
            
    }
    
}

/*
*   depending of the action that recieves as an argument, this function
*   activates or desactivates the blinking of the led for the high priority alarms
*
*/
void REDLed(bool action, bool transition)
{
     uint32_t blink_period=200;
    if(action)
    {
        //digitalWrite(Rpin,HIGH);
        //analogWrite(Rpin, DUTY_LEDR);
        if(transition)
        {
            DriverLedBlink(&RED_led, blink_period);
            
        }
        else
        {
            
        }
                       
        
    }
    else
    {
        //digitalWrite(Rpin,LOW);
        DriverLedTurnOff(&RED_led);
    
    }
    
    
}

/*
*   depending of the action that recieves as an argument, this function
*   activates or desactivates the sound alarm for the high priority alarms
*
*/

void BUZZER(bool action)
{
    if(action)
    {
        //digitalWrite(Rpin,HIGH);
        analogWrite(BUZZERpin, DUTY_BUZZER);
    }
    else
    {
        analogWrite(BUZZERpin, 0);
    
    }
}

void RELE(bool action)
{
    if(action) //action means to power off if it is true
    {
        digitalWrite(RELEpin,HIGH);
    }
    else
    {
        digitalWrite(RELEpin,LOW);
    }
    
}