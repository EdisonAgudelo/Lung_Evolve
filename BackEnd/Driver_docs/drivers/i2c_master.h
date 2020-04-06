#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <Arduino.h>
#include <Wire.h>

#include "i2c_common.h"
struct MASTER_PARAMETER
{
  ;
  uint8_t tx_DAT;
  uint8_t tx_SIZE_DAT;
  uint8_t rx_DAT;
  uint8_t rx_SIZE_DAT;

}; 

/**********************************************************************/


uint8_t is_command_sent; // this is a flag, its 1 when a command is already been send, its 0 when a command is not sended yet

uint16_t working_mode;

#define MASTER_ADDRESS 1
#define SLAVE1_ADDRESS 2

#define SIZE_DATA 2

#define TUNNING 0x3;              //0011
#define STAND_BY 0x6;             //0110
#define PRESSURE_CONTROLLED 0x6C; //1100

void MASTER_init(void);
void function_send(uint8_t slave_address, uint8_t comando);
void function_request(uint8_t slave_address);
//void MASTER_Write_Data(uint8_t SLAVE_ADDRESS,uint16_t DAT);
//uint8_t MASTER_Recieve_Data(uint8_t SLAVE_ADDRESS,uint16_t SIZE_DAT);

#endif /*I2C_MASTER_H*/
