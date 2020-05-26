 

#ifndef _HARDWARE_INTERFACE_H_
#define _HARDWARE_INTERFACE_H_

//this library is used to create an interface with mcu periphericals. if everything work properly,  you can change MCU and modify 
// only this library to make compatible with your mcu and all other firmware elements shuld work well.

#include <stdint.h>
#include <Arduino.h>

//------------pwm id ------------//
const int kHardwarePWMMotor1 = 1;
const int kHardwarePWMMotor2 = 3;
const int kHardwarePWMMotor3 = 4;

const int kHardwareCounterFlow1 = 0; 
const int kHardwareCounterFlow2 = 5;

//---------pin definitions---------//
const int kHardwareLedRedPin = 12; // review
const int kHardwareBuzzerPin = 0; // review

//driver enable pin
const int kHardwareEnMotor1 = 1;  // review

//driver step or pulse pin
const int kHardwareStepMotor1 = 12;
const int kHardwareStepMotor2 = 2;
const int kHardwareStepMotor3 = 7;

//driver dir pin
const int kHardwareDirMotor1 = 1; // review
const int kHardwareDirMotor2 = 3; // review
const int kHardwareDirMotor3 = 4; // review

//final switch pins
const int kHardwareSwitchFMotor1 = -1; // review
const int kHardwareSwitchBMotor1 = -1; // review
const int kHardwareSwitchFMotor2 = -1; // review
const int kHardwareSwitchBMotor2 = -1; // review
const int kHardwareSwitchFMotor3 = -1; // review
const int kHardwareSwitchBMotor3 = -1; // review

//driver flow pins

//rele pins
const int kHarwareRele1 = -1; // review
const int kHarwareRele2 = -1; // review
const int kHarwareRele3 = -1; // review


// ------- conventions --------//

const bool kInput = true;
const bool kOutput = false;
const bool kHigh = true;
const bool kLow = false;



//configuration functions 
void PWMConfigFrecuency(int frecuency, int pwm_id);

void PinConfigRisingIRS(int pin, void (*callback)(void));

void PinConfigDigital(int pin, bool dir);

void PinConfigAnalog(int pin);


//read digital level of a given pin
bool PinReadDigital(int pin);

//perform adc opration to get a "analog" value
uint16_t PinReadAnalog(int pin);

// drives an specific pint ro HIGH or low
void PinSetDigital(int pin, bool level);

//return true if PWM has a pending interrupt
bool PWMIsPendingInterrupt(int pwm_id);

//set a flag which pwm should clear;
bool PWMRequestInterrupt(int pwm_id);

//this configure one timer to has 1ms interrupt period
void Timer1msISR(void (*callback)(void));

//this return de actual count of timer who is generating 1ms interrupt
uint16_t Timer1msCount(void);

//disable all interrupts
void HardwareDisableISR(void);

//enable all interrupts
void HardwareEnableISR(void);
#endif