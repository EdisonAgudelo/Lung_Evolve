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
   
#define PIN_EN 7
#define PIN_DIR 8
#define PIN_STEP 2

uint32_t time_ref;
int32_t cuentas=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_EN, INPUT);
  pinMode(PIN_STEP, INPUT);
  pinMode(PIN_DIR, INPUT);
  Serial.begin(115200);
  time_ref = millis();
  attachInterrupt(digitalPinToInterrupt(PIN_STEP), count, RISING);
}

void loop() {

  if(!digitalRead(PIN_EN))
  {
    if(millis()-time_ref>1000)
    {
      time_ref = millis();
      Serial.println(cuentas);
    }
  }
}

void count(void)
{
  cuentas+=digitalRead(PIN_DIR)?1:-1;
}
