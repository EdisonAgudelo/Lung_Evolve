#include "easydriver.h"

/* para revisar el driver http://www.schmalzhaus.com/EasyDriver/
 * the correct use of this functions is recomended to do it as follows:
 * 
 * configure_driver_pins(uint8_t DIR,uint8_t STEP)
 * motor_steps_per_rev(uint8_t STEPS_PER_REV)
 * initialize_driver_pins(uint8_t DIR)
 * set_speed_motor(uint8_t RPM)
 * stepper_motor_action
 * 
*/

void configure_driver_pins(uint8_t DIR,uint8_t STEP){
  static struct DRIVER_PARAMETERS driver_parameters;
  pinMode(DIR,OUTPUT);          //set DIR pin as an output
  pinMode(STEP,OUTPUT);         //set STEP pin as an output
  driver_parameters.DIR=DIR;    //save parameter DIR into driver parameters struct
  driver_parameters.STEP=STEP;  //save parameter DIR into driver parameters struct
}


void initialize_driver_pins(uint8_t DIR){
  static struct DRIVER_PARAMETERS driver_parameters;
  digitalWrite(driver_parameters.DIR,LOW);   //set DIR pin as High value (5V)
  digitalWrite(driver_parameters.STEP,LOW);   //set DIR pin as Low value (0V)
}


void motor_steps_per_rev(uint8_t STEPS_PER_REV){
  static struct STEPPER_MOTOR_PARAMETERS stepper_motor_parameters;
  stepper_motor_parameters.STEPS_PER_REV=STEPS_PER_REV;
}



void set_speed_motor(uint8_t RPM){
  static struct STEPPER_MOTOR_PARAMETERS stepper_motor_parameters;
  stepper_motor_parameters.SPEED_RPM=RPM;

}

void stepper_motor_action(uint8_t STATE_MOTOR, bool DIRECTION){     //if STATE_MOTOR is 0, it is no movement, if it is 1,te motor moves as configurated
  static struct DRIVER_PARAMETERS driver_parameters;
    static struct STEPPER_MOTOR_PARAMETERS stepper_motor_parameters;
   
  switch(STATE_MOTOR)
  {
    case 0: OFF_STEPPER_MOTOR;
            break;
    case 1: set_direction(DIRECTION);
            for(int i=0;(i<STEPS_PER_SECOND);i++)
            {
              digitalWrite( driver_parameters.STEP, HIGH);
              delayMicroseconds(10);
              digitalWrite( driver_parameters.STEP, LOW);
              delay(STEP_DELAY);
            }
            break;
    default: OFF_STEPPER_MOTOR;
            break; 
  }
}

void set_direction(bool DIRECTION){
  static struct DRIVER_PARAMETERS driver_parameters;
      static struct STEPPER_MOTOR_PARAMETERS stepper_motor_parameters;
  stepper_motor_parameters.DIR=DIRECTION;
  digitalWrite(driver_parameters.DIR, DIRECTION);
  delayMicroseconds(100);
  
}
