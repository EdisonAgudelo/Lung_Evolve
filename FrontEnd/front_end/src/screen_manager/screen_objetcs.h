#ifndef SCREEN_OBJECTS_H
#define SCREEN_OBJECTS_H


#include <Nextion.h>




/*Nextion NOMBRE_OBJETO = Next_TIPO_OBJETO(PAGINA,ID,"NOMBRE_OBJETO")*/

//Objects window 0//////////////////////////////////////////////
NexButton b1 = NexButton(0, 1, "b1");      // Main menu Button

//Objects window 1//////////////////////////////////////////////
NexButton b2 = NexButton(1, 2, "b2");      // breathing parameters Button
NexButton b3 = NexButton(1, 3, "b3");      // calibration Button  
NexButton b4 = NexButton(1, 4, "b4");      // system configuration Button
NexButton b5 = NexButton(1, 5, "b5");      // Alarm configuration Button   
NexButton b6 = NexButton(1, 6, "b6");      // Next page button
 

//Objets window 2//////////////////////////////////////////////
NexButton b7 = NexButton(2, 7, "b7");      // about lung evolve Button 
NexButton b8 = NexButton(2, 8, "b8");      // help Button
NexButton b9 = NexButton(2, 9, "b9");    // shut down/power off Button  
NexButton b10 = NexButton(2,10 , "b10");    // previous page Button

//Objets window 3//////////////////////////////////////////////

NexButton b11 = NexButton(3, 11, "b11");      //  assistive Button
NexButton b12 = NexButton(3, 12, "b12");      //  controlled Button
NexButton b13 = NexButton(3, 13, "b13");      //  previous page Button

//Objets window 4//////////////////////////////////////////////

NexButton b14 = NexButton(4, 14, "b14");      //  volume control Button
NexButton b15 = NexButton(4, 15, "b15");      //  pressure control Button
NexButton b16 = NexButton(4, 16, "b16");      //  previous page Button


//Objets window 5//////////////////////////////////////////////
//NexButton b17 = NexButton(5, 17, "b17");      //  FIO2 increment Button
//NexButton b18 = NexButton(5, 18, "b18");      //  FIO2 decrement Button
//NexButton b19 = NexButton(5, 19, "b19");      //  BPM increment Button
//NexButton b20 = NexButton(5, 20, "b20");      //  BPM decrement Button
//NexButton b21 = NexButton(5, 21, "b21");      //  PEEP increment Button
//NexButton b22 = NexButton(5, 22, "b22");      //  PEEP decrement Button
NexButton b23 = NexButton(5, 23, "b23");      //  next page Button
NexButton b24 = NexButton(5, 24, "b24");      //  previous page Button



//Objets window 6//////////////////////////////////////////////
//NexButton b25 = NexButton(6, 25, "b25");      //  Heith increment Button
//NexButton b26 = NexButton(6, 26, "b26");      //  Heith decrement Button
//NexButton b27 = NexButton(6, 27, "b27");      //  apnea time increment Button
//NexButton b28 = NexButton(6, 28, "b28");      //  apnea time decrement Button
NexButton b29 = NexButton(6, 29, "b29");      //  begin ventilation Button
NexButton b30 = NexButton(6, 30, "b30");      //  previous page Button


//NexCheckbox c0 = NexCheckbox(6, 31, "c0");    // 1:2 I:E checkbox option
//NexCheckbox c1 = NexCheckbox(6, 31, "c1");    // 1:3 I:E checkbox option
//NexCheckbox c2 = NexCheckbox(6, 33, "c2");    // 1:4 I:E checkbox option
//NexCheckbox c3 = NexCheckbox(6, 34, "c3");    // Male gender checkbox option
//NexCheckbox c4 = NexCheckbox(6, 35, "c4");    // Female gender checkbox option

//Objects window 7//////////////////////////////////////////////
//NexButton b31 = NexButton(7, 36, "b31");      //  FIO2 increment Button
//NexButton b32 = NexButton(7, 37, "b32");      //  FIO2 decrement Button
//NexButton b33 = NexButton(7, 38, "b33");      //  BPM increment Button
//NexButton b34 = NexButton(7, 39, "b34");      //  BPM decrement Button
//NexButton b35 = NexButton(7, 40, "b35");      //  PEEP increment Button
//NexButton b36 = NexButton(7, 41, "b36");      //  PEEP decrement Button
NexButton b37 = NexButton(7, 42, "b37");      //  next page Button
NexButton b38 = NexButton(7, 43, "b38");      //  previous page Button



//Objects window 8//////////////////////////////////////////////

//NexButton b72 = NexButton(8, 44, "b38");      //  pressure increment Button
//NexButton b39 = NexButton(8, 45, "b39");      //  pressure decrement Button
//NexButton b40 = NexButton(8, 46, "b40");      //  apnea time increment Button
//NexButton b41 = NexButton(8, 47, "b41");      //  apnea time decrement Button
NexButton b42 = NexButton(8, 48, "b42");      //  begin ventilation Button
NexButton b43 = NexButton(8, 49, "b43");      //  previous page Button


//NexCheckbox c5 = NexCheckbox(8, 50, "c5");    // 1:2 I:E checkbox option
//NexCheckbox c6 = NexCheckbox(8, 51, "c6");    // 1:3 I:E checkbox option
//NexCheckbox c7 = NexCheckbox(8, 52, "c7");    // 1:4 I:E checkbox option
//NexCheckbox c8 = NexCheckbox(8, 53, "c8");    // Male gender checkbox option
//NexCheckbox c9 = NexCheckbox(8, 54, "c9");    // Female gender checkbox option

//Objects window 9//////////////////////////////////////////////

//NexButton b44 = NexButton(9, 55, "b44");      //  FIO2 increment Button
//NexButton b45 = NexButton(9, 56, "b45");      //  FIO2 decrement Button
//NexButton b46 = NexButton(9, 57, "b46");      //  PEEP increment Button
//NexButton b47 = NexButton(9, 58, "b47");      //  PEEP decrement Button
//NexButton b48 = NexButton(9, 59, "b48");      //  Heith increment Button
//NexButton b49 = NexButton(9, 60, "b49");      //  Heith decrement Button
NexButton b50 = NexButton(9, 61, "b50");      //  begin ventilation Button
NexButton b51 = NexButton(9, 62, "b51");      //  previous page Button


//NexCheckbox c10 = NexCheckbox(9, 63, "c10");  // Male gender checkbox option
//NexCheckbox c11 = NexCheckbox(9, 64, "c11");  // Female gender checkbox option


//Objects window 10//////////////////////////////////////////////

//NexButton b52 = NexButton(10, 65, "b52");      //  max input pressure increment Button
//NexButton b53 = NexButton(10, 66, "b53");      //  max input pressure decrement Button
//NexButton b54 = NexButton(10, 67, "b54");      //  min input pressure increment Button
//NexButton b55 = NexButton(10, 68, "b55");      //  min input pressure decrement Button
//NexButton b56 = NexButton(10, 69, "b56");      //  max output pressure increment Button
//NexButton b57 = NexButton(10, 70, "b57");      //  max output pressure decrement Button
//NexButton b58 = NexButton(10, 71, "b58");      //  min output pressure increment Button
//NexButton b59 = NexButton(10, 72, "b59");      //  min output pressure decrement Button
NexButton b60 = NexButton(10, 73, "b60");      //  previous page Button
NexButton b61 = NexButton(10, 74, "b61");      //  next page Button

//Objects window 11//////////////////////////////////////////////

//NexButton b62 = NexButton(11, 75, "b62");      //  max tidal volume increment Button
//NexButton b63 = NexButton(11, 76, "b63");      //  max tidal volume decrement Button
//NexButton b64 = NexButton(11, 77, "b64");      //  min tidal volume increment Button
//NexButton b65 = NexButton(11, 78, "b65");      //  min tidal volume decrement Button
NexButton b66 = NexButton(11, 79, "b66");      //  previous page Button
NexButton b73 = NexButton(11, 80, "b73");      //  save alarm configuration

//Objects window 12//////////////////////////////////////////////

//Objects window 13//////////////////////////////////////////////

//Objects window 14//////////////////////////////////////////////

//Objects window 15//////////////////////////////////////////////

//Objects window 16//////////////////////////////////////////////

//Objects window 17//////////////////////////////////////////////
NexButton b67 = NexButton(17, 81, "b67");      //  previous page Button
NexButton b68 = NexButton(17, 82, "b68");      //  Main menu Button
NexButton b69 = NexButton(17, 83, "b69");      //  alarm state page Button

NexDSButton bt0 = NexDSButton(17, 84, "bt0");  // pause or not pause dual state Button

//Objects window 18//////////////////////////////////////////////

NexButton b70 = NexButton(18, 85, "b70");      //  previous page Button

//Objects window 19//////////////////////////////////////////////
NexDSButton bt1 = NexDSButton(19,86 , "bt1");  //  silence or not silence sound alarms Button
NexButton b71 = NexButton(19, 87, "b71");      //  other opt Button



NexTouch *nex_listen_list[] = 
{
  &b1,
  &b2,
  &b3,
  &b4,
  &b5,
  &b6,
  &b7,
  &b8,
  &b9,
  &b10,
  &b11,
  &b12,
  &b13,
  &b14,
  &b15,
  &b16,
  &b23,
  &b24,
  &b29,
  &b30,
  &b37,
  &b38,
  &b42,
  &b43,
  &b50,
  &b51,
  &b60,
  &b61,
  &b66,
  &b67,
  &b68,
  &b69,
  &b70,  
  &b71,
  &b73,
  &bt0,
  &bt1, 
  NULL  
};  



#endif /*SCREEN_OBJECTS_H*/