#ifndef BACKEND_MANAGER_H
#define BACKEND_MANAGER_H

#include "../data_bus/data_bus.h" /////////revisar
//#include "../hardware/hardware_front.h"
#include "../alarm_manager/alarm_manager.h"

uint8_t prev_state; 

void serial_backend_init(void);

//uint8_t CRC8Calculate(uint8_t *data, uint8_t length);

#endif /*BACKEND_MANAGER_H*/