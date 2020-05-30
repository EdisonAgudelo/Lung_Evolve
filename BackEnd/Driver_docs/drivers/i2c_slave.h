#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

/**********************************************************************/

#include "i2c_common.h"


uint8_t buffer_update;  // this is a flag, its 1 when there has been storage a complete parameter in the buffer (2 bytes) // it has to be clear in the user code

struct SLAVE_PARAMETER
{
  
  uint8_t tx_DAT;
  uint8_t tx_SIZE_DAT;
  uint8_t rx_DAT;
  uint8_t rx_SIZE_DAT;
  uint16_t DAT_FRAME[15];
};
extern struct SLAVE_PARAMETER SLAVE_parameter;


#define SIZE_DATA 2

#define MASTER_ADDRESS 1
#define SLAVE1_ADDRESS 2
//#define LENGTH(Array[]) sizeof(Array)/sizeof(Array[0])

void SLAVE_init(void);
void function_recieve(int numBytes);
void function_send_from_request(void);
//void data_frame_conformation(void);
//void SLAVE_Write_Data(uint8_t MASTER_ADDRESS,uint8_t DAT);
//uint8_t SLAVE_Reeieve_Data(uint8_t MASTER_ADDRESS,uint8_t SIZE_DAT);

#endif /*I2C_SLAVE_H*/