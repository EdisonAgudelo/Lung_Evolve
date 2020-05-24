
#include "driver_stepper.h"
#include "hardware_interface.h"
#include "../../fw_lib/time.h"

//initialize class variables
int Stepper::self_count = 0;
void *Stepper::self[STEPPER_MAX_COUNT] = {nullptr};                                 //no instances are available yet
stepper_callback Stepper::pin_isr[STEPPER_MAX_COUNT] = {Pin1ISR, Pin2ISR, Pin3ISR}; //put all PinXISR callbacks here

Stepper::Stepper(int _pin_step, int _pin_dir, int _pwm_gen_id) : pin_step(_pin_step), pin_dir(_pin_dir), pwm_id(_pwm_gen_id)
{
    //indicate that other pins are no available
    pin_enable = -1;
    pin_limit_backward = -1;
    pin_limit_forward = -1;

    //set the default state.
    vel_target = 0;
    pos_target = 0;
    vel_actual = 0;
    pos_actual = 0;
    state = kStepperStateStop;
    step_period = 1000; //any value to avoid 0 division

    //save this instance id
    id = self_count;

    //increment clases instance created
    self[self_count++] = (void *)this;

    // confing HW pin
    PinConfigDigital(pin_step, kOutput);
    PinConfigDigital(pin_dir, kOutput);

    PinSetDigital(pin_step, !kStepLevel);
    PinSetDigital(pin_dir, !kStepForward);
}

Stepper::Stepper(int _pin_step, int _pin_dir, int _pin_enable, int _pwm_gen_id) : pin_step(_pin_step), pin_dir(_pin_dir), pin_enable(_pin_enable), pwm_id(_pwm_gen_id)
{
    //indicate that other pins are no available
    pin_limit_forward = -1;
    pin_limit_backward = -1;

    //set the default state.
    vel_target = 0;
    pos_target = 0;
    vel_actual = 0;
    pos_actual = 0;
    state = kStepperStateStop;
    step_period = 1000; //any value to avoid 0 division

    //save this instance id
    id = self_count;

    //increment clases instance created
    self[self_count++] = (void *)this;

    // confing HW pin
    PinConfigDigital(pin_step, kOutput);
    PinConfigDigital(pin_dir, kOutput);
    PinConfigDigital(pin_enable, kOutput);

    PinSetDigital(pin_step, !kStepLevel);
    PinSetDigital(pin_dir, !kStepForward);
    PinSetDigital(pin_enable, !kStepEnable);
}

Stepper::~Stepper()
{
}

void Stepper::SetLimitPin(int _pin_limit_backward, int _pin_limit_forward)
{
    pin_limit_backward = _pin_limit_backward;
    pin_limit_forward = _pin_limit_forward;

    if (0 <= pin_limit_backward)
    {
        PinConfigDigital(pin_limit_backward, kInput);
        PinConfigRisingIRS(pin_limit_backward, pin_isr[id]);
    }
    if (0 <= pin_limit_forward)
    {
        PinConfigDigital(pin_limit_forward, kInput);
        PinConfigRisingIRS(pin_limit_forward, pin_isr[id]);
    }
}

void Stepper::HardStop(void)
{
    CalculateDistance();
    vel_target = 0;
    vel_actual = 0;

    PinSetDigital(pin_enable, !kStepEnable);
    PinSetDigital(pin_step, !kStepLevel);
    state = kStepperStateStop;
}

void Stepper::Loop(void)
{
    CalculateDistance();
    switch (state)
    {
    case kStepperStateFoward:
    case kStepperStateBackward:
       
        if(STEPPER_UPDATE_PERIOD<=GetDiffTime(millis(), count_prev_time)){
        
            if(vel_actual<vel_target)
            {
                vel_actual+=acc_value;
                vel_actual=vel_actual>vel_target? vel_target : vel_actual;
            }
            else if(vel_actual>vel_target)
            {
                vel_actual-=acc_value;
                vel_actual=vel_actual<vel_target? vel_target : vel_actual;
            }
            
            
        step_period = 1000000000 / vel_actual;
        }
        break;

    case kStepperStateStop:
        break;
    }
}

void Stepper::ISRHandle(void)
{
    switch (state)
    {
    case kStepperStateFoward:
        if (PinReadDigital(pin_limit_forward))
            HardStop();
        break;
    case kStepperStateBackward:
        if (PinReadDigital(pin_limit_backward))
            HardStop();
        break;
    }
}

void Stepper::SetVel(int _vel)
{
    vel_target = _vel;
}

void Stepper::SetPos(int _pos)
{
    pos_target = _pos;
}

void Stepper::CalculateDistance(void)
{
    uint32_t enlapsed_time = GetDiffTime(micros(), count_prev_time);

    switch (state)
    {
    case kStepperStateFoward:
        pos_actual += (enlapsed_time * 1000) / step_period + 1;
        while (!PWMIsButtom(pwm_id));
        break;
    case kStepperStateBackward:
        pos_actual -= ((enlapsed_time * 1000) / step_period + 1);
        while (!PWMIsButtom(pwm_id));
        break;
    default:
        break;
    }
    //wait fow PWM counter reset, this is to try to avoid motor step lose due to previous round

    count_prev_time = micros();
}

StepperDriverStates Stepper::GetState(void)
{
    return state;
}

/////////////// define n PinXISR acording to STEPPER_MAX_COUNT ////////////

void Pin1ISR(void)
{
    ((Stepper *)(Stepper::self[0]))->ISRHandle();
}

void Pin2ISR(void)
{
    ((Stepper *)(Stepper::self[1]))->ISRHandle();
}

void Pin3ISR(void)
{
    ((Stepper *)(Stepper::self[2]))->ISRHandle();
}
