
#ifndef _DRIVER_STEPPER_H
#define _DRIVER_STEPPER_H


#define STEPPER_MAX_COUNT 3 //max instaces of the class

#define STEPPER_UPDATE_PERIOD 100 //Each UPDATE_PERIOD ms this library performs an upate in vel referece. This for aceleration purpose.
#define STEPPER_MIN_VEL_TO_STOP 60 // steps/s
#define STEPPER_TIME_GAP 5 //this value idicate how much millisecond is isr timeout ahead of real stimate end time
#define STEPPER_DEFAULT_ACC 60 //steps/(100ms)^2 => 600 steps/s^2


#include "hardware_interface.h"

const bool kStepForward = kHigh;
const bool kStepBackward = kLow;
const bool kStepLevel = kHigh;
const bool kStepEnable = kHigh;

enum{
    kSteeperISRTypeTravelEnd,
    kSteeperISRTypeTimeOut
};

typedef enum {
    kStepperStateStop,
    kStepperStateFoward,
    kStepperStateBackward
}StepperDriverStates;

typedef void (*stepper_callback)(void);


class Stepper
{
    private:

        //driver hardware
        int pin_step;
        int pin_dir;
        int pin_enable;
        int pin_limit_backward;  //end switch to stop inmed
        int pin_limit_forward;
        uint8_t pwm_id;          //counter id
        
        //driver control
        int32_t vel_target; //in steps per seconds
        int32_t pos_target; //in steps
        int32_t vel_actual;
        int32_t pos_actual;
        uint8_t acc_value; // this value is added to vel_actual value each STEPPER_UPDATE_PERIOD

        //for protection
        int32_t pos_request; //used to save target pos when a new pos target is requested 
        bool pos_change_request;

        bool soft_stop_request; // for timeout interrupt;

        StepperDriverStates state;

        uint32_t count_prev_time; //us  //this save the las revision of timer counts
        uint32_t step_period; //ns

        uint32_t update_prev_time; //this is for update vel rate.

        int32_t estimate_time; //ms

        uint32_t break_distance; //save the delta distance in which motor should start slow down;

        //stop sincronization 
        bool hard_stop_flag; //to avoid strange behaviour, it is set to ISR soft stop to indicate that it is waiting for execution
        bool soft_stop_flag; //to avoid strange behaviour, it is set to ISR hard stop to indicate that it is waiting for execution
        bool no_change_param; //it is used by main code flow avoid asycronus interrupt execution

        //driver config
        uint16_t steps_per_rev;
        uint8_t u_steps ;

        //class usage
        int id;

        //class variables
        static int self_count;

        static stepper_callback pin_isr[STEPPER_MAX_COUNT];
        static stepper_callback timeout_isr[STEPPER_MAX_COUNT];

        //private methods
        void CalculateDistance(void);
        void CalculateTime(void);
        void GeneralInit(void);
        
    public:
        
        static void *self[STEPPER_MAX_COUNT];

        Stepper(int _pin_step, int _pin_dir, int _pwm_gen_id);
        Stepper(int _pin_step, int _pin_dir, int _pin_enable, int _pwm_gen_id);
        ~Stepper();

        void SetLimitPin(int _pin_limmit_backward, int _pin_limmit_forward);

       //methods
        void ISRHandle(int type);
        void HardStop(void); //this function stops motor without pos error adjustment
        void SoftStop(void); //this function stops motor but first it tres to reach desired ref pos manually
        void Loop(void);
        
        //setters
        void SetVel(int32_t _vel); 
        void SetPos(int32_t _pos);

        //getters
        StepperDriverStates GetState(void);
        int32_t GetPos(void);
};




void Pin1ISR(void);
void Pin2ISR(void);
void Pin3ISR(void);


void TimeOut1ISR(void);
void TimeOut2ISR(void);
void TimeOut3ISR(void);








#endif