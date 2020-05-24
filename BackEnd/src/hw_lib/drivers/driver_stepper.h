
#ifndef _DRIVER_STEPPER_H
#define _DRIVER_STEPPER_H


#define STEPPER_MAX_COUNT 3

#define STEPPER_UPDATE_PERIOD 100 //Each UPDATE_PERIOD ms this library performs an upate in vel referece. This for aceleration purpose.

const bool kStepForward = kHigh;
const bool kStepBackward = kLow;
const bool kStepLevel = kHigh;
const bool kStepEnable = kHigh;

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
        uint8_t pwm_id;
        
        //driver control
        int32_t vel_target; //in steps per seconds
        int32_t pos_target; //in steps
        int32_t vel_actual;
        int32_t pos_actual;
        uint8_t acc_value; // this value is added to vel_actual value each STEPPER_UPDATE_PERIOD

        StepperDriverStates state;

        uint32_t count_prev_time; //us  //this save 
        uint32_t step_period; //ns

        uint32_t update_prev_time; //this is

        //driver config
        uint16_t steps_per_rev;
        uint8_t u_steps ;

        //class usage
        int id;
        //class variables
        static int self_count;
        static stepper_callback pin_isr[STEPPER_MAX_COUNT];

        //private methods
        void CalculateDistance(void);
        
    public:
        
        static void *self[STEPPER_MAX_COUNT];

        Stepper(int _pin_step, int _pin_dir, int _pwm_gen_id);
        Stepper(int _pin_step, int _pin_dir, int _pin_enable, int _pwm_gen_id);
        ~Stepper();

        void SetLimitPin(int _pin_limmit_backward, int _pin_limmit_forward);

       //methods
        void ISRHandle(void);
        void HardStop(void);
        void Loop(void);
        
        //setters
        void SetVel(int _vel); 
        void SetPos(int _pos);

        //getters
        StepperDriverStates GetState(void);
};




void Pin1ISR(void);
void Pin2ISR(void);
void Pin3ISR(void);








#endif