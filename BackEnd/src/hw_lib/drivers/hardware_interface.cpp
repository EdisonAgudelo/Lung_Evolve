

#include "hardware_interface.h"


/*
  timer 0 and 5 is for flow sensor count
  timer 1 woriking for millis and micros aplication
  Timer 2 , 3, 4 is for custom pwm genarator
*/

void PinConfigRisingIRS(int pin, void (*callback)(void))
{
  //attachInterrupt(digitalPinToInterrupt(pin), callback, RISING);
}


void Timer1msISR(void (*callback)(void))
{

}
uint16_t Timer1msCount(void)
{

}

void PinConfigDigital(int pin, bool dir)
{

}

void PinConfigAnalog(int pin)
{


}


bool PinReadDigital(int pin)
{
    return true;
}


uint16_t PinReadAnalog(int pin)
{
    return 0;
}


void PinSetDigital(int pin, bool level)
{

}


void PWMConfigFrecuency(int frecuency, int pwm_id)
{

}

bool PWMIsButtom(int pwm_id)
{
    
}

