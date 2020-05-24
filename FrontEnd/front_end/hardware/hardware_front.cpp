
//inicializar pines
/*
    leds
    buzzer
    serial pantalla
    serial backend
*/
#include "hardware_front.h"

void init_hardware_front(void)
{
    pinMode(Ypin,1);
    pinMode(Rpin,1);
    pinMode(BUZZERpin,1);

    
}


void YELLOWLed(bool action)
{
    if(action)
    {
        digitalWrite(Ypin,HIGH);
    }
    else
    {
        digitalWrite(Ypin,LOW);
    
    }
    
}

void REDLed(bool action)
{
    if(action)
    {
        digitalWrite(Rpin,HIGH);
    }
    else
    {
        digitalWrite(Rpin,LOW);
    
    }
    
}

void BUZZER(bool action)
{
    if(action)
    {
        digitalWrite(Rpin,HIGH);
    }
    else
    {
        digitalWrite(Rpin,LOW);
    
    }
}