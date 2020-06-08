#include "src/screen_manager/screen_manager.h"
#include "src/hardware/hardware_front.h"
#include "src/alarm_manager/alarm_manager.h"
//#include "src/backend_manager/backend_manager.h"
#include <SoftwareSerial.h>
//SoftwareSerial SFrontEnd (7,6);


void setup()
{
  //SFrontEnd.begin(9600);
  Serial.begin(9600);
  serial_screen_init();
  //backend_init();
	init_hardware_front();
  init_screen_management();
}
byte b;
void loop()
{
  if(Serial2.available())
  {
    //Serial.print("pantalla envió algo");
    //b=Serial2.peek();
    b=Serial2.read();
    Serial.write(b);
  }
  screen_management();
  //backend_management();
  alarm_management_loop();
}