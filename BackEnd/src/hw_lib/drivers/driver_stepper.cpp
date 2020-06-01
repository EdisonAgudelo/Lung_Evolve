
#include "driver_stepper.h"
#include "../low_level/hardware_interface.h"
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
}

Stepper::Stepper(int _pin_step, int _pin_dir, int _pin_enable, int _pwm_gen_id) : pin_step(_pin_step), pin_dir(_pin_dir), pin_enable(_pin_enable), pwm_id(_pwm_gen_id)
{
    //indicate that other pins are no available
    pin_limit_backward = -1;
    pin_limit_forward = -1;
}

void Stepper::Begin(void)
{
    //set the default state.
    vel_target = 100; //any value
    pos_target = 0;
    vel_actual = 0;
    pos_actual = 0;

    acc_value = STEPPER_DEFAULT_ACC;

    pos_request = 0;
    pos_change_request = false;

    state = kStepperStateStop;
    step_period = 1000; //any value to avoid 0 division

    brake_distance = 0;

    hard_stop_flag = false;
    soft_stop_flag = false;
    no_change_param = false;

    soft_stop_request = false;

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

    update_prev_time = Millis();
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
    //check if it is posible to change parameter an avoid sinc errors
    if (!soft_stop_flag)
    { //don not excute if there is a soft stop running
        if (!no_change_param)
        {
            //calculate the distance traveled since last update
            CalculateDistance();

            //stuck pin to low to stop driver. and PWM
            PinSetDigital(pin_step, !kStepLevel);

            //indicate that motor is stopped
            vel_actual = 0;

            pos_target = pos_actual;

            //disable driver
            if (0 <= pin_enable)
                PinSetDigital(pin_enable, !kStepEnable);

 

            //detach timer interrupt
            TimeVirtualISRAdd(id, nullptr, 0); 

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

            //disconet pwm output
            PinSetDigital(pin_step, !kStepLevel);

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

            //try to get reach position
            while (0 != missed_steps-- && (0 > pin_end || !PinReadDigital(pin_end)))
            {
                PinSetDigital(pin_step, kStepLevel);
                uDelay(step_period / 2000);
                PinSetDigital(pin_step, !kStepLevel);
                uDelay(step_period / 2000);
            }

            //if while ends due to end switch
            if (0 <= pin_end && PinReadDigital(pin_end))
                pos_target = pos_actual; //force  position
            else{
                pos_target -= pos_adj;
                pos_actual = pos_target; //force  position
            }
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

    if(soft_stop_request){
        soft_stop_request = false;
        SoftStop();
    }

    if (STEPPER_UPDATE_PERIOD <= GetDiffTime(Millis(), update_prev_time))
    {
        update_prev_time = Millis();

        no_change_param = true;

        //////secure zone/////

        CalculateDistance();

        switch (state)
        {
        case kStepperStateFoward:
            remain_distance = pos_target > pos_actual ? (uint32_t)(pos_target - pos_actual) : 0;
            goto StepperStateBoth;

        case kStepperStateBackward:
            remain_distance = pos_target < pos_actual ? (uint32_t)(pos_actual - pos_target) : 0;
            
            StepperStateBoth:
            update_param = false;

            // check if motor is close to target position and it is moving
            if ((remain_distance < brake_distance) && (0 != vel_target))
            {
                //slowdown motor vel
                if (vel_actual != STEPPER_MIN_VEL_TO_STOP)
                {
                    vel_actual -= acc_value;

                    //hold a minimum operation vel
                    vel_actual = vel_actual < STEPPER_MIN_VEL_TO_STOP ? STEPPER_MIN_VEL_TO_STOP : vel_actual;

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

                //prevent motor works under hardware capabilities -> actually it is no needed
                //vel_actual = (vel_actual < STEPPER_MIN_VEL && vel_actual != 0) ? STEPPER_MIN_VEL : vel_actual;

                //trigger param calculation
                update_param = true;

                //recalculate brake distance
                brake_distance = (vel_actual * vel_actual) / ((int32_t)(acc_value*2*((uint32_t)1000/STEPPER_UPDATE_PERIOD)));

            }

            //if actual vel reach 0 value stop driver.
            if (0 >= vel_actual)
            {
                HardStop();
            }
            //if a update parameter was triggered
            else if (update_param)
            {

                //compute step period
                //step_period = 1000000000 / vel_actual;

                //uptade pwm frecuency and attach pin, compute step period
                step_period=PWMConfigFrecuency((uint32_t)vel_actual, pwm_id);

                //compute with this velocity how much time it will require to reach pos target
                CalculateTime();

                //set interrupt for stop motor in the correct moment
                TimeVirtualISRAdd(id, timeout_isr[id], estimate_time - STEPPER_TIME_GAP); //stop me in the estimated time

                //if there is not more time,  trigger stop
                if (0 > estimate_time)
                    SoftStop();
            }

            break;

        case kStepperStateStop:
            // check if user set a position

            brake_distance=0;

            //check if an pos request is active
            if (pos_change_request)
            {
                pos_change_request = false; //end request
                pos_target = pos_request;
                pos_adj = ((pos_target-pos_actual) * STEPPER_FINE_ADJ) /STEPPER_FINE_ADJ_DIV;
                pos_target += pos_adj;
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
        soft_stop_request = true;
        break;

    default:
        break;
    }
}

void Stepper::SetVel(int32_t _vel)
{
    vel_target = _vel;
    vel_target = vel_target < 0 ? 0 : vel_target; //avoid negative velocity;
}

//for sercurity reasons, only a pos target can ben changed in stop estate.
void Stepper::SetPos(int32_t _pos)
{

    if (_pos != pos_target)
    {
        pos_change_request = true;
        pos_request = _pos;
    }
}

void Stepper::CalculateDistance(void)
{
    
    uint32_t enlapsed_time = GetDiffTime(Micros(), count_prev_time);
    count_prev_time = Micros();

    if (0 != vel_actual)
        switch (state)
        {
        case kStepperStateFoward:

            if (!PWMIsPendingInterrupt(pwm_id))
            {
                PWMRequestInterrupt(pwm_id);
                pos_actual++;
            }
            pos_actual += (enlapsed_time * 1000) / step_period;
            //pos_actual += contador1();

            break;
        case kStepperStateBackward:

            if (!PWMIsPendingInterrupt(pwm_id))
            {
                PWMRequestInterrupt(pwm_id);
                pos_actual--;
            }

            pos_actual -= ((enlapsed_time * 1000) / step_period);
            
            break;
        default:
            break;
        }

}

void Stepper::CalculateTime(void)
{
    int32_t delta_distance;

    if (0 == vel_actual)
        estimate_time = 0x7fffffff;
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
            estimate_time = 0x7fffffff;
            return;
        }

        if (delta_distance < 0)
            estimate_time = 0;
        else
            estimate_time =((int32_t)(((double)delta_distance)*(((double)step_period)/1000000.0)));
    }
}

StepperDriverStates Stepper::GetState(void)
{
    return state;
}

int32_t Stepper::GetPos(void)
{
    return pos_actual;
}

float Stepper::GetPosmm(void)
{
    return pos_actual/steps_per_mm;
}


void Stepper::SetVelmm(float _vel_mm_s)
{
    SetVel((int32_t)(_vel_mm_s*steps_per_mm));
}

void Stepper::SetPosmm(float _pos_mm)
{
    SetPos((int32_t)(_pos_mm*steps_per_mm));
}

void Stepper::SetDriverConfig(uint16_t _steps_per_rev, float _mm_per_rev, uint8_t _u_steps)
{
    u_steps=_u_steps;
    steps_per_mm = (_steps_per_rev * _u_steps) / _mm_per_rev; 
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
