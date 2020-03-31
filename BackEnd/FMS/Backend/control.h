

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdint.h>

typedef struct
{
    struct
    {
        bool p : 1;
        bool i : 1;
        bool d : 1;
    } en_control_type;
    struct
    {
        float integer;
        float previous_value;
    }energy;
    struct
    {
        float kp;
        float ki;
        float kd;
    }constants;
    struct
    {
        float i;
        float d;
        float p;
    }max;
    bool take_time:1;
    uint32_t previous_time;
    float dt; //s
    float offset;
}ControlData;

//if any constant is set negative, this automatically disable asociated control. 
//if dt is set to negative, library get time by theysefl
void control_init(ControlData *pObj, float kp, float ki, float kd, float dt);

//extra parameters to has a better control on library 
void control_set_aditional(ControlData *pObj, float max_p, float max_i, float max_d, float offset);

//execute periodically 
float control_execute(ControlData *pObj, float error); 

//reset energy storage varibales
void control_windup(ControlData *pObj);
#endif