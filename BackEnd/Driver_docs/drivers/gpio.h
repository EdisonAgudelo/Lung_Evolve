#ifndef GPIO_H
#define GPIO_H

#include <Arduino.h>
#include <stdint.h>

/***************************/
//ENDSTOP SWITCHES PIN CONFIGURATION
#define ENDSTOP1 A2
#define ENDSTOP2 9
#define ENDSTOP3 8

void set_pin(uint8_t pin);
void clear_pin(uint8_t pin);
bool read_digital_pin(int pin);

#endif /*GPIO_H*/
