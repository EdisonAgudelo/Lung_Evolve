#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <Arduino.h>
#include <Wire.h>

struct MASTER_PARAMETER{;
uint8_t tx_DAT;
uint8_t tx_SIZE_DAT;
uint8_t rx_DAT;
uint8_t rx_SIZE_DAT;

};//extern struct MASTER_PARAMETER MASTER_parameter;

uint16_t working_mode;


#define MASTER_ADDRESS 1
#define SLAVE1_ADDRESS 2

#define SIZE_DATA 2

#define TUNNING 0x3;              //0011
#define STAND_BY 0x6;             //0110
#define PRESSURE_CONTROLLED 0x6C; //1100

void MASTER_init(void);
void MASTER_Write_Data(uint8_t SLAVE_ADDRESS,uint16_t DAT);
uint8_t MASTER_Recieve_Data(uint8_t SLAVE_ADDRESS,uint16_t SIZE_DAT);

#endif /*I2C_MASTER_H*/
