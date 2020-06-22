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


//this library is focused to supose external functions that should
//exist o must be created. This functions are oriented to hardware
//interface and are the basic contructions brinks.

#ifndef _BASIC_FUNCTION_
#define _BASIC_FUNCTION_

#include "drivers/driver_led.h"
#include "low_level/hardware_interface.h"
#include "drivers/driver_flowmeter.h"
//----------Driver varibles----------//
extern DriverLed g_discharge_rele;
extern Flowmeter flow_in;
extern Flowmeter flow_out;

//this function bring a especific motor to a know position
//return 0 is no succes o 1 if action was totally complete
bool DriverMotorMoveTo(int motor_id, float line_pos);

//this function set the linear motor vel 
//return 0 is no succes o 1 if action was totaly complete
bool DriverMotorSetVel(int motor_id, float motor_vel);

//this function stop motor if it is moving
void DriverMotorStop(int motor_id);

//check if motor is full stop
bool DriverMotorIsStop(int motor_id);

//Set zero pos on drivers
void DriverMotorSetZeroPos(int motor_id);

//this function initialize all develpment board GPIO wich are not initialized by drivers
//return 0 if there are some error else return 1
bool PinInitialization(void);

//this function manage all on/off valves
//return 0 if there are some error else return 1
bool DriverValveOpenTo(int valve_id, bool valve_position);

//return filtered sensor values
float SensorGetValue(int sensor_id);

//this function returns true if sensor is out of safe operation area
bool SensorGetAlarm(int sensor_id);

//return actual motor position
float DriverMotorActualPos(int motor_id);

//this function initialize all peripheral drivers
//return 0 if there are some error else return 1
bool DirverInitialization(void);

//this function execute driver functions wich need to be execute frecuently
bool DriverLoops(void);

#endif
