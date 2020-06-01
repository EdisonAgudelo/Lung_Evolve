#ifndef BACKEND_MANAGER_H
#define BACKEND_MANAGER_H

#include "../data_bus/data_bus.h" /////////revisar
//#include "../hardware/hardware_front.h"
#include "../alarm_manager/alarm_manager.h"
#include "../screen_manager/screen_manager.h"
#include "crc8.h"

typedef enum 
{
    krecieve=0,
    kcheckStatus,
    kcheckScreenUpdate,
    ksendData
}STATE_backend;

 

void serial_backend_init(void);
bool recieve(void);
void check_data(uint8_t *Buffer, int nbytes);
void get_data(uint8_t *Buffer, int nbytes);
void sendDATA(void);
void sendACK(void);
void sendNACK(void);
void backend_management(void);


#endif /*BACKEND_MANAGER_H*/