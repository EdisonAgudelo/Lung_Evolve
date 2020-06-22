
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

#ifndef BACKEND_MANAGER_H
#define BACKEND_MANAGER_H

#include "../data_bus/data_bus.h" /////////revisar
//#include "../hardware/hardware_front.h"
#include "../alarm_manager/alarm_manager.h"
#include "../screen_manager/screen_manager.h"
#include "crc8.h"


 

void backend_init(void);

void backend_management(void);


#endif /*BACKEND_MANAGER_H*/