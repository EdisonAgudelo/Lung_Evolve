 

#ifndef _HARDWARE_INTERFACE_H_
#define _HARDWARE_INTERFACE_H_

//this library is used to create an interface with mcu periphericals. if everything work properly,  you can change MCU and modify 
// only this library to make compatible with your mcu and all other firmware elements shuld work well.

#include <stdint.h>

//pin definitions
const int kHardwareLedRedPin = 12; //this constant is used to config led driver 
const int kHardwareBuzzerPin = 0;

const bool kInput = true;
const bool kOutput = false;

//this function drives an specific to HIGH level 
void set_pin(int pin);

//this function drives an specific to LOW level
void clear_pin(int pin);

//this function read analog pin
bool read_digital_pin(int pin);

uint16_t read_analog_pin(int pin);

//this functions do requiered pin config to work properly
void pin_config_digital(int pin, bool dir);
void pin_config_analog(int pin);



#endif