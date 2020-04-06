
#include <Arduino.h>

#include "control.h"

//if any constant is set negative, this automatically disable asociated control.
//if dt is set to negative, library get time by theysefl
void ControlInit(ControlData *pObj, float kp, float ki, float kd, float dt)
{
    pObj->constants.kp = kp;
    pObj->constants.ki = ki;
    pObj->constants.kd = kd;
    if (pObj->constants.kp < 0.0)
        pObj->en_control_type.p = false;
    if (pObj->constants.ki < 0.0)
        pObj->en_control_type.i = false;
    if (pObj->constants.kd < 0.0)
        pObj->en_control_type.d = false;
    if (dt <= 0.0)
    {
        pObj->take_time = true;
        pObj->dt = ((float)millis()) / 1000.0;
    }
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
        pObj->max.p = max_p;
    }
    if (pObj->en_control_type.i)
    {
        pObj->max.i = max_i;
    }
    if (pObj->en_control_type.d)
    {
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
        pObj->dt=((float)(millis()-pObj->previous_time))/1000.0;
        pObj->previous_time=millis();
    }

    if(pObj->en_control_type.p)
    {
        action_control+=error*pObj->constants.kp;

        action_control=(action_control>=pObj->max.p)?pObj->max.p:action_control;
        action_control=(action_control<=-pObj->max.p)?-pObj->max.p:action_control;
    }
    
    if(pObj->en_control_type.i)
    {
        pObj->energy.integer+=error*pObj->dt;
        
        pObj->energy.integer=(pObj->energy.integer>=pObj->max.i)?pObj->max.i:pObj->energy.integer;
        pObj->energy.integer=(pObj->energy.integer<=-pObj->max.i)?-pObj->max.i:pObj->energy.integer;
        
        action_control+=pObj->energy.integer*pObj->constants.ki;
    }
    
    if(pObj->en_control_type.d && pObj->dt!=0.0)
    {
        float temp_activity;

        temp_activity=(error-pObj->energy.previous_value)/pObj->dt;

        temp_activity=(temp_activity>=pObj->max.d)?pObj->max.d:temp_activity;
        temp_activity=(temp_activity<=-pObj->max.d)?-pObj->max.d:temp_activity;

        action_control+=temp_activity*pObj->constants.kd;
        pObj->energy.previous_value=error;
    }
    action_control+=pObj->offset;
}

//reset energy storage varibales
void ControlWindup(ControlData *pObj)
{
    pObj->energy.integer = 0.0;
    pObj->energy.previous_value = 0.0;
}
