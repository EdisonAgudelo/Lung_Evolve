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

//definitions for sofware I2C
#define I2C_MAXWAIT 2
#define I2C_TIMEOUT 1
#define I2C_PULLUP 1
#define I2C_FASTMODE 1

#define SDA_PORT PORTD //r
#define SDA_PIN 7      //r
#define SCL_PORT PORTD //r
#define SCL_PIN 6      //r

#include "SoftI2CMaster.h"
#include "hardware_interface.h"
//#include "Wire.h"
#include "I2C.h"

//library version for arduino mega

//read data from a I2C slave device
bool I2CRead(int id, uint8_t addres, uint8_t *buffer, uint8_t lenght)
{
  int i = 0;
  uint16_t time_out = 0;

  switch (id)
  {
  case kSoftI2C:

    if (!i2c_start_wait((addres << 1) | I2C_READ))
    {
      i2c_stop();
      return false;
    }

    while (0 != lenght--)
    {

      buffer[i++] = i2c_read(0 == lenght);
    }
    i2c_stop();
    break;

  case kHardI2C:
    /*
    // request data to a slave
    Wire.requestFrom(addres, lenght);

    do
    {
      time_out++;
      while (Wire.available())
      {
        time_out = 0;
        //for protection purpose
        if (i >= lenght)
          break;
        buffer[i++] = Wire.read();
      }
    } while (i < lenght && time_out < I2C_TIMEOUT * 10);

    if (time_out >= I2C_TIMEOUT * 10)
      return false;
*/
    return I2c.read(addres, lenght, buffer) == 0;

    break;

  default:
    return false;
    break;
  }

  return true;
}

//write data in I2C slave divice.
bool I2CWrite(int id, uint8_t addres, uint8_t *buffer, uint8_t lenght)
{
  int i = 0;
  switch (id)
  {
  case kSoftI2C:

    if (!i2c_start_wait((addres << 1) | I2C_WRITE))
    {
      i2c_stop();
      return false;
    }
    while (0 != lenght--)
    {
      if (!i2c_write(buffer[i++]))
        break;
    }
    i2c_stop();

    //if it is a no complete transaction
    if (lenght != 0xff)
      return false;
    break;

  case kHardI2C:

    if (lenght < 2)
    {
      return (I2c.write(addres, buffer[0])) == 0;
    }
    else
    {
      return (I2c.write(addres, buffer[0], buffer + 1, lenght - 1)) == 0;
    }

    /*
    Wire.beginTransmission(addres);
    i = Wire.write(buffer, lenght);
    Wire.endTransmission();

    //if it is a no complete transaction
    if (i != lenght)
      return false;
  */

    break;

  default:
    return false;
    break;
  }
  return true;
}

bool I2CBegin(int id)
{
  switch (id)
  {
  case kHardI2C:

    /*
    Wire.setClock(400000);
    Wire.begin();
	
	*/

    I2c.pullup(true);
    I2c.timeOut(2);
    I2c.setSpeed(true);
    I2c.begin();
    break;
  case kSoftI2C:
    break;
  default:
    return false;
    break;
  }
  return true;
}
