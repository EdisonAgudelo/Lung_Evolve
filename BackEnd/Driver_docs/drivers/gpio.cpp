#include "gpio.h"

void set_GPIO_High(uint8_t PIN){
  
  digitalWrite(PIN,HIGH);
}



void set_GPIO_Low(uint8_t PIN){
  
  digitalWrite(PIN,LOW);
}


bool read_GPIO(uint8_t PIN){
  return bitRead(PIN, OUTPUT);
  
}
