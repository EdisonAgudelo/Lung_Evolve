
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

#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "../nextion/Nextion.h"
//#include "screen_objetcs.h"
#include "../data_bus/data_bus.h"
#include "../alarm_manager/alarm_manager.h"


typedef struct 
{
  byte fio2;
  byte bpm;
  byte peep;
  byte heigh;
  byte apnea;
  byte ie;
  byte gender;
  byte pressure;

}buffer_screen;


typedef enum
{
  Kcontrolled=0,
  Kassistive,
  Kalarms

}ModeHeader;

typedef enum
{
  Kvolume1=0,
  Kvolume2,
  kpressure1,
  Kpressure2,
  Kassistive1,
  Kassistive2,
  kalarms1,
  Kalarms2

}TypeHeader;



#define NUM_BYTES1 18 //for volume control
//#define NUM_BYTES2  10//for pressure control
//#define NUM_BYTES3  11//for assistive control
//#define NUM_BYTES4  12//for alarm configuration

const int NUM_BYTES=18;

extern bool update;

extern bool alarmPage;

extern bool new_conf_screen;

//extern CONFIGURATION config;
//extern Alarm_state AlarmState;
//extern DATA dataValue;
//extern ALARMS alarms_data;
void serial_screen_init(void);
void init_screen_management(void);
void handlers (void);
void b2PushCallback(void *ptr);
void virtualCallbackISR(void);
void b3PushCallback(void *ptr);
void b5PushCallback(void *ptr);
void b9PushCallback(void *ptr);
void b11PushCallback(void *ptr);
void b12PushCallback(void *ptr);
void b14PushCallback(void *ptr);
void b15PushCallback(void *ptr);
void b23PushCallback(void *ptr);
void b29PushCallback(void *ptr);
void b37PushCallback(void *ptr);
void b42PushCallback(void *ptr);
void b75PushCallback(void *ptr);
void b50PushCallback(void *ptr);
void b61PushCallback(void *ptr);
void b73PushCallback(void *ptr);
void bt0PushCallback(void *ptr);
void bt0PopCallback(void *ptr);
void bt1PushCallback(void *ptr);
void bt1PopCallback(void *ptr);
void b69PushCallback(void *ptr);
void b70PushCallback(void *prt);
void b67PushCallback(void *ptr);
void screen_alarms(void);
void send_screen_alarm(void);
void init_mask (void);
void screen_revieve_data(void);
void screen_management(void);
#endif /*SCREEN_MANAGER_H*/