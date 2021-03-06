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

#include "../hw_lib/time.h"

#include "control.h"

//if any constant is set negative, this automatically disable asociated control.
//if dt is set to negative or 0, library get time by itsefl
void ControlInit(ControlData *pObj, float kp, float ki, float kd, float dt)
{
    pObj->constants.kp = kp;
    pObj->constants.ki = ki;
    pObj->constants.kd = kd;

    //disable all saturation
    pObj->en_control_saturation.all = 0b000;
    pObj->en_control_type.all = 0b111;

    if (pObj->constants.kp <= 0.0)
        pObj->en_control_type.p = false;
    if (pObj->constants.ki <= 0.0)
        pObj->en_control_type.i = false;
    if (pObj->constants.kd <= 0.0)
        pObj->en_control_type.d = false;

    if (dt <= 0.0)
    {
        pObj->take_time = true;
        pObj->dt = ((float)Millis()) / 1000.0;
    }
    else
    {
        pObj->take_time = false;
        pObj->dt = dt;
    }

    pObj->energy.integer = 0.0;
    pObj->energy.previous_value = 0.0;
}

//extra parameters to has a better control on library
void ControlSetAditional(ControlData *pObj, float max_p, float max_i, float max_d, float offset)
{
    if (pObj->en_control_type.p)
    {
        pObj->en_control_saturation.p = true;
        pObj->max.p = max_p;
    }
    if (pObj->en_control_type.i)
    {
        pObj->en_control_saturation.i = true;
        pObj->max.i = max_i;
    }
    if (pObj->en_control_type.d)
    {
        pObj->en_control_saturation.d = true;
        pObj->max.d = max_d;
    }
    pObj->offset=offset;
}

//execute periodically
float ControlExecute(ControlData *pObj, float error)
{
    float action_control=0.0;

    if(pObj->take_time)
    {
        pObj->dt=((float)(Millis()-pObj->previous_time))/1000.0;
        pObj->previous_time=Millis();
    }

    if(pObj->en_control_type.p)
    {
        action_control+=error*pObj->constants.kp;

        if(pObj->en_control_saturation.p)
        {
            action_control=(action_control>=pObj->max.p)?pObj->max.p:action_control;
            action_control=(action_control<=-pObj->max.p)?-pObj->max.p:action_control;
        }
    }
    
    if(pObj->en_control_type.i)
    {
        pObj->energy.integer+=error*pObj->dt;
        
        if(pObj->en_control_saturation.i)
        {
            pObj->energy.integer=(pObj->energy.integer>=pObj->max.i)?pObj->max.i:pObj->energy.integer;
            pObj->energy.integer=(pObj->energy.integer<=-pObj->max.i)?-pObj->max.i:pObj->energy.integer;
        }
        action_control+=pObj->energy.integer*pObj->constants.ki;
    }
    
    if(pObj->en_control_type.d && pObj->dt!=0.0)
    {
        float temp_activity;

        temp_activity=(error-pObj->energy.previous_value)/pObj->dt;

        if(pObj->en_control_saturation.d)
        {
            temp_activity=(temp_activity>=pObj->max.d)?pObj->max.d:temp_activity;
            temp_activity=(temp_activity<=-pObj->max.d)?-pObj->max.d:temp_activity;
        }
        action_control+=temp_activity*pObj->constants.kd;
        pObj->energy.previous_value=error;
    }
    action_control+=pObj->offset;

    return action_control;
}

//reset energy storage varibales
void ControlWindup(ControlData *pObj)
{
    pObj->energy.integer = 0.0;
    pObj->energy.previous_value = 0.0;
}
