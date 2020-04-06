#include "easydriver.h"

#include "gpio.h"

/* to check out the driver http://www.schmalzhaus.com/EasyDriver/
 * the correct use of this functions is recomended to do it as follows:
 * 
 * configure_driver_pins(uint8_t DIR,uint8_t STEP)
 * motor_steps_per_rev(uint8_t STEPS_PER_REV)
 * initialize_driver_pins(uint8_t DIR)
 * set_speed_motor(uint8_t RPM)
 * stepper_motor_action
 * 
*/

extern struct STEPPER_MOTOR_PARAMETERS stepper_motor_parameters;
extern struct DRIVER_PARAMETERS driver_parameters;

/************************************************************************************/
void configure_driver_pins(uint8_t DIR,uint8_t STEP){
  //static struct DRIVER_PARAMETERS driver_parameters;
  pinMode(DIR,OUTPUT);          //set DIR pin as an output
  pinMode(STEP,OUTPUT);         //set STEP pin as an output
  driver_parameters.DIR=DIR;    //save parameter DIR into driver parameters struct
  driver_parameters.STEP=STEP;  //save parameter DIR into driver parameters struct
}

/************************************************************************************/
void initialize_driver_pins(uint8_t DIR){
  //static struct DRIVER_PARAMETERS driver_parameters;
  digitalWrite(driver_parameters.DIR,LOW);   //set DIR pin as High value (5V)
  digitalWrite(driver_parameters.STEP,LOW);   //set DIR pin as Low value (0V)
}

/************************************************************************************/
void motor_steps_per_rev(uint8_t STEPS_PER_REV){
  //static struct STEPPER_MOTOR_PARAMETERS stepper_motor_parameters;
  stepper_motor_parameters.STEPS_PER_REV=STEPS_PER_REV;
}

/************************************************************************************/
void set_speed_motor(uint8_t RPM){
  //static struct STEPPER_MOTOR_PARAMETERS stepper_motor_parameters;
  stepper_motor_parameters.SPEED_RPM=RPM;

}

/************************************************************************************/
void stepper_speed_action(uint8_t STATE_MOTOR, bool DIRECTION, uint8_t PIN_ENDSTOP){     //if STATE_MOTOR is 0, it is no movement, if it is 1,te motor moves as configurated
  //static struct DRIVER_PARAMETERS driver_parameters;
    //static struct STEPPER_MOTOR_PARAMETERS stepper_motor_parameters;
   
  switch(STATE_MOTOR)
  {
    case 0: OFF_STEPPER_MOTOR;
            break;
    case 1: set_direction(DIRECTION);
            for(int i=0;(i<STEPS_PER_SECOND);i++)
            {
              if (detect_endstop(PIN_ENDSTOP)){
                STATE_MOTOR=0;
                break;
              }
              else
              {
                digitalWrite( driver_parameters.STEP, HIGH);
                delayMicroseconds(10);
                digitalWrite( driver_parameters.STEP, LOW);
                delay(STEP_DELAY);
              }
    
            }
            break;
    default: OFF_STEPPER_MOTOR;
            break; 
  }
}

/************************************************************************************/
void stepper_distance_action(uint8_t STATE_MOTOR, bool DIRECTION, uint8_t DISTANCE, uint8_t PIN_ENDSTOP){     //if STATE_MOTOR is 0, it is no movement, if it is 1,te motor moves as configurated
  //static struct DRIVER_PARAMETERS driver_parameters;
    //static struct STEPPER_MOTOR_PARAMETERS stepper_motor_parameters;
   
  switch(STATE_MOTOR)
  {
    case 0: OFF_STEPPER_MOTOR;
            break;
    case 1: set_direction(DIRECTION);
            float f_STEPS_TO_MOVE;
            uint8_t STEPS_TO_MOVE;
            f_STEPS_TO_MOVE=DISTANCE*stepper_motor_parameters.STEPS_PER_REV/stepper_motor_parameters.AXIS_PERIMETER;
            STEPS_TO_MOVE= (uint8_t) f_STEPS_TO_MOVE;
            for(int i=0;(i<STEPS_PER_SECOND);i++)
            {
              if(detect_endstop(PIN_ENDSTOP)){
                STATE_MOTOR=0;
                break;
              }
              else
              {
                digitalWrite( driver_parameters.STEP, HIGH);
                delayMicroseconds(10);
                digitalWrite( driver_parameters.STEP, LOW);
                delay(STEP_DELAY);                                //revisar bien el tiempo 
                STEP_COUNT++;

              }
                 break;
            }        
            
     default: OFF_STEPPER_MOTOR;
            break; 
  }
}


/************************************************************************************/
void set_direction(bool DIRECTION){
  //static struct DRIVER_PARAMETERS driver_parameters;
    //  static struct STEPPER_MOTOR_PARAMETERS stepper_motor_parameters;
  stepper_motor_parameters.DIR=DIRECTION;
  digitalWrite(driver_parameters.DIR, DIRECTION);
  delayMicroseconds(100);
  
}

/************************************************************************************/
bool detect_endstop(uint8_t PIN){
  return read_digital_pin(PIN);
  
}

/************************************************************************************/
//uint8_t step_counts(){
  //return STEP_COUNT;
//}

/************************************************************************************/
void reset_counts(){
   STEP_COUNT=0;
}
/******************************** END *************************************************/
