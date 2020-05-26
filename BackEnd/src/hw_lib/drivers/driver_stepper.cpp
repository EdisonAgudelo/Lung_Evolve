
#include "driver_stepper.h"
#include "hardware_interface.h"
#include "../time.h"

//initialize class variables
int Stepper::self_count = 0;
void *Stepper::self[STEPPER_MAX_COUNT] = {nullptr};                                                 //no instances are available yet
stepper_callback Stepper::pin_isr[STEPPER_MAX_COUNT] = {Pin1ISR, Pin2ISR, Pin3ISR};                 //put all PinXISR callbacks here
stepper_callback Stepper::timeout_isr[STEPPER_MAX_COUNT] = {TimeOut1ISR, TimeOut2ISR, TimeOut3ISR}; //put all PinXISR callbacks here

Stepper::Stepper(int _pin_step, int _pin_dir, int _pwm_gen_id) : pin_step(_pin_step), pin_dir(_pin_dir), pwm_id(_pwm_gen_id)
{
    //indicate that other pins are no available
    pin_enable = -1;
    pin_limit_backward = -1;
    pin_limit_forward = -1;

    GeneralInit();
}

Stepper::Stepper(int _pin_step, int _pin_dir, int _pin_enable, int _pwm_gen_id) : pin_step(_pin_step), pin_dir(_pin_dir), pin_enable(_pin_enable), pwm_id(_pwm_gen_id)
{
    //indicate that other pins are no available
    pin_limit_backward = -1;
    pin_limit_forward = -1;

    GeneralInit();
}

void Stepper::GeneralInit(void)
{
    //set the default state.
    vel_target = STEPPER_MIN_VEL;
    pos_target = 0;
    vel_actual = 0;
    pos_actual = 0;

    acc_value = STEPPER_DEFAULT_ACC;

    pos_request = 0;
    pos_change_request = false;

    state = kStepperStateStop;
    step_period = 1000; //any value to avoid 0 division

    break_distance = 0;

    hard_stop_flag = false;
    soft_stop_flag = false;
    no_change_param = false;

    //save this instance id
    id = self_count;

    //increment clases instance created
    self[self_count++] = (void *)this;

    // confing HW pin
    PinConfigDigital(pin_step, kOutput);
    PinConfigDigital(pin_dir, kOutput);

    PinSetDigital(pin_step, !kStepLevel);
    PinSetDigital(pin_dir, !kStepForward);

    if (0 <= pin_enable)
    {
        PinConfigDigital(pin_enable, kOutput);
        PinSetDigital(pin_enable, !kStepEnable);
    }
}

Stepper::~Stepper()
{
    //this library is no designed to have dinamic instances
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
    hard_stop_flag = true;
    //check if it is posible to change parameter an avoid sinc erros
    if (!soft_stop_flag)
    { //don not excute if there is a soft stop running
        if (!no_change_param)
        {
            //calculate the distance traveled since last update
            CalculateDistance();

            //indicate that motor is stopped
            vel_actual = 0;

            pos_target = pos_actual;

            //disable driver
            if (0 <= pin_enable)
                PinSetDigital(pin_enable, !kStepEnable);

            //stuck pin to low to stop driver.
            PinSetDigital(pin_step, !kStepLevel);

            //detach timer interrupt
            TimeVirtualISRAdd(id, nullptr, 0); //stop me in the estimated time

            //update state
            state = kStepperStateStop;

            hard_stop_flag = false;
        }
    }
    else
    {
        hard_stop_flag = false;
    }
}

//this function should be tested for correct operation
void Stepper::SoftStop(void)
{
    int32_t missed_steps = 0;
    int pin_end;

    //if there is a hard stop pendig, ignore this soft stop
    if (!hard_stop_flag)
    {
        soft_stop_flag = true;
        //check if it is posible to change parameter to avoid sinc errors
        if (!no_change_param)
        {

            vel_actual = 0;

            //calculate the distance traveled since last update
            CalculateDistance();

            switch (state)
            {
            case kStepperStateFoward:
                missed_steps = pos_target - pos_actual;
                pin_end = pin_limit_forward;

                break;
            case kStepperStateBackward:
                missed_steps = pos_actual - pos_target;
                pin_end = pin_limit_backward;
                break;
            default:
                return; //a hard stop was execute
                break;
            }

            missed_steps = missed_steps < 0 ? 0 : missed_steps;

            //disconet pwm output
            PinSetDigital(pin_step, !kStepLevel);

            //try to get reach position
            while (0 != missed_steps-- && (0 > pin_end || !PinReadDigital(pin_end)))
            {
                PinSetDigital(pin_step, kStepLevel);
                delayMicroseconds(step_period / 1000);
                PinSetDigital(pin_step, !kStepLevel);
                delayMicroseconds(step_period / 1000);
            }

            //if while ends due to end switch
            if (0 <= pin_end && PinReadDigital(pin_end))
                pos_target = pos_actual; //force some position
            else
                pos_actual = pos_target; //force some position

            //disable driver
            if (0 <= pin_enable)
                PinSetDigital(pin_enable, !kStepEnable);

            state = kStepperStateStop;
            soft_stop_flag = false;
        }
    }
}

void Stepper::Loop(void)
{
    uint32_t remain_distance;
    bool update_param;

    if (STEPPER_UPDATE_PERIOD <= GetDiffTime(Millis(), count_prev_time))
    {
        count_prev_time = Millis();

        no_change_param = true;

        CalculateDistance();

        switch (state)
        {
        case kStepperStateFoward:
        case kStepperStateBackward:

            update_param = false;
            remain_distance = pos_target > pos_actual ? pos_target - pos_actual : pos_actual - pos_target;

            // check if motor is close to target position and it is moving
            if (remain_distance < break_distance && 0 != vel_target)
            {
                //slowdown motor vel
                if (vel_actual != STEPPER_MIN_VEL)
                {
                    vel_actual -= acc_value;

                    //hold a minimum operation vel
                    vel_actual = vel_actual < STEPPER_MIN_VEL ? STEPPER_MIN_VEL : vel_actual;

                    //trigger param calculation
                    update_param = true;
                }
            }
            //check if it should update velocity value
            else if (vel_actual != vel_target)
            {

                if (vel_actual < vel_target)
                {
                    vel_actual += acc_value;

                    //prevent overshoot
                    vel_actual = vel_actual > vel_target ? vel_target : vel_actual;
                }
                else
                {
                    vel_actual -= acc_value;

                    //prevent overshoot
                    vel_actual = vel_actual < vel_target ? vel_target : vel_actual;
                }

                //prevent motor works under hardware capabilities
                vel_actual = (vel_actual < STEPPER_MIN_VEL && vel_actual != 0) ? STEPPER_MIN_VEL : vel_actual;

                //trigger param calculation
                update_param = true;

                //recalculate break distance
                break_distance = vel_actual * vel_actual / acc_value;
            }

            //if actual vel reach 0 value stop driver.
            if (0 == vel_actual)
            {
                HardStop();
            }
            //if a update parameter tirgger is pending
            else if (update_param)
            {

                //compute step period
                step_period = 1000000000 / vel_actual;

                //uptade pwm frecuency
                PWMConfigFrecuency(vel_actual, pwm_id);

                //compute with this velocity how much time it will require to reach pos target
                CalculateTime();

                //set interrupt for stop motor in the correct moment
                TimeVirtualISRAdd(id, timeout_isr[id], estimate_time); //stop me in the estimated time

                //if there is not more time mamually trigger stop
                if (0 == estimate_time)
                    SoftStop();
            }

            break;

        case kStepperStateStop:
            // check if user set a position

            //check if an inversion request is active
            if (pos_change_request)
            {
                pos_change_request = false; //end request
                pos_target = pos_request;
            }

            //check if this driver can jump to foward
            if (pos_target > pos_actual && (0 > pin_limit_forward || !PinReadDigital(pin_limit_forward)))
            {

                state = kStepperStateFoward;

                //setup driver
                PinSetDigital(pin_dir, kStepForward);
                if (0 <= pin_enable)
                    PinSetDigital(pin_enable, kStepEnable);
            }
            //check if this driver can jump to backward
            else if (pos_target < pos_actual && (0 > pin_limit_backward || !PinReadDigital(pin_limit_backward)))
            {

                state = kStepperStateBackward;

                //setup driver
                PinSetDigital(pin_dir, !kStepForward);
                if (0 <= pin_enable)
                    PinSetDigital(pin_enable, kStepEnable);
            }
            break;
        }

        no_change_param = false;

        //execute pendig actions
        if (hard_stop_flag)
            HardStop();
        if (soft_stop_flag)
            SoftStop();
    }
}

void Stepper::ISRHandle(int type)
{
    switch (type)
    {
    case kSteeperISRTypeTravelEnd:

        switch (state)
        {
        case kStepperStateFoward:
            if (0 <= pin_limit_forward && PinReadDigital(pin_limit_forward))
                HardStop();
            break;
        case kStepperStateBackward:
            if (0 <= pin_limit_backward && PinReadDigital(pin_limit_backward))
                HardStop();
            break;
        }
        break;

    case kSteeperISRTypeTimeOut:
        SoftStop();
        break;

    default:
        break;
    }
}

void Stepper::SetVel(int32_t _vel)
{
    vel_target = _vel;
    vel_target = vel_target < STEPPER_MIN_VEL ? STEPPER_MIN_VEL : vel_target; //avoid negative velocity;
}

//for sercurity reasons, only a pos target can ben changed in stop estate.
void Stepper::SetPos(int32_t _pos)
{

    if (_pos != pos_target)
    {
        pos_change_request = true;
        pos_request = _pos;
    }
    /*
    //check if there are changes
    if (_pos != pos_target)
    {
        //update actual distance
        CalculateDistance();

        //reset flag
        inversion_flag = false;

        
        switch (state)
        {
        case kStepperStateFoward:

            //if it is ahead of actual position, nothing happen
            if (_pos > pos_actual)
            {
                pos_target = _pos;
                CalculateTime();
                TimeVirtualISRAdd(id, timeout_isr[id], estimate_time); //stop me in the estimated time
            }

            //if it is back of actual position, trigger an stop request
            else
            {
                inversion_flag = true;
                pos_transfer = _pos;
                vel_tranfer = vel_target;
                vel_target = 0;
                TimeVirtualISRAdd(id, nullptr, 0); //delete callback
            }

            break;
        case kStepperStateBackward:

            //if it is ahead of las position, nothing happen
            if (_pos < pos_target)
            {
                pos_target = _pos;
                CalculateTime();
                TimeVirtualISRAdd(id, timeout_isr[id], estimate_time); //stop me in the estimated time
            }
            
            //if it is back of actual position, trigger an stop request
            else
            {
                inversion_flag = true;
                pos_transfer = _pos;
                vel_tranfer = vel_target;
                vel_target = 0;
                TimeVirtualISRAdd(id, nullptr, 0); //delete callback
            }
            break;
        default:
            //if motor is stopped, apply rigth now.
            pos_target = _pos;
            break;
        }
    }
    */
}

void Stepper::CalculateDistance(void)
{
    uint32_t enlapsed_time = GetDiffTime(micros(), count_prev_time);

    if (0 != vel_actual)
        switch (state)
        {
        case kStepperStateFoward:
            pos_actual += (enlapsed_time * 1000) / step_period;

            if (!PWMIsPendingInterrupt(pwm_id))
            {
                PWMRequestInterrupt(pwm_id);
                pos_actual++;
            }

            break;
        case kStepperStateBackward:
            pos_actual -= ((enlapsed_time * 1000) / step_period);

            if (!PWMIsPendingInterrupt(pwm_id))
            {
                PWMRequestInterrupt(pwm_id);
                pos_actual--;
            }
            break;
        default:
            break;
        }

    count_prev_time = micros();
}

void Stepper::CalculateTime(void)
{
    int32_t delta_distance;

    if (0 == vel_actual)
        estimate_time = 0xffffffff;
    else
    {
        switch (state)
        {
        case kStepperStateFoward:
            delta_distance = pos_target - pos_actual;
            break;
        case kStepperStateBackward:
            delta_distance = pos_actual - pos_target;
            break;
        default:
            estimate_time = 0xffffffff;
            return;
        }

        if (delta_distance < 0)
            estimate_time = 0;
        else
            estimate_time = (delta_distance * 1000) / vel_actual - STEPPER_TIME_GAP;
    }
}

StepperDriverStates Stepper::GetState(void)
{
    return state;
}

/////////////// define n PinXISR acording to STEPPER_MAX_COUNT ////////////

void Pin1ISR(void)
{
    ((Stepper *)(Stepper::self[0]))->ISRHandle(kSteeperISRTypeTravelEnd);
}

void Pin2ISR(void)
{
    ((Stepper *)(Stepper::self[1]))->ISRHandle(kSteeperISRTypeTravelEnd);
}

void Pin3ISR(void)
{
    ((Stepper *)(Stepper::self[2]))->ISRHandle(kSteeperISRTypeTravelEnd);
}

/////////////// define n TimeOutXISR acording to STEPPER_MAX_COUNT ////////////
void TimeOut1ISR(void)
{
    ((Stepper *)(Stepper::self[0]))->ISRHandle(kSteeperISRTypeTimeOut);
}

void TimeOut2ISR(void)
{
    ((Stepper *)(Stepper::self[1]))->ISRHandle(kSteeperISRTypeTimeOut);
}

void TimeOut3ISR(void)
{
    ((Stepper *)(Stepper::self[2]))->ISRHandle(kSteeperISRTypeTimeOut);
}
