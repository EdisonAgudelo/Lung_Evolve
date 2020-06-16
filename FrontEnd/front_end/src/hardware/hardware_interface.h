 

#ifndef _HARDWARE_INTERFACE_H_
#define _HARDWARE_INTERFACE_H_


#include <stdint.h>



void UartConfigCallback(void (*callback)(void));
void UartBegin(uint32_t baudrate);
bool UartAvailable(void);
void UartWrite(uint8_t data);
uint8_t UartRead(void);



#endif