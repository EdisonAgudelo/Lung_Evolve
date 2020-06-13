#include "src/Hicop/hicop.h"

HicopHeaders test_type = kHicopHeaderAlarm;
uint8_t data[] = {0x19,0x1};
uint8_t len = sizeof(data);

void setup()
{
  
  HicopInit();
  delay(1000);
  HicopSendData(test_type,data,len);
}

void loop()
{
  HicopLoop();
 // DriverLoops();
}
