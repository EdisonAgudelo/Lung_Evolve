/*
    Lung Evolve Mechanical Ventilator
    Copyright (C) 2020  Edison Agudelo, Mateo Garcia, Alejandra Londoño

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see https://www.gnu.org/licenses/gpl-3.0.html.
    
    */
   
#include "crc8.h"

#define PIN_ANALOG_FLOW A2
#define PIN_ANALOG_TEMP A1

const uint32_t kSoftResetTime = 80; //ms

const uint8_t kI2CAddr = 64;

const uint16_t kFlowCommandStarFlowMeasure = 0x1000;
const uint16_t kFlowCommandStarTempMeasure = 0x1001;
const uint16_t kFlowCommandReadScaleFactor = 0x30de;
const uint16_t kFlowCommandReadOffset = 0x30df;
const uint16_t kFlowCommandSoftReset = 0x2000;

const uint16_t kScaleFactor = 120;
const uint16_t kOffesetFlow = 32768;

volatile uint16_t command = kFlowCommandSoftReset;
volatile uint32_t command_time = 0;

uint32_t keep_alive_time = 0;

uint16_t temp=0;
uint16_t flow=0xffff/2;


#include <Wire.h>

void setup()
{
 Wire.begin(kI2CAddr);                
 Wire.onRequest(requestEvent);
 Wire.onReceive(receiveEvent);
 Serial.begin(115200);          
 CRC8Configure(0x31,0x0);
 command_time = millis();
 pinMode(PIN_ANALOG_FLOW, INPUT);
 pinMode(PIN_ANALOG_TEMP, INPUT);
 keep_alive_time = millis();

}

void loop()
{
  temp = (uint16_t)analogRead(PIN_ANALOG_TEMP)*(uint16_t)16;
  flow = (uint16_t)analogRead(PIN_ANALOG_FLOW)*(uint16_t)16;
  if(command == kFlowCommandSoftReset)
    if((millis()-command_time)>kSoftResetTime)
      command = 0;

  if((millis()-keep_alive_time)>1000)
  {
    keep_alive_time = millis();
    Serial.println("alive");
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent()
{
 Serial.println("Respondiendo");
 switch(command)
 {
  case kFlowCommandStarFlowMeasure:
     flow<<=2;
     Wire.write((uint8_t)(flow>>8));
     Wire.write((uint8_t)(flow&0xff));
     Wire.write(CRC8Calculate(flow));
     flow>>=2;
     break;
  case kFlowCommandStarTempMeasure:
     temp<<=2;
     Wire.write((uint8_t)(temp>>8));
     Wire.write((uint8_t)(temp&0xff));
     Wire.write(CRC8Calculate(temp));
     temp>>2;
     break;
  case kFlowCommandReadScaleFactor:
     Wire.write((uint8_t)(kScaleFactor>>8));
     Wire.write((uint8_t)(kScaleFactor&0xff));
     Wire.write(CRC8Calculate(kScaleFactor));
     break;
  case kFlowCommandReadOffset:
     Wire.write((uint8_t)(kOffesetFlow>>8));
     Wire.write((uint8_t)(kOffesetFlow&0xff));
     Wire.write(CRC8Calculate(kOffesetFlow));
     break;
  case kFlowCommandSoftReset:
    Serial.println("Error: System booting2");
    goto error_code;
  default:
    Serial.println("Error: no prev comand");
     error_code:
     Wire.write(0xff);
     Wire.write(0xff);
     Wire.write(0xff);
     break;
 }
}


void receiveEvent(int len)
{
  if(len>2){
    Serial.println("len error");
    while(Wire.available())Wire.read(); //clean reception buffer
  }
  else
    {
      if(command != kFlowCommandSoftReset){
        command_time = millis();
        command =((uint16_t) Wire.read())<<8;    // receive command
        command +=((uint16_t) Wire.read());
        Serial.print("Comando: ");
        Serial.println(command);         // print comand
        }
       else{
         Serial.println("Error: System booting");
         while(Wire.available())Wire.read(); //clean reception buffer
       }
    }
}
