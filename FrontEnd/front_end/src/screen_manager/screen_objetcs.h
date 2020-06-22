
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

#ifndef SCREEN_OBJECTS_H
#define SCREEN_OBJECTS_H


#include "../nextion/Nextion.h"




/*Nextion NOMBRE_OBJETO = Next_TIPO_OBJETO(PAGINA,ID,"NOMBRE_OBJETO")*/

NexNumber n1 = NexNumber(5, 13, "n1");
NexNumber n2 = NexNumber(5, 11, "n2");
NexNumber n3 = NexNumber(5, 12, "n3");
NexNumber n20 = NexNumber(5, 16, "n20");
NexNumber n4 = NexNumber(6, 9, "n4");
NexNumber n5 = NexNumber(6, 11, "n5");
NexNumber n6 = NexNumber(7, 8, "n6");
NexNumber n7 = NexNumber(7, 10, "n7");
NexNumber n8 = NexNumber(7, 11, "n8");
NexNumber n21 = NexNumber(7, 23, "n21");
NexNumber n9 = NexNumber(8, 8, "n9");
NexNumber n10 = NexNumber(8, 10, "n10");
NexNumber n26 = NexNumber(8, 24, "n26");
NexNumber n11 = NexNumber(9, 9, "n11");
NexNumber n12 = NexNumber(9, 8, "n12");
NexNumber n23 = NexNumber(9, 14, "n23");
NexNumber n13 = NexNumber(10, 10, "n13");
NexNumber n24 = NexNumber(10, 12, "n24");
NexNumber n25 = NexNumber(10, 16, "n25");
NexNumber n14 = NexNumber(11, 8, "n14");
NexNumber n15 = NexNumber(11, 9, "n15");
NexNumber n16 = NexNumber(11, 10, "n16");
NexNumber n17 = NexNumber(11, 11, "n17");
NexNumber n18 = NexNumber(12, 9, "n18");
NexNumber n19 = NexNumber(12, 10, "n19");
NexNumber n32 = NexNumber(20, 35, "n32");
NexNumber n33 = NexNumber(20, 36, "n33");
NexNumber n34 = NexNumber(20, 37, "n34");
NexNumber n35 = NexNumber(20, 38, "n35");

NexProgressBar j1 = NexProgressBar(20,2,"j1");

NexButton b2 = NexButton(1, 5, "b2");      // parameters Button
NexButton b3 = NexButton(1, 6, "b3");      // calibration Button
NexButton b5 = NexButton(1, 7, "b5");      // alarm config Button
NexButton b9 = NexButton(2, 8, "b9");      // Main menu Button
NexButton b11 = NexButton(3, 5, "b11");      // assistive ventilation Button
NexButton b12 = NexButton(3, 6, "b12");      // controlled ventilation Button
NexButton b14 = NexButton(14, 5, "b14");      // volume control Button
NexButton b15 = NexButton(15, 6, "b15");      // pressure control Button
NexButton b23 = NexButton(5, 14, "b23");      //  next page Button
NexButton b29 = NexButton(6, 19, "b29");      //begin ventilation
NexButton b37 = NexButton(7, 14, "b37");      //next page Button
NexButton b42 = NexButton(8, 12, "b42");      //begin ventilation
NexButton b75 = NexButton(9, 12, "b75");      //next page Button
NexButton b50 = NexButton(10, 7, "b50");      //begin ventilation
NexButton b61 = NexButton(11, 7, "b61");      //next page Button
NexButton b73 = NexButton(12, 8, "b73");      //save alarm Button
NexDSButton bt0 = NexDSButton(18,3,"bt0");
NexDSButton bt1 = NexDSButton(20,2,"bt1");
NexButton b69 = NexButton(18, 5, "b69");      //alarm page button
NexButton b67 = NexButton(18, 6, "b67");      //back page button
NexButton b70 = NexButton(19, 3, "b70");      //back page button


NexCheckbox c0 = NexCheckbox(6, 31, "c0");    // 1:2 I:E checkbox option
NexCheckbox c1 = NexCheckbox(6, 31, "c1");    // 1:3 I:E checkbox option
NexCheckbox c2 = NexCheckbox(6, 33, "c2");    // 1:4 I:E checkbox option
NexCheckbox c3 = NexCheckbox(6, 34, "c3");    // Male gender checkbox option
NexCheckbox c4 = NexCheckbox(6, 35, "c4");    // Female gender checkbox option
NexCheckbox c5 = NexCheckbox(8, 20, "c5");    // 1:2 I:E checkbox option
NexCheckbox c6 = NexCheckbox(8, 21, "c6");    // 1:3 I:E checkbox option
NexCheckbox c7 = NexCheckbox(8, 22, "c7");    // 1:4 I:E checkbox option
NexCheckbox c8 = NexCheckbox(8, 23, "c8");    // Male gender checkbox option
NexCheckbox c9 = NexCheckbox(8, 24, "c9");    // Female gender checkbox option
NexCheckbox c10 = NexCheckbox(10, 16, "c10"); // Male gender
NexCheckbox c11 = NexCheckbox(10, 17, "c11"); // Female gender


NexWaveform s0 = NexWaveform(20, 5, "s0");


NexText t30 = NexText(19,4,"t30");
NexText t31 = NexText(19,5,"t31");
NexText t32 = NexText(19,6,"t32");
NexText t33 = NexText(19,7,"t33");
NexText t34 = NexText(19,8,"t34");
NexText t35 = NexText(19,10,"t35");
NexText t36 = NexText(19,11,"t36");
NexText t37 = NexText(19,12,"t37");
NexText t38 = NexText(19,13,"t38");
NexText t39 = NexText(19,14,"t39");
NexText t40 = NexText(19,15,"t40");
NexText t41 = NexText(19,16,"t41");
NexText t42 = NexText(19,17,"t42");
NexText t43 = NexText(19,18,"t43");
NexText t44 = NexText(19,19,"t44");
NexText t45 = NexText(19,20,"t45");
NexText t46 = NexText(19,21,"t46");
NexText t47 = NexText(19,22,"t47");
NexText t48 = NexText(19,23,"t48");
NexText t49 = NexText(19,24,"t49");
NexText t50 = NexText(19,9,"t50");
NexText t51 = NexText(20,6,"t51");
NexText t52 = NexText(20,7,"t52");

NexTouch *nex_listen_list[] = 
{
  &b2,
  &b3,
  &b5,
  &b9,
  &b11,
  &b12,
  &b14,
  &b15,
  &b23,
  &b29,
  &b37,
  &b42,
  &b75,
  &b50,
  &b61,
  &b73,
  &bt0,
  &bt1,
  &b69,
  &b70,
  &b67,
  NULL  
};  



#endif /*SCREEN_OBJECTS_H*/