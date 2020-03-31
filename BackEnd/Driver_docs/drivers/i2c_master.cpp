#include "i2C_master.h"


/*
 * init function has to be called in setup() function
 * 
*/

//extern union BreathingParameters;
//    extern struct MASTER_PARAMETER MASTER_parameter;
/************************************************************************************/
void MASTER_init(void){
  Wire.begin(MASTER_ADDRESS);
}


/************************************************************************************/
void MASTER_Write_Data(uint8_t SLAVE_ADDRESS,uint16_t DAT, uint8_t COM){
        
         Wire.beginTransmission(SLAVE1_ADDRESS);   
         Wire.write(COM );                          // send data frame
         Wire.endTransmission();                 // stop transmittingter
         command_sended=COM;
         Wire.beginTransmission(SLAVE1_ADDRESS);   
         Wire.write(DAT );                          // send data frame
         Wire.endTransmission();                 // stop transmittingter         
         
  }



/************************************************************************************/
void MASTER_Recieve_Data(uint8_t SLAVE_ADDRESS){
  //hace un write para enviar comando
  Wire.requestFrom(SLAVE_ADDRESS, SIZE_DATA);                      // request SIZE_DATA bytes from slave device SLAVE_ADDRESS
  for(i=0;i<(sizeof(BreathingParameters)/sizeof(BreathingParametes.BUFFER[0]);i++)
  {
    while(1<Wire.available())                                         // slave may send less than requested
    { 
       BreathingParameters.BUFFER[command_sended]=Wire.read();
      
    }
  }

}

/******************************************** END **************************************/
