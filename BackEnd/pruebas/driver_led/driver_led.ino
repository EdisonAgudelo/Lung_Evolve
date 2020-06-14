#include "src/driver_led.h"
#include "src/time.h"

#define LED_PIN 13
#define CHANGE_TIME 10000

DriverLed led;

int mode = 0;
uint32_t time_ref;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN,OUTPUT);
  DriverLedInit(&led, LED_PIN);
  DriverLedBlink(&led, 500);
  time_ref=millis();
}

void loop() {

  if(GetDiffTime(millis(), time_ref)>=CHANGE_TIME){
    time_ref = millis();
    mode++;
    switch (mode)
    {
      case 0:
        DriverLedBlink(&led, 500);
      break;
  
      case 1:
        DriverLedNBlink(&led, 1000, 5);
      break;
  
      case 2:
        DriverLedTShoot(&led, 500, 1);
      break;
  
      case 3:
        DriverLedShoot(&led, 500);
        mode =-1;
      break;
  
      default:
      
      break;
    }
  }
  // put your main code here, to run repeatedly:
  DriverLedLoop(&led);
}
