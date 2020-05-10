//this library is focused to supose external functions that should
//exist o must be created. This functions are oriented to hardware
//interface and are the basic contructions brinks.

#ifndef _BASIC_FUNCTION_
#define _BASIC_FUNCTION_

#include "driver_led.h"

//----------Driver varibles----------//
extern DriverLed led_red;
extern DriverLed buzzer;

//this function bring a especific motor to a know position
//return 0 is no succes o 1 if action was totaly complete
bool DriverMotorMoveTo(int motor_id, int line_pos);

//this function set the lin vel or rotation vel of motor
//return 0 is no succes o 1 if action was totaly complete
bool DriverMotorSetVel(int motor_id, int motor_vel);

//this function initialize all develpment board GPIO
//return 0 if there are some error else return 1
bool PinInitialization(void);

//this function manage all on/off valves
//return 0 if there are some error else return 1
bool DriverValveOpenTo(int valve_id, bool valve_position);

//return filtered pressure in cmH20*1000 or air flow in L/min*1000
long int SensorGetValue(int sensor_id);

//return actual motor position
int DriverMotorActualPos(int motor_id);

//this function initialize all peripheral drivers
//return 0 if there are some error else return 1
bool DirverInitialization(void);

//this function execute driver functions wich need to be execute frecuently
bool DriverLoops(void);

#endif
