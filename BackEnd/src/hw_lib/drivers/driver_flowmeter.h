

#ifndef _DRIVER_FLOWMETER_H_
#define _DRIVER_FLOWMETER_H_

#include <stdint.h>
#include <stdbool.h>

#define FLOWMETER_UPDATE_RATE 50 //ms
#define FLOWMETER_MAX_ERROR_COUNT 3
#define FLOWMETER_SOFT_RESET_TIME 100 // ms


class Flowmeter
{

private:
    uint8_t i2c_addr;
    uint8_t i2c_id; //it saves wich timer is working with 
    uint32_t prev_update_time; //for timing application purpose

    uint16_t value_raw; //save the sensor data in raw.
    uint16_t scale_factor;
    uint16_t offset_value;    

    uint32_t prev_update_time_us;
    float volume_raw;

    bool sensor_available;

    uint8_t read_error_count; //saves how many times library can not acces to data.

    bool sensor_power_up; //this flag indicate that a sensor is reseting or turning on
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
    float GetVolume(bool reset = false);

};



#endif