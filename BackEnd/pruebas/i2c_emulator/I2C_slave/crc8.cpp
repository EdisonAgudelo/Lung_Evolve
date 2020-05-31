
#include <stdint.h>

#include "crc8.h"
/*
this function calculates crc 8 for a given data.  
*/

uint8_t g_crc_polynomial=0x31; //default value
uint8_t g_crc_inital = 0xff; //default value


uint8_t CRC8Calculate(uint8_t *data, uint8_t length)
{ 
  uint8_t crc = g_crc_inital;	
  uint8_t index;
  uint8_t bit;
  
  //calculates 8-Bit checksum with given polynomial
  for (index = 0; index < length; ++index)
  {
	crc ^= (data[index]);
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

uint8_t CRC8Calculate(uint16_t data)
{ 
  uint8_t buffer[] = {(uint8_t)(data>>8), (uint8_t)(data&0xff)};
  return CRC8Calculate(buffer, 2);
}

/*
configure crc parameters used to calculate crc value
*/
uint8_t CRC8Configure(uint8_t poly, uint8_t initial)
{
	g_crc_polynomial = poly;
	g_crc_inital = initial;
}