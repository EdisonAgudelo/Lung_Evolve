 

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


//---------pin definitions---------//
const int kHardwareLedRedPin = 8; // review
const int kHardwareBuzzerPin = 3; // review

//driver enable pin
const int kHardwareEnMotor1 = 4;  // review

//driver step or pulse pin
const int kHardwareStepMotor1 = 12;
const int kHardwareStepMotor2 = 2;
const int kHardwareStepMotor3 = 7;

//driver dir pin
const int kHardwareDirMotor1 = 5; // review
const int kHardwareDirMotor2 = 6; // review
const int kHardwareDirMotor3 = 9; // review

//final switch pins
const int kHardwareSwitchFMotor1 = 19; // review
const int kHardwareSwitchBMotor1 = -1; // review
const int kHardwareSwitchFMotor2 = -1; // review
const int kHardwareSwitchBMotor2 = -1; // review
const int kHardwareSwitchFMotor3 = -1; // review
const int kHardwareSwitchBMotor3 = -1; // review

//driver flow pins

//rele pins
const int kHardwareRele1 = 10; // review
const int kHardwareRele2 = 11; // review
const int kHardwareRele3 = 12; // review
const int kHardwareRele4 = -1; // review

//Driver diff pressure
const int kHardwareDiffPressure1 = A8; // review
const int kHardwareDiffPressure2 = A9; // review

//Temperature sensor 
const int kHardwareTemp1 = -1; //review
const int kHardwareTemp2 = -1; //review

//Voltage sensor
const int kHardwareVoltage1 = -1; //review
const int kHardwareVoltage2 = -1; //review


// ------- conventions --------//

const bool kInput = true;
const bool kOutput = false;
const bool kHigh = true;
const bool kLow = false;

const uint8_t kSoftI2C = 1;
const uint8_t kHardI2C = 0;

// --------- HW parameter ---------//

const uint16_t kACDResolution = 0x3ff;


//--------- Functions interface --------//

bool I2CBegin(int id);
bool I2CRead(int id, uint8_t addres, uint8_t *buffer, uint8_t lenght);
bool I2CWrite(int id, uint8_t addres, uint8_t *buffer, uint8_t lenght);


//configuration functions 
uint32_t PWMConfigFrecuency(uint32_t frecuency, int pwm_id);

void PinConfigRisingIRS(int pin, void (*callback)(void));

void PinConfigDigital(int pin, bool dir);

void PinConfigAnalog(int pin);

void CounterBegin(int counter_id);

void UartConfigCallback(void (*callback)(void));
void UartBegin(uint32_t baudrate);
bool UartAvailable(void);
void UartWrite(uint8_t data);
uint8_t UartRead(void);


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

//this function return the actual value of a counter
uint32_t CounterGetValue(int counter_id);


#endif