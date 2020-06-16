
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
   
#ifndef _DRIVER_W_FLOWMETER_H_
#define _DRIVER_W_FLOWMETER_H_

#include <stdint.h>
#include <stdbool.h>

#define W_FLOWMETER_UPDATE_RATE 200 //ms


class WFlowmeter
{

private:
    uint8_t counter_id; //it saves wich timer is working with 
    uint32_t prev_update_time; //for time aplication purpose

    uint32_t flow_counts; //all saved counts
    uint32_t flow_rate_raw; //instant value

    float counts_per_slm; //counts per standar liter per minute

public:
    
    WFlowmeter(int8_t _counter_id, float _counts_per_slm);
    ~WFlowmeter();

    void Begin(void);
    //methods
    void Loop(void);

    //getters
    float GetFlow(void);
    float GetVolume(bool reset = false);

};



#endif