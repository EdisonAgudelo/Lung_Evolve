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

#include "driver_voltage.h"
#include "../low_level/hardware_interface.h"
#include "../time.h"

Voltage::Voltage(int _analog_pin, float _attenuation) : analog_pin(_analog_pin), attenuation(_attenuation)
{
    value_raw = 0;

    alarm_enable = false;
    float max_value = -1;
    float min_value = -1;
    alarm = false;
}
Voltage::~Voltage()
{
}

void Voltage::Begin(void)
{
    PinConfigAnalog(analog_pin);
    update_ref_time = Millis();
}

void Voltage::SetAlarm(float _max_value, float _min_value)
{
    if(_max_value != _min_value){
        max_value = _max_value;
        min_value=_min_value;
        alarm_enable = true;
    }
    else
    {
        alarm_enable = false;
    }
    
}

bool Voltage::GetAlarm(void)
{
    return alarm;
}

float Voltage::GetVoltage(void)
{
    return voltage;
}

void Voltage::Loop()
{
    int i;
    if(GetDiffTime(Millis(), update_ref_time)>VOLTAGE_UPDATE_PERIOD){

        update_ref_time=Millis();

        value_raw = 0;
        for(i=0;i<VOLTAGE_OVERSAMPLING; i++){
           value_raw += PinReadAnalog(analog_pin);
        }
        value_raw /= i;   

        voltage = (((float)value_raw)*kBoardVoltage/(float)kACDResolution)*attenuation;

        if(alarm_enable && (voltage>max_value || voltage < min_value))
            alarm = true;
        else
            alarm = false;
    }
}

