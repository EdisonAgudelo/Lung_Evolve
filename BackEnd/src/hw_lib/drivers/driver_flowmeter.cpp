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

#include "driver_flowmeter.h"

#include "../low_level/hardware_interface.h"

#include "../time.h"
#include "../../fw_lib/crc8.h"

//all command are in big endian
const uint16_t kFlowCommandStarFlowMeasure = 0x1000;
const uint16_t kFlowCommandStarTempMeasure = 0x1001;
const uint16_t kFlowCommandReadScaleFactor = 0x30de;
const uint16_t kFlowCommandReadOffset = 0x30df;
const uint16_t kFlowCommandSoftReset = 0x2000;

Flowmeter::Flowmeter(int8_t _i2c_id, uint8_t _i2c_addr) : i2c_id(_i2c_id), i2c_addr(_i2c_addr)
{
}

Flowmeter::~Flowmeter()
{
    //this library support instance deletion but it is no recommended
}

//methods

void Flowmeter::Begin(void)
{
    //CounterBegin(counter_id);
    prev_update_time = Millis();
    sensor_reset_time = Millis();

    sensor_power_up = true; //let the sensor to power up
    sensor_available = false;

    value_raw = 0;

    I2CBegin(i2c_id);

    /*
    SensorGetParam();

    //start measurements
    if (sensor_available)
        MakeTransaction(kFlowCommandStarFlowMeasure, nullptr);*/
}

void Flowmeter::SensorGetParam(void)
{
    int any_use_count;

    if (!sensor_power_up)
    {
        sensor_power_up = true;
        sensor_available = false;
        sensor_reset_time = Millis();
        
        //reset sensor
        MakeTransaction(kFlowCommandSoftReset, nullptr);
    }
    else if (GetDiffTime(Millis(), sensor_reset_time)>=FLOWMETER_SOFT_RESET_TIME)
    {
        
        sensor_power_up = false;

        //initial expected value
        sensor_available = true;

        any_use_count = 0;

        //try to get data 3 times before give up
        while (!MakeTransaction(kFlowCommandReadScaleFactor, &scale_factor))
        {
            any_use_count++;
            if (any_use_count >= 3)
            {
                sensor_available = false;
                break;
            }
           // uDelay(1000);
        }


        any_use_count = 0;

        //if there are no erros from last transaction, make other read
        while (sensor_available && !MakeTransaction(kFlowCommandReadOffset, &offset_value))
        {
            any_use_count++;
            if (any_use_count >= 3)
            {
                sensor_available = false;
                break;
            }
           // uDelay(1000);
        }
    }
}

bool Flowmeter::MakeTransaction(uint16_t command, uint16_t *buffer)
{
    uint8_t cmd[2] = {(uint8_t)(command >> 8), (uint8_t)(command & 0xff)};
    uint8_t response[3];

    //send command if it is avilable
    if (0 != command)
    {
        if (!I2CWrite(i2c_id, i2c_addr, cmd, 2))
            return false;
        else
            uDelay(500); //typical response time
    }

      
    
    //if user is waiting for data
    if (buffer != nullptr)
    {
        if (!I2CRead(i2c_id, i2c_addr, response, 3))
            return false;

        CRC8Configure(0x31, 0x0);

        //Serial.print(response[0]); Serial.print(response[1]);Serial.print(" crc: "); Serial.println(response[2]);

        if (response[2] != CRC8Calculate(response, 2))
            return false;

        *buffer = (((uint16_t)response[0]) << 8) + (uint16_t)response[1];
    }

    return true; //if everything is ok
}

void Flowmeter::Loop(void)
{

    if (GetDiffTime(Millis(), prev_update_time) > FLOWMETER_UPDATE_RATE)
    {
        prev_update_time = Millis();

        if (sensor_available)
        {
            //get a data from sensor streaming
            if (MakeTransaction(0, &value_raw))
                read_error_count = 0;
            else
                read_error_count++;

            //if sensor don't answere 3 consecutive times, enter in recovery mode 
            if (FLOWMETER_MAX_ERROR_COUNT <= read_error_count)
            {
                sensor_available = false;
                read_error_count = 0;
            }
        }
        //recovery mode
        else
        {
            SensorGetParam();

            //start sensor streaming
            if (sensor_available)
                MakeTransaction(kFlowCommandStarFlowMeasure, nullptr);
        }
    }
}

//---------------------- getters ----------------------//

float Flowmeter::GetFlow(void)
{
    //if sensor is available, raw data will be uptaded, else it will hold the last read value.
    if(scale_factor != 0.0)
        return ((float)(value_raw) - (float)offset_value) / ((float)scale_factor);
    else
        return 0.0;
}
