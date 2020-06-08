
#ifndef _HICOP_H_
#define _HICOP_H_

#include <stdint.h>
#include <stdbool.h>

//hicop header types
typedef enum
{
    kHicopHeaderData = 0x0, //>> This header is for data streaming. Hicop protocol do not resend it in case of fail transfer
    kHicopHeaderAlarm, //>> This header is for data streaming. Hicop protocol resend it in case of fail transfer. It has a limited attempts before discart it 
    kHicopHeaderConfig, //>> This header is for config high level user parameters. Hicop protocol resend it in case of fail transfer. It has a limited attempts before discart it 
    kHicopHeaderFlags //>> Hicop protocol flags. User must use it under his responsability. 
} HicopHeaders;
//kHicopHeaderAlarm and kHicopHeaderConfig have the same behaviour. User can use this for specific application purpose


//hicop available flags
typedef enum
{
    kHicopNack = 0x0,
    kHicopAck
} HicopFlags;

//this function checks if other mcu has sent data. In case of available data, function 
//transffers those data to user buffer. Otherwise, return false.
bool HicopReadData(HicopHeaders *type, uint8_t *data, uint8_t *lengh);

//This function takes user data and saves it into internal buffer to send to the other mcu. 
//It allows user to accomulate a limited consecutive data for subsequent transffers.
//Funtion returns true if there are space in FIFO buffer else returns false.
bool HicopSendData(HicopHeaders type, uint8_t *data, uint8_t lengh);

//this function should be called fast enough to respond to other MCU and to have great performance. 
//this make all Hicop transaction and hicop protocols.
void HicopLoop(void);

//this function makes hicop initialization procedure. Only should be called one time in fw beginings
void HicopInit(void);

#endif