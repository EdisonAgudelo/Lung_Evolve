#include "i2C_master.h"


/*
   init function has to be called in setup() function

*/

/************************************************************************************/

void MASTER_init(void){
   Wire.begin(MASTER_ADDRESS);
}
/************************************************************************************/
void function_send (uint8_t slave_address, uint8_t comando) {
  bool pos;
  int i;
  Wire.beginTransmission(slave_address);
  Wire.write(comando);                         // send data frame
  //Wire.endTransmission();                 // stop transmittingter
  command_sended = comando;
  if (command_sended == 0)
  {
    pos = i;
  }
      else
      {
        pos = command_sended * 2 + i;
      }

      //Wire.beginTransmission(SLAVE1_ADDRESS);
      for (i = 0; i < 2; i++) {

        Wire.write(BreathingParameters[pos] );                         // send data frame
      }
      Wire.endTransmission();                 // stop transmittingt
}
/************************************************************************************/


    void function_request(uint8_t slave_address) {
      int i;
      Wire.requestFrom(slave_address);                      // request SIZE_DATA bytes from slave device SLAVE_ADDRESS
      for (i = 10; i < 23; i++)
      {
        BreathingParameters.BUFFER[i] = Wire.read();
      }
    }

/******************************************** END **************************************/


    //
    //extern union CONF BreathingParameters;
    ////    extern struct MASTER_PARAMETER MASTER_parameter;
    ///************************************************************************************/
    //void MASTER_init(void){
    //  Wire.begin(MASTER_ADDRESS);
    //}
    //
    //
    ///************************************************************************************/
    //void MASTER_Write_Data(uint8_t SLAVE_ADDRESS,uint16_t DAT, uint8_t COM){
    //
    //         Wire.beginTransmission(SLAVE1_ADDRESS);
    //         Wire.write(COM );                          // send data frame
    //         Wire.endTransmission();                 // stop transmittingter
    //         command_sended=COM;
    //         Wire.beginTransmission(SLAVE1_ADDRESS);
    //         Wire.write(DAT );                          // send data frame
    //         Wire.endTransmission();                 // stop transmittingter
    //
    //  }
    //
    //
    //void MASTER_Write_Data(uint8_t SLAVE_ADDRESS,uint8_t DAT, uint8_t COM){
    //  {
    //
    //    for(i=0;i<13;i++)
    //    {
    //      for(i=0;i<1;i++)
    //      {
    //
    //      }
    //    }
    //
    //  }
    //
    //
    ///************************************************************************************/
    //void MASTER_Recieve_Data(uint8_t SLAVE_ADDRESS){
    //  int i;
    //
    //  Wire.requestFrom(SLAVE_ADDRESS, SIZE_DATA);                      // request SIZE_DATA bytes from slave device SLAVE_ADDRESS
    //  for(i=0;i<14;i++)
    //  {
    //    while(1<Wire.available())                                         // slave may send less than requested
    //    {
    //       BreathingParameters.BUFFER[command_sended]=Wire.read();
    //
    //    }
    //  }
    //
    //}
    //
    ///******************************************** END **************************************/
    //
    //
    //
    //
