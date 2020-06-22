 
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
// ------- conventions --------//


const uint8_t kSoftI2C = 1;
const uint8_t kHardI2C = 0;


//--------- Functions interface --------//

//i2c functions
bool I2CBegin(int id);
bool I2CRead(int id, uint8_t addres, uint8_t *buffer, uint8_t lenght);
bool I2CWrite(int id, uint8_t addres, uint8_t *buffer, uint8_t lenght);


#endif