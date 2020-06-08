#ifndef HARDWARE_FRONT_H
#define HARDWARE_FRONT_H

#include "driver_led.h"

extern DriverLed RED_led;
extern DriverLed YELLOW_led;




void REDLed(bool action);
void YELLOWLed(bool action);
void BUZZER(bool action);
void init_hardware_front(void);

#endif /*HARDWARE_FRONT_H*/