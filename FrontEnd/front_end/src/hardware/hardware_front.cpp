
#include "hardware_front.h"


#include <Arduino.h>


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
    //TCCR1B = TCCR1B & B11111000 | B00000101;

    DriverLedInit(&RED_led, Rpin);
    DriverLedInit(&YELLOW_led, Ypin);

    
}

/*
*   depending of the action that recieves as an argument, this function
*   activates or desactivates the blinking of the led for the medium priority alarms
*
*/

void YELLOWLed(bool action)
{
    uint32_t blink_period=2000;
    if(action)
    {
        //digitalWrite(Ypin,HIGH);
        //analogWrite(Ypin, DUTY_LEDY);
        DriverLedBlink(&YELLOW_led, blink_period);
        
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
void REDLed(bool action)
{
     uint32_t blink_period=200;
    if(action)
    {
        //digitalWrite(Rpin,HIGH);
        //analogWrite(Rpin, DUTY_LEDR);
        DriverLedBlink(&RED_led, blink_period);
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