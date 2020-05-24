

#include "hardware_interface.h"

#include <Arduino.h>




void PinConfigRisingIRS(int pin, void (*callback)(void))
{
  //attachInterrupt(digitalPinToInterrupt(pin), callback, RISING);
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

