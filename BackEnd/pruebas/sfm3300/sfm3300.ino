#include "src/driver_flowmeter.h"
#include "src/hardware_interface.h"
#include "src/time.h"

#define SFM3300_ADDR 64

Flowmeter sfm3300(kHardI2C, SFM3300_ADDR);
uint32_t ref_time=0;
uint32_t ref_time2=0;
uint32_t counts=0;

void setup() {
  // put your setup code here, to run once:
  sfm3300.Begin();
  Serial.begin(115200);
  ref_time = Millis();
}

void loop() {
  counts++;
  // put your main code here, to run repeatedly:
  sfm3300.Loop();
  if(GetDiffTime(Millis(),ref_time)>40)
  {
    ref_time=Millis();
    Serial.print(sfm3300.GetFlow());
    Serial.print(" ");
    Serial.println(sfm3300.Available());
  }
  
  if(GetDiffTime(Millis(),ref_time2)>1000)
  {
    ref_time2 = Millis();
    Serial.println(counts);
    counts=0;
  }
 

  
}
