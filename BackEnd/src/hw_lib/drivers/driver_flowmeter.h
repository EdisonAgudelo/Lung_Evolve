

#ifndef _DRIVER_FLOWMETER_H_
#define _DRIVER_FLOWMETER_H_

#include <stdint.h>
#include <stdbool.h>

#define FLOWMETER_UPDATE_RATE 10 //ms
#define FLOWMETER_MAX_ERROR_COUNT 3 //admissible consecutive errors 
#define FLOWMETER_SOFT_RESET_TIME 100 //ms


class Flowmeter
{

private:
    uint8_t i2c_addr; //sensor addr
    uint8_t i2c_id; //it saves which i2c is working with

    uint16_t value_raw; //save the sensor data in raw.
    uint16_t scale_factor; //sensor param
    uint16_t offset_value; //sensor param 

    uint32_t prev_update_time; //for timing application purpose

    bool sensor_available; //indicate when sensor is ready to measure or if it disconnected

    uint8_t read_error_count; //saves how many times library can not acces to sensor data.

    bool sensor_power_up; //this flag indicate when a sensor is reseting or turning on
    uint32_t sensor_reset_time; //saves when sensor started to make a soft reset
    
    bool MakeTransaction(uint16_t command, uint16_t *buffer);

    void SensorGetParam(void); //reset sensor and try to get sensor param

public:
    
    Flowmeter(int8_t _i2c_id, uint8_t _i2c_addr);
    ~Flowmeter();

    //methods
    void Begin(void);
    void Loop(void);

    //getters
    float GetFlow(void);

};



#endif