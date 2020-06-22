 
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
   
#ifndef _HARDWARE_INTERFACE_H_
#define _HARDWARE_INTERFACE_H_

//this library is used to create an interface with mcu periphericals. if everything work properly,  you can change MCU and modify 
// only this library to make compatible with your mcu and all other firmware elements shuld work well.

#include <stdint.h>
#include <Arduino.h>

//---------- counter Id -------//
const int kHardwareCounterFlow1=0;

//------------pwm id ------------//

const int kHardwarePWMMotor1 = 1;
const int kHardwarePWMMotor2 = 5;
const int kHardwarePWMMotor3 = 4;


//---------pin definitions---------//


//these pins will no have any effect if you change it, only is 
//for user information
const int kHardwareFrontComTx = 16; //r
const int kHardwareFrontComRx = 17; //r

const int kHardwarePCComTx = 1; //r
const int kHardwarePCComRx = 0; //r

const int kHardwareI2C1SDAPin = 20; //r
const int kHardwareI2C1SCLPin = 21; //r

const int kHardwareI2C2SDAPin = 23; //r
const int kHardwareI2C2SCLPin = 25; //r


//indicator 
const int kHardwareLedRedPin = 8; // review
const int kHardwareBuzzerPin = 37; // review

//driver enable pin
const int kHardwareEnMotor1 = 11;  // r //x
const int kHardwareEnMotor2 = 47;  // r //x
const int kHardwareEnMotor3 = 5;  // r //x
 
//driver step or pulse pin
const int kHardwareStepMotor1 = 12; //x
const int kHardwareStepMotor2 = 45; //x
const int kHardwareStepMotor3 = 7; //x
const int kHardwareWFlowPin = 38;

//driver dir pin
const int kHardwareDirMotor1 = 13; // r //x
const int kHardwareDirMotor2 = 43; // r //x
const int kHardwareDirMotor3 = 6; // r //x

//final switch pins
const int kHardwareSwitchFMotor1 = 2; // r 
const int kHardwareSwitchBMotor1 = 3; // r
const int kHardwareSwitchFMotor2 = -1; // r
const int kHardwareSwitchBMotor2 = 19; // r
const int kHardwareSwitchFMotor3 = -1; // r
const int kHardwareSwitchBMotor3 = 18; // r

//driver flow pins

//rele pins
const int kHardwareRele1 = 29; // r //x
const int kHardwareRele2 = 33; // r //x
const int kHardwareRele3 = 31; // r //x
const int kHardwareRele4 = 27; // r //x

//Driver diff pressure
const int kHardwareDiffPressure1 = A1; // r
const int kHardwareDiffPressure2 = A0; // r

//Temperature sensor 
const int kHardwareTemp1 = A2; //r
const int kHardwareTemp2 = A3; //r

//Voltage sensor
const int kHardwareVoltage1 = A9; //r
const int kHardwareVoltage2 = A8; //r


// ------- conventions --------//

const bool kInput = true;
const bool kOutput = false;
const bool kHigh = true;
const bool kLow = false;

const uint8_t kSoftI2C = 1;
const uint8_t kHardI2C = 0;

// --------- HW parameter ---------//

const uint16_t kACDResolution = 0x3ff;
const float kBoardVoltage = 5.0;
const uint32_t kDebugBaudRate = 115200;

//--------- Functions interface --------//

//i2c functions
bool I2CBegin(int id);
bool I2CRead(int id, uint8_t addres, uint8_t *buffer, uint8_t lenght);
bool I2CWrite(int id, uint8_t addres, uint8_t *buffer, uint8_t lenght);




//Timer/counter/PWM functions functions
double PWMConfigFrecuency(uint32_t frecuency, int pwm_id);
void CounterBegin(int counter_id);

//return true if PWM has a pending interrupt
bool PWMIsPendingInterrupt(int pwm_id);

//set a flag which pwm should clear;
bool PWMRequestInterrupt(int pwm_id);

//this configure one timer to has 1ms interrupt period
void Timer1msISR(void (*callback)(void));

//this return de actual count of timer who is generating 1ms interrupt
uint16_t Timer1msCount(void);

//this function return the actual value of a counter
uint32_t CounterGetValue(int counter_id);



//GPIO functions
void PinConfigRisingIRS(int pin, void (*callback)(void));
void PinConfigDigital(int pin, bool dir);
void PinConfigAnalog(int pin);
bool PinReadDigital(int pin);
uint16_t PinReadAnalog(int pin);
void PinSetDigital(int pin, bool level);



//Uart Functions
void UartConfigCallback(void (*callback)(void));
void UartBegin(uint32_t baudrate);
bool UartAvailable(void);
void UartWrite(uint8_t data);
uint8_t UartRead(void);

void DebugBegin(void);
void DebugWrite(char *buffer, uint16_t length);


#endif