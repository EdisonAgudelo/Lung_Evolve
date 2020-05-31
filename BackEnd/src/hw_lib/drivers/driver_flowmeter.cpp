

#include "driver_flowmeter.h"

#include "hardware_interface.h"

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
    //this library suppotr instance deletion but it is no recommended
}

//methods

void Flowmeter::Begin(void)
{
    //CounterBegin(counter_id);
    prev_update_time = Millis();
    sensor_reset_time = Millis();

    sensor_power_up = true; //let the sensor to power up
    sensor_available = false;

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
            uDelay(1000);
        }

        any_use_count = 0;

        while (sensor_available && !MakeTransaction(kFlowCommandReadOffset, &offset_value))
        {
            any_use_count++;
            if (any_use_count >= 3)
            {
                sensor_available = false;
                break;
            }
            uDelay(1000);
        }
    }
}

bool Flowmeter::MakeTransaction(uint16_t command, uint16_t *buffer)
{
    uint8_t cmd[2] = {command >> 8, command & 0xff};
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

            if (FLOWMETER_MAX_ERROR_COUNT <= read_error_count)
            {
                sensor_available = false;
                read_error_count = 0;
            }
        }
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
    if (sensor_available)
        return ((float)(value_raw>>2) - (float)offset_value) / ((float)scale_factor);
    else
        return 0.0;
}
