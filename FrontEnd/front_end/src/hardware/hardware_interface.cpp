
#include "hardware_interface.h"
#include <Arduino.h>

void (*uart_callback)(void)=nullptr;


////////// Serial interface ////////
void UartConfigCallback(void (*callback)(void))
{
  uart_callback = callback;
}

void serialEvent1() {
  if(uart_callback!=nullptr)
    uart_callback();
}

void UartBegin(uint32_t baudrate)
{
  Serial1.begin(baudrate);
}

void UartWrite(uint8_t data)
{
  Serial1.write(data);
}

uint8_t UartRead(void)
{
  return Serial1.read();
}

bool UartAvailable(void){
  return Serial1.available();
}



