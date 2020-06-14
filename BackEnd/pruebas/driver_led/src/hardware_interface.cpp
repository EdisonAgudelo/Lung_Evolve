
 
  #include "hardware_interface.h"
 
 #include <Arduino.h>
 
 void PinSetDigital(int pin, bool level) 
 {
	digitalWrite(pin, level);
 }