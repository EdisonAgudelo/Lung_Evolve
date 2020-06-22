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

#ifndef _DRIVER_DIFF_PRESSURE_H_
#define _DRIVER_DIFF_PRESSURE_H_

#define DIFFPRESSURE_CONSTANT (10.197162129779/0.057)  // to compute pressure in cmH2O
#define DIFFPRESSURE_OFFSET 0.5 // to compute pressure in cmH2O
#define DIFFPRESSURE_UPDATE_PERIDO 10 //ms

#include <stdint.h>

class DiffPressure
{
    private:

        int analog_pin;
        uint16_t analog_value;

        uint32_t prev_update_time;

    public:

        DiffPressure(int _analog_pin);
        ~DiffPressure();

        void Begin(void);
        void Loop(void);


        //getter
        float GetDiffPressure(void);

};






#endif