
#include "hardware_interface.h"
#include <Arduino.h>

void (*uart_callback)(void)=nullptr;


////////// Serial interface ////////
void UartConfigCallback(void (*callback)(void))
{
  uart_callback = callback;
}

void serialEvent() {
  if(uart_callback!=nullptr)
    uart_callback();
}

void UartBegin(uint32_t baudrate)
{
  Serial.begin(baudrate);
}

void UartWrite(uint8_t data)
{
  Serial.write(data);
}

uint8_t UartRead(void)
{
  return Serial.read();
}

bool UartAvailable(void){
  return Serial.available();
}



