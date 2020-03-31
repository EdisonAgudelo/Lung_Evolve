#ifndef EASYDRIVER_H
#define EASYDRIVER_H

#include <Arduino.h>
struct DRIVER_PARAMETERS{
  int DIR;
  int STEP;
};//struct DRIVER_PARAMETERS driver_parameters;

struct STEPPER_MOTOR_PARAMETERS{
  int STEPS_PER_REV;
  int SPEED_RPM;
  bool DIR;
  float AXIS_PERIMETER; //in mm
};//struct STEPPER_MOTOR_PARAMETERS stepper_motor_parameters;

 extern uint8_t STEP_COUNT;

#define ON_STEPPER_MOTOR digitalWrite(driver_parameters.STEP, HIGH) 
#define OFF_STEPPER_MOTOR digitalWrite(driver_parameters.STEP, LOW)
#define STEPS_PER_SECOND (stepper_motor_parameters.STEPS_PER_REV*stepper_motor_parameters.SPEED_RPM/60)
#define STEP_DELAY 1000/STEPS_PER_SECOND
#define FIXED_STEP_DELAY 6
#define get_step_count STEP_COUNT

void configure_driver_pins(uint8_t DIR,uint8_t STEP);
void initialize_driver_pins(uint8_t DIR);
void motor_steps_per_rev(uint8_t STEPS_PER_REV);
void set_speed_motor(uint8_t RPM);
void stepper_speed_action(uint8_t STATE_MOTOR, bool DIRECTION, uint8_t PIN_ENDSTOP);
void set_direction(bool DIRECTION);
void stepper_distance_action(uint8_t STATE_MOTOR, bool DIRECTION, uint8_t DISTANCE, uint8_t PIN_ENDSTOP);
bool detect_endstop(uint8_t PIN);
void reset_counts(void);
//uint8_t step_counts(void);


#endif /*EASYDRIVER_H*/
