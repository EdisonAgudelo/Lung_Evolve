#ifndef _DRIVER_VOLTAGE_H_
#define _DRIVER_VOLTAGE_H_


#define VOLTAGE_UPDATE_PERIOD 1000 //update time each 2 minutes
#define VOLTAGE_OVERSAMPLING 10 //total average data

#include <stdint.h>

class Voltage {
    private:
        int analog_pin;
        uint32_t update_ref_time;
        uint32_t value_raw;

        bool alarm_enable;
        float max_value;
        float min_value;
        float voltage;

        float attenuation;

        bool alarm;
    public:

        Voltage(int _analog_pin, float _attenuation);
        ~Voltage();

        void Begin(void);

        void SetAlarm(float _max_value, float _min_value);

        bool GetAlarm(void);
        float GetVoltage(void);

        void Loop();

};





#endif 