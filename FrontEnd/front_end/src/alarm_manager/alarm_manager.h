
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


#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H

#include "../data_bus/data_bus.h"
#include "../hardware/hardware_front.h"




typedef struct 
{
    bool HighAlarmState;
    bool MediumAlarmState;
    bool BuzzerOn;
    bool ScreenSoundOff;
    bool transition1;
    bool transition2;
    int prevstate1;
    int prevstate2;
} Alarm_state;

volatile extern Alarm_state AS;

//unsigned long start_counter;

void init_alarm_maganement(void) ;
void alarm_management_loop(void);
void check_state_alarm(void);
void alarm_action(void);


#endif /*ALARM_MANAGER_H*/