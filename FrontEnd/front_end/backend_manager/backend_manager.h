#ifndef BACKEND_MANAGER_H
#define BACKEND_MANAGER_H

//serial to backend pins
#define SBACK_END_TX 8
#define SBACK_END_RX 9

void init_serial(void);
void serial_conf(void);
uint8_t CRC8Calculate(uint8_t *data, uint8_t length);

#endif /*BACKEND_MANAGER_H*/