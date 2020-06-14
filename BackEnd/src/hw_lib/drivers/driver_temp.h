
#ifndef _DRIVER_TEMP_H_
#define _DRIVER_TEMP_H_


#define TEMP_UPDATE_PERIOD 30000 //update time each half minute
#define TEMP_TEMP_COEF 0.0195 // V/°C
#define TEMP_OVERSAMPLING 10 //total average data
#define TEMP_TEMP_OFFSET 0.4 // V

#include <stdint.h>

class Temp {
    private:
        int analog_pin;
        uint32_t update_ref_time;
        uint32_t value_raw;

        bool alarm_enable;
        float max_value;
        float min_value;
        float temp;

        bool alarm;
    public:

        Temp(int _analog_pin);
        ~Temp();

        void Begin(void);

        void SetAlarm(float _max_value, float _min_value);

        bool GetAlarm(void);
        float GetTemp(void);

        void Loop();

};





#endif 