#ifndef HARDWARE_FRONT_H
#define HARDWARE_FRONT_H

#include "driver_led.h"

extern DriverLed RED_led;
extern DriverLed YELLOW_led;



//frontend peripheral pins
#define Ypin 9 
#define Rpin 10
#define BUZZERpin 3

//serial to backend pins
#define SFRONT_END_TX 7
#define SFRONT_END_RX 6

//duty cycle front end peripherals 
#define DUTY_LEDY 128
#define DUTY_LEDR 128
#define DUTY_BUZZER 128

void REDLed(bool action);
void YELLOWLed(bool action);
void BUZZER(bool action);
void init_hardware_front(void);

#endif /*HARDWARE_FRONT_H*/