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

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdint.h>

typedef struct
{
    union {
        struct
        {
            bool p : 1;
            bool i : 1;
            bool d : 1;
        };
        uint8_t all:3;
    } en_control_type, en_control_saturation;

    struct
    {
        float integer;
        float previous_value;
    } energy;

    struct
    {
        float kp;
        float ki;
        float kd;
    } constants;

    struct
    {
        float i;
        float d;
        float p;
    } max;

    bool take_time : 1;
    uint32_t previous_time;
    float dt; //s
    float offset;
    
} ControlData;

//if any constant is set negative, this automatically disable asociated control.
//if dt is set to negative or 0, library get time by itsefl
void ControlInit(ControlData *pObj, float kp, float ki, float kd, float dt);

//extra parameters to has a better control on library
void ControlSetAditional(ControlData *pObj, float max_p, float max_i, float max_d, float offset);

//execute periodically
float ControlExecute(ControlData *pObj, float error);

//reset energy storage varibales
void ControlWindup(ControlData *pObj);
#endif