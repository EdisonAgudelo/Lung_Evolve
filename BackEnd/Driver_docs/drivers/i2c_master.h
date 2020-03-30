#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <Arduino.h>
#include <Wire.h>

struct MASTER_PARAMETER{;
uint8_t tx_DAT;
uint8_t tx_SIZE_DAT;
uint8_t rx_DAT;
uint8_t rx_SIZE_DAT;

};extern struct MASTER_PARAMETER MASTER_parameter;

#define MASTER_ADDRESS 1;
#define SLAVE1_ADDRESS 2;


void MASTER_init(void);
void data_frame_conformation();
void MASTER_Write_Data(uint8_t SLAVE_ADDRESS,uint8_t DAT);
uint8_t MASTER_Recieve_Data(uint8_t SLAVE_ADDRESS,uint8_t SIZE_DAT);

#endif /*I2C_MASTER_H*/
