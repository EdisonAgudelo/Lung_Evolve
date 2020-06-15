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
