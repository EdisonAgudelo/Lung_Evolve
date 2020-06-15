 

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
const int kHardwareBuzzerPin = 3; // review

//driver enable pin
const int kHardwareEnMotor1 = 13;  // r
const int kHardwareEnMotor2 = 43;  // r
const int kHardwareEnMotor3 = 8;  // r
 
//driver step or pulse pin
const int kHardwareStepMotor1 = 12;
const int kHardwareStepMotor2 = 45;
const int kHardwareStepMotor3 = 7;
const int kHardwareWFlowPin = 38;

//driver dir pin
const int kHardwareDirMotor1 = 11; // r
const int kHardwareDirMotor2 = 47; // r
const int kHardwareDirMotor3 = 6; // r

//final switch pins
const int kHardwareSwitchFMotor1 = 2; // r
const int kHardwareSwitchBMotor1 = 3; // r
const int kHardwareSwitchFMotor2 = -1; // r
const int kHardwareSwitchBMotor2 = 18; // r
const int kHardwareSwitchFMotor3 = -1; // r
const int kHardwareSwitchBMotor3 = 19; // r

//driver flow pins

//rele pins
const int kHardwareRele1 = 27; // r
const int kHardwareRele2 = 29; // r
const int kHardwareRele3 = 31; // r
const int kHardwareRele4 = 33; // r

//Driver diff pressure
const int kHardwareDiffPressure1 = A0; // r
const int kHardwareDiffPressure2 = A1; // r

//Temperature sensor 
const int kHardwareTemp1 = A2; //r
const int kHardwareTemp2 = A3; //r

//Voltage sensor
const int kHardwareVoltage1 = A8; //r
const int kHardwareVoltage2 = A9; //r


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