
#ifndef _HICOP_CONFIG_H_
#define _HICOP_CONFIG_H_

//high layer


#define HICOP_MAX_DATA_LENGTH 250 // don't move, but it talks about maximum transmission lenght
#define HICOP_STACK_SIZE 2  //FIFO length 
#define HICOP_MAX_RESPONSE_TIME 500 //ms. Which is other mcu maximum response time
#define HICOP_MAX_SEND_TRIES 10    //how many times hicop protocol will try to resend a failed transmission


//lower layer


#define HICOP_LL_BUFFER_LENGTH 255
#define HICOP_LL_SERIAL_BAUDRATE 115200
#define HICOP_LL_INVALID_HEADER 0xff
#define HICOP_LL_MAX_RX_TIME 100 //mss
#define HICOP_LL_CRC8_POLY 0x31
#define HICOP_LL_CRC8_INIT 0x00

#endif