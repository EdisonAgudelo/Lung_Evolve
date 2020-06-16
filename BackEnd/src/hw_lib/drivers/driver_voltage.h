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

#ifndef _DRIVER_VOLTAGE_H_
#define _DRIVER_VOLTAGE_H_


#define VOLTAGE_UPDATE_PERIOD 1000 //update voltage each second
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