#ifndef GPIO_H
#define GPIO_H

#include <Arduino.h>


/***************************/
//ENDSTOP SWITCHES PIN CONFIGURATION
#define ENDSTOP1 A2
#define ENDSTOP2 9
#define ENDSTOP3 8


void set_GPIO_High(uint8_t PIN);
void set_GPIO_Low(uint8_t PIN);
bool state_GPIO(uint8_t PIN);



#endif /*GPIO_H*/
