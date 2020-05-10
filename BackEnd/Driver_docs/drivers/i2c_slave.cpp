
#include "i2c_slave.h"
#include <stdint.h>

/*
 * 
 * 
 * 
*/
/************************************************************************************/

enum STATE_RECIEVE_T STATE_RECIEVE;

void SLAVE_init(void)
{
  Wire.begin(SLAVE1_ADDRESS);                 // join i2c bus with address SLAVE_ADDRESS
  Wire.onReceive(function_recieve);           // master send data to slave
  Wire.onRequest(function_send_from_request); // master request data from slave
}

/************************************************************************************/
void function_recieve(int numBytes)
{
  uint8_t pos;
  switch (STATE_RECIEVE)
  {
  case kGET_COMMAND:
    while (1 < Wire.available())
    {
      command_sended = Wire.read(); //store the command sended by the master
    }
    STATE_RECIEVE = kGET_DATA1;
    break;
  case kGET_DATA1:
    if (command_sended == 0)
    {
      pos = 0;
    }
    else
    {
      pos = command_sended * 2;
    }
    BreathingParameters.BUFFER[pos] = Wire.read();
    STATE_RECIEVE = kGET_DATA2;
    break;
  case kGET_DATA2:
    if (command_sended == 0)
    {
      pos = 0;
    }
    else
    {
      pos = command_sended * 2 + 1;
    }
    BreathingParameters.BUFFER[pos] = Wire.read();
    STATE_RECIEVE = kGET_COMMAND;
    buffer_update = 1;
    break;
  default:
    STATE_RECIEVE = kGET_COMMAND;
    break;
  }
}
/************************************************************************************/

void function_send_from_request(void)
{
  int i;
  for (i = 10; i < 23; i++)
  {
    Wire.write(BreathingParameters.BUFFER[i]);
  }
}
/******************************************** END **************************************/

//
//extern union CONF BreathingParameters;
///************************************************************************************/
//void SLAVE_init(void){
//  Wire.begin(SLAVE1_ADDRESS);                     // join i2c bus with address SLAVE_ADDRESS
//  Wire.onReceive(SLAVE_Receive_Data);             // master send data to slave
//  Wire.onRequest(SLAVE_Write_Data);               // master request data from slave
//
//}

///************************************************************************************/
//void SLAVE_Write_Data(void){
//  int i;
//
//  for(i=0;14;i++)                   //i<(sizeof(BreathingParameters)/sizeof(BreathingParametes.BUFFER[0]))
//  {
//    Wire.write(BreathingParameters.BUFFER[i], SIZE_DATA);
//    delayMicroseconds(70);
//  }
//}
///*
//  switch(STATE_REQUEST){
//    case kGET_COMMANDq:
//        command_sended=Wire.read();
//    case:kSEND_COMMANDq:
//    default:
//  }
//
//   while(1<Wire.available())
//   {
//      Wire.write(BreathingParameters.BUFFER[command_sended], SIZE_DATA);
//   }
//
//  */
//
//
///************************************************************************************/
//void SLAVE_Receive_Data(int numBytes){
//
//    switch(STATE_RECIEVE){
//    case kGET_COMMAND:
//        while(1<Wire.available()){
//         command_sended=Wire.read();     //store the command sended by the master
//        }
//         STATE_RECIEVE=  kSEND_COMMAND;
//         break;
//    case kSEND_COMMAND:
//        while(1<Wire.available()){
//         BreathingParameters.BUFFER[command_sended]=Wire.read();
//        }
//         STATE_RECIEVE= kGET_COMMAND;
//         break;
//    default:
//        STATE_RECIEVE= kGET_COMMAND;
//        break;
//
//  }
//}
//
//
///******************************************** END **************************************/
//
//
//
