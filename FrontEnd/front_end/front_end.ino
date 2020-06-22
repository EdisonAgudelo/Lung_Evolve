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

#include "src/screen_manager/screen_manager.h"
#include "src/hardware/hardware_front.h"
#include "src/alarm_manager/alarm_manager.h"
#include "src/backend_manager/backend_manager.h"
#include "src/screen_manager/time_s.h"



void setup()
{
  TimeVirtualISRBegin();
  //SFrontEnd.begin(9600);
  Serial.begin(9600);
  //Serial3.begin(115200);
  serial_screen_init();
  backend_init();
	init_hardware_front();
  init_screen_management();
  init_alarm_maganement();
  //TimeVirtualISRAdd(0,virtualCallbackISR,2000);
  //Serial.write(alarms_struct.low_breathing_rate);
  
}

void loop()
{
  
  screen_management();
  backend_management();
  alarm_management_loop();
  

}