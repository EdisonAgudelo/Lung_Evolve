#include "i2C_master.h"


/*
 * init function has to be called in setup() function
 * 
*/

    extern struct MASTER_PARAMETER MASTER_parameter;
/************************************************************************************/
void MASTER_init(void){
  Wire.begin(MASTER_ADDRESS);
}


/************************************************************************************/
void MASTER_Write_Data(uint8_t SLAVE_ADDRESS,uint16_t DAT){

    MASTER_parameter.tx_DAT=DAT;
    MASTER_parameter.tx_SIZE_DAT=sizeof(DAT);
    Wire.beginTransmission(SLAVE_ADDRESS);                        // transmit to device with address SLAVE_ADDRESS
    Wire.write(MASTER_parameter.tx_DAT );                          // send data frame
    Wire.endTransmission();                                       // stop transmitting

}

/************************************************************************************/
uint8_t MASTER_Recieve_Data(uint8_t SLAVE_ADDRESS){
  Wire.requestFrom(SLAVE_ADDRESS, SIZE_DATA);                      // request SIZE_DATA bytes from slave device SLAVE_ADDRESS
  while(Wire.available())                                         // slave may send less than requested
  { 
     MASTER_parameter.rx_DAT = Wire.read(); // receive a byte as character
    
  }
  return MASTER_parameter.rx_DAT;
}

/******************************************** END **************************************/
