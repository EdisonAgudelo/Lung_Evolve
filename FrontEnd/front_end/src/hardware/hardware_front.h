#ifndef HARDWARE_FRONT_H
#define HARDWARE_FRONT_H

#include "driver_led.h"

extern DriverLed RED_led;
extern DriverLed YELLOW_led;




void REDLed(bool action, bool transition);
void YELLOWLed(bool action,bool transition);
void BUZZER(bool action);
void RELE(bool action);
void init_hardware_front(void);
//void ledloop(void);
#endif /*HARDWARE_FRONT_H*/