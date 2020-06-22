
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

#include <stdint.h>

#include "crc8.h"


uint8_t g_crc_polynomial=0x31; //default value
uint8_t g_crc_inital = 0xff; //default value


uint8_t CRC8Calculate(uint8_t *dat, uint8_t length)
{ 
  uint8_t crc = g_crc_inital;	
  uint8_t index;
  uint8_t bit;
  
  //calculates 8-Bit checksum with given polynomial
  for (index = 0; index < length; ++index)
  {
	crc ^= (dat[index]);
    for ( bit = 8; bit > 0; --bit)
    {
	if (crc & 0x80) 
		crc = (crc << 1) ^ g_crc_polynomial;
    else 
		crc = (crc << 1);
    }
  }
  return crc;
}

/*
configure crc parameters used to calculate crc value
*/
uint8_t CRC8Configure(uint8_t poly, uint8_t initial)
{
	g_crc_polynomial = poly;
	g_crc_inital = initial;
}