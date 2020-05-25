#ifndef BACKEND_MANAGER_H
#define BACKEND_MANAGER_H

uint8_t prev_state; 

void init_serial(void);
void serial_conf(void);
uint8_t CRC8Calculate(uint8_t *data, uint8_t length);

#endif /*BACKEND_MANAGER_H*/