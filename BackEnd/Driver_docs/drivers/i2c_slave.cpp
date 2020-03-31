
#include "i2c_slave.h"

/*
 * 
 * 
 * 
*/


extern union BreathingParameters;
/************************************************************************************/
void SLAVE_init(void){
  Wire.begin(SLAVE1_ADDRESS);                     // join i2c bus with address SLAVE_ADDRESS
  Wire.onReceive(SLAVE_Receive_Data);             // master send data to slave   
  Wire.onRequest(SLAVE_Write_Data);               // master request data from slave
  
}

/************************************************************************************/
void SLAVE_Write_Data(void){

  for(i=0;i<(sizeof(BreathingParameters)/sizeof(BreathingParametes.BUFFER[0]);i++)
  {
    Wire.write(BreathingParameters.BUFFER[i], SIZE_DATA);
    delayMicroseconds(70);
  }
}
/*
  switch(STATE_REQUEST){
    case kGET_COMMANDq:
        command_sended=Wire.read();
    case:kSEND_COMMANDq:
    default:
  }

   while(1<Wire.available())
   {
      Wire.write(BreathingParameters.BUFFER[command_sended], SIZE_DATA);
   }
   
  */


/************************************************************************************/
void SLAVE_Receive_Data(int numBytes){

    switch(STATE_RECIEVE){
    case kGET_COMMAND:
        while(1<Wire.available()){
         command_sended=Wire.read();     //store the command sended by the master
        }
         STATE_RECIEVE=kSEND_COMMAND;
         break;
    case:kSEND_COMMAND:
        while(1<Wire.available()){
         BreathingParameters.BUFFER[command_sended]=Wire.read();
        }
         STATE_RECIEVE=kGET_COMMAND;
         break;
    default:
        STATE_RECIEVE=kGET_COMMAND;
        break; 

  }
}


/******************************************** END **************************************/
