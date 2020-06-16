#ifndef BACKEND_MANAGER_H
#define BACKEND_MANAGER_H

#include "../data_bus/data_bus.h" /////////revisar
//#include "../hardware/hardware_front.h"
#include "../alarm_manager/alarm_manager.h"
#include "../screen_manager/screen_manager.h"
#include "crc8.h"


 

void backend_init(void);

void backend_management(void);


#endif /*BACKEND_MANAGER_H*/