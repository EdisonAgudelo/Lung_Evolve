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
NexNumber n81 = NexNumber(7, 23, "n81");
NexNumber n9 = NexNumber(8, 8, "n9");
NexNumber n10 = NexNumber(8, 10, "n10");
NexNumber n11 = NexNumber(9, 9, "n11");
NexNumber n12 = NexNumber(9, 8, "n12");
NexNumber n0 = NexNumber(9, 14, "n0");
NexNumber n82 = NexNumber(10, 10, "n82");
NexNumber n14 = NexNumber(11, 8, "n14");
NexNumber n15 = NexNumber(11, 9, "n15");
NexNumber n16 = NexNumber(11, 10, "n16");
NexNumber n17 = NexNumber(11, 11, "n17");
NexNumber n18 = NexNumber(12, 9, "n18");
NexNumber n19 = NexNumber(12, 10, "n19");



NexButton b9 = NexButton(2, 8, "b9");      // Main menu Button
NexButton b11 = NexButton(3, 5, "b11");      // assistive ventilation Button
NexButton b12 = NexButton(3, 6, "b12");      // controlled ventilation Button
NexButton b14 = NexButton(14, 5, "b14");      // volume control Button
NexButton b15 = NexButton(15, 6, "b15");      // pressure control Button
NexButton b23 = NexButton(5, 14, "b23");      //  next page Button
NexButton b29 = NexButton(6, 17, "b29");      //begin ventilation
NexButton b37 = NexButton(7, 14, "b37");      //next page Button
NexButton b42 = NexButton(8, 12, "b42");      //begin ventilation
NexButton b75 = NexButton(9, 12, "b75");      //next page Button
NexButton b50 = NexButton(10, 7, "b50");      //begin ventilation
NexButton b61 = NexButton(11, 7, "b61");      //next page Button
NexButton b73 = NexButton(12, 8, "b73");      //save alarm Button
NexDSButton bt0 = NexDSButton(18,3,"bt0");

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

//NexDSButton bt0 = NexDSButton(17, 3, "bt0");  // pause or not pause dual state Button





NexTouch *nex_listen_list[] = 
{
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
  NULL  
};  



#endif /*SCREEN_OBJECTS_H*/