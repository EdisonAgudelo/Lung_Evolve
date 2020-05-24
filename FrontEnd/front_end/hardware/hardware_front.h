#ifndef HARDWARE_FRONT_H
#define HARDWARE_FRONT_H

#define Ypin 7
#define Rpin 6
#define BUZZERpin 3

void REDLed(bool action);
void YELLOWLed(bool action);
void BUZZER(bool action);
void init_hardware_front(void);

#endif /*HARDWARE_FRONT_H*/