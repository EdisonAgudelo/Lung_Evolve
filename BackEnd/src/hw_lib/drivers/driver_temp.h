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