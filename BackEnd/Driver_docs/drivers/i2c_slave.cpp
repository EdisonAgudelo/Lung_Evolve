
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
void SLAVE_Write_Data(uint16_t DAT){
    //uint8_t SIZE= LENGTH_ARRAY(DAT_FRAME);
    
   // for(int i=0;i<SIZE;i++)
   //{
   while(1<Wire.available())
   {
      Wire.write(DAT_FRAME[i]);
   }
   
   //}
  
}

/************************************************************************************/
void SLAVE_Receive_Data(int numBytes){
    //static struct SLAVE_PARAMETER SLAVE_parameter;
    //SLAVE_parameter.rx_SIZE_DAT=numBytes;
    //static uint16_t recived;
   int i=0;
   while(1<Wire.available())
   {
     if(is_command_send=!1)             // checks if a command has already been sent
     {
        command_sended=Wire.read();     //store the command sended by the master
        is_command_sent=1;
     }
     else
     {
      BreathingParameters.BUFFER[command_sended]=Wire.read();
      is_command_sent=0;
     }
   }
    

}






//
//
//
//
//
//
///*****************************/
////slave reciever
//
//#include <Wire.h>
//
//void setup()
//{
//  Wire.begin(4);                // join i2c bus with address #4
//  Wire.onReceive(receiveEvent); // register event
//  Serial.begin(9600);           // start serial for output
//}
//
//void loop()
//{
//  delay(100);
//}
//
//// function that executes whenever data is received from master
//// this function is registered as an event, see setup()
//void receiveEvent(int howMany)
//{
//  while(1 < Wire.available()) // loop through all but the last
//  {
//    char c = Wire.read(); // receive byte as a character
//    Serial.print(c);         // print the character
//  }
//  int x = Wire.read();    // receive byte as an integer
//  Serial.println(x);         // print the integer
//}
//
//
///***********************************************************/
