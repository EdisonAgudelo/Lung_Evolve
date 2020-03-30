#ifndef EASYDRIVER_H
#define EASYDRIVER_H

#include <Arduino.h>
struct DRIVER_PARAMETERS{
  int DIR;
  int STEP;
};extern struct DRIVER_PARAMETERS driver_parameters;

struct STEPPER_MOTOR_PARAMETERS{
  int STEPS_PER_REV;
  int SPEED_RPM;
  bool DIR;
};extern struct STEPPER_MOTOR_PARAMETERS stepper_motor_parameters;

#define ON_STEPPER_MOTOR digitalWrite(driver_parameters.STEP, HIGH) 
#define OFF_STEPPER_MOTOR digitalWrite(driver_parameters.STEP, LOW)
#define STEPS_PER_SECOND (stepper_motor_parameters.STEPS_PER_REV*stepper_motor_parameters.SPEED_RPM/60)
#define STEP_DELAY 1000/STEPS_PER_SECOND


void configure_driver_pins(uint8_t DIR,uint8_t STEP);
void initialize_driver_pins(uint8_t DIR);
void motor_steps_per_rev(uint8_t STEPS_PER_REV);
void set_speed_motor(uint8_t RPM);
void stepper_motor_action(uint8_t STATE_MOTOR,bool DIRECTION);
void set_direction(bool DIRECTION);


#endif /*EASYDRIVER_H*/
