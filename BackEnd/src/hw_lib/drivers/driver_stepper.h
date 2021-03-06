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


#ifndef _DRIVER_STEPPER_H
#define _DRIVER_STEPPER_H

#define STEPPER_MAX_COUNT 3 //max instaces of the class

#define STEPPER_UPDATE_PERIOD 30                                    //Each UPDATE_PERIOD ms this library performs an upate in vel referece. This for aceleration purpose.
#define STEPPER_MIN_VEL_TO_STOP 200                                 // steps/s
#define STEPPER_TIME_GAP 3                                          //this value idicate how much millisecond is isr timeout ahead of real estimate end time
#define STEPPER_DEFAULT_ACC (2000 / (1000 / STEPPER_UPDATE_PERIOD)) // => 1000 steps/s^2
#define STEPPER_FINE_ADJ -2

#include "../low_level/hardware_interface.h"

const bool kStepForward = kHigh;
const bool kStepBackward = kLow;
const bool kStepLevel = kHigh;
const bool kStepEnable = kLow;

enum
{
    kSteeperISRTypeTravelEnd,
    kSteeperISRTypeTimeOut,
    KSteeperISRTypeResetTimeRef
};

typedef enum
{
    kStepperStateStop,
    kStepperStateFoward,
    kStepperStateBackward
} StepperDriverStates;

typedef void (*stepper_callback)(void);

class Stepper
{
private:
    //driver hardware
    int pin_step;
    int pin_dir;
    int pin_enable;
    int pin_limit_backward; //end switch to stop inmed
    int pin_limit_forward;
    uint8_t pwm_id; //counter id

    //driver control
    int32_t vel_target; //in steps per seconds
    int32_t pos_target; //in steps
    int32_t vel_actual;
    int32_t pos_actual;
    int32_t pos_adj;
    uint32_t acc_value; // this value is added to vel_actual value each STEPPER_UPDATE_PERIOD

    //for protection
    int32_t pos_request; //used to save target pos when a new pos target is requested
    bool pos_change_request;

    bool soft_stop_request; // for timeout interrupt;

    StepperDriverStates state;

    double count_prev_time; //us  //this save the las revision of timer counts
    double step_period;     //ns

    uint32_t update_prev_time; //this is for update vel rate.

    int32_t estimate_time; //ms

    int32_t brake_distance; //save the delta distance in which motor should start slow down;
    //stop sincronization
    bool hard_stop_flag;  //to avoid strange behaviour, it is set to ISR soft stop to indicate that it is waiting for execution
    bool soft_stop_flag;  //to avoid strange behaviour, it is set to ISR hard stop to indicate that it is waiting for execution
    bool no_change_param; //it is used by main code flow avoid asycronus interrupt execution

    //driver config
    float steps_per_mm;
    uint8_t u_steps;

    //class usage
    int id;

    //class variables
    static int self_count;

    static stepper_callback pin_isr[STEPPER_MAX_COUNT];
    static stepper_callback timeout_isr[STEPPER_MAX_COUNT];

    //private methods
    void CalculateDistance(void);
    void CalculateTime(void);

    void HardStop(void); //this function stops motor without pos error adjustment
    void SoftStop(void); //this function stops motor but first it tryes to reach desired ref pos manually

public:
    static void *self[STEPPER_MAX_COUNT];

    Stepper(int _pin_step, int _pin_dir, int _pwm_gen_id);
    Stepper(int _pin_step, int _pin_dir, int _pin_enable, int _pwm_gen_id);
    ~Stepper();

    void SetLimitPin(int _pin_limmit_backward, int _pin_limmit_forward);
    void SetDriverConfig(uint16_t _steps_per_rev, float _mm_per_rev, uint8_t _u_steps);

    //methods
    void Begin(void);
    void ISRHandle(int type);
    void Loop(void);
    void RequestStop(void);

    //setters
    void SetVel(int32_t _vel);
    void SetVelmm(float _vel_mm_s);
    void SetPos(int32_t _pos);
    void SetPosmm(float _pos_mm);
    void SetPosRef(void);

    //getters
    StepperDriverStates GetState(void);
    int32_t GetPos(void);
    float GetPosmm(void);
};

void Pin1ISR(void);
void Pin2ISR(void);
void Pin3ISR(void);

void TimeOut1ISR(void);
void TimeOut2ISR(void);
void TimeOut3ISR(void);

#endif