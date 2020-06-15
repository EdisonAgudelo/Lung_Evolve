
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

#include "src/Hicop/hicop.h"
#define PIN_LED 13


//This struct saves all Front end commands or information
typedef union {
  struct
  {
    //working mode options
    bool is_tunning;          //calibration
    bool is_standby;          //no breathing
    bool is_volume_controled; //control by volumen or pressure
    bool is_assisted;         //true: ventilator should wait for patient trigger or false: wait for time trigger

    //reference values
    bool trigger_source_pressure; //pressure true or flow false

    uint8_t trigger_value;  //1 ~ 3 slm or -3 ~ -5 cmh20
    uint8_t FiO2;           // 21 - 100 [%]
    uint8_t in_presure;     // 0 ~ 65 [cmH2O]
    uint16_t volume_tidal;  // 200 ~ 650 [mL]
    uint8_t breathing_rate; //6 ~ 40 [breaths/min]
    uint8_t ie_ratio;       //1:1 ~ 1:3

    uint32_t apnea_time; // 0 ~ 30000 [ms]
    uint32_t pause_time; // 0 ~ 30000 [ms]

    //warnings
    uint8_t maximun_in_pressure;   // 0 ~ 65 [cmH2O]
    uint8_t minimun_in_pressure;   // 0 ~ 65 [cmH2O]
    uint8_t maximun_out_pressure;  // 0 ~ 65 [cmH2O]
    uint8_t minimun_out_pressure;  // 0 ~ 65 [cmH2O]
    uint16_t maximun_volume_tidal; // 0 ~ 650  [mL]
    uint16_t minimun_volume_tidal; // 0 ~ 650  [mL]
    uint8_t minimun_peep;          // 0 ~ 20 [cmH2O]
    uint16_t maximum_leakage;      //0-600 [mL]
  };

  uint8_t all[31];

} BreathingParameters;

BreathingParameters test_param = {
  false, //tuning 
  false, //standby
  true,  //volume controled
  false, //is assisted
  true, //pressure as trigger source
  5,    //trugger value
  50,   //fio2
  18,   //in pressure
  500,  //tidal
  20,   //breathing rate
  2,   //ie_ratio
  200, //apnea
  100, //pause time
  25,  //max in pressure
  15,  //min in pressure
  25, //max out pressure
  15,  //min out pressure
  600, //max volume tidal
  400, //min volumen tidal,
  5, //minimum peep
  100 //maximum leakage
  };
  
HicopHeaders test_type;
uint8_t len ;
uint8_t transfer_buffer[100];

uint32_t ref_time;

void setup()
{
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED,LOW);
  HicopInit();
  delay(1000);
  HicopSendData(kHicopHeaderConfig,test_param.all, sizeof(test_param.all));
  ref_time = millis();
}

void loop()
{
  
  HicopLoop();
  if(HicopReadData(&test_type,transfer_buffer, &len ))
    digitalWrite(PIN_LED,HIGH);
  else
    digitalWrite(PIN_LED,LOW);


  if(millis()-ref_time>15000)
  {
    ref_time=millis();
    test_param.is_standby=!test_param.is_standby;
    HicopSendData(kHicopHeaderConfig,test_param.all,sizeof(test_param.all));
  }
  
}
