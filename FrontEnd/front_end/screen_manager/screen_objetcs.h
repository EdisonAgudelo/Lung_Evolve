#ifndef SCREEN_OBJECTS_H
#define SCREEN_OBJECTS_H


#include <Nextion.h>


/*Nextion NOMBRE_OBJETO = Next_TIPO_OBJETO(PAGINA,ID,"NOMBRE_OBJETO")*/

//Objects window 0//////////////////////////////////////////////
NexButton b1 = NexButton(0, , "b1");      // Main menu Button

//Objects window 1//////////////////////////////////////////////
NexButton b2 = NexButton(1, , "b2");      // breathing parameters Button
NexButton b3 = NexButton(1, , "b3");      // calibration Button  
NexButton b4 = NexButton(1, , "b4");      // system configuration Button
NexButton b5 = NexButton(1, , "b5");      // Alarm configuration Button   
NexButton b6 = NexButton(1, , "b6");      // Next page button
 

//Objets window 2//////////////////////////////////////////////
NexButton b7 = NexButton(2, , "b7");      // about lung evolve Button 
NexButton b8 = NexButton(2, , "b8");      // help Button
NexButton b9 = NexButton(2, , "b9");    // shut down/power off Button  
NexButton b10 = NexButton(2, , "b10");    // previous page Button

//Objets window 3//////////////////////////////////////////////

NexButton b11 = NexButton(3, , "b11");      //  assistive Button
NexButton b12 = NexButton(3, , "b12");      //  controlled Button
NexButton b13 = NexButton(3, , "b13");      //  previous page Button

//Objets window 4//////////////////////////////////////////////

NexButton b14 = NexButton(4, , "b14");      //  volume control Button
NexButton b15 = NexButton(4, , "b15");      //  pressure control Button
NexButton b16 = NexButton(4, , "b16");      //  previous page Button


//Objets window 5//////////////////////////////////////////////
NexButton b17 = NexButton(5, , "b17");      //  FIO2 increment Button
NexButton b18 = NexButton(5, , "b18");      //  FIO2 decrement Button
NexButton b19 = NexButton(5, , "b19");      //  BPM increment Button
NexButton b20 = NexButton(5, , "b20");      //  BPM decrement Button
NexButton b21 = NexButton(5, , "b21");      //  PEEP increment Button
NexButton b22 = NexButton(5, , "b22");      //  PEEP decrement Button
NexButton b23 = NexButton(5, , "b23");      //  next page Button
NexButton b24 = NexButton(5, , "b24");      //  previous page Button



//Objets window 6//////////////////////////////////////////////
NexButton b25 = NexButton(6, , "b25");      //  Heith increment Button
NexButton b26 = NexButton(6, , "b26");      //  Heith decrement Button
NexButton b27 = NexButton(6, , "b27");      //  apnea time increment Button
NexButton b28 = NexButton(6, , "b28");      //  apnea time decrement Button
NexButton b29 = NexButton(6, , "b29");      //  begin ventilation Button
NexButton b30 = NexButton(6, , "b30");      //  previous page Button


NexCheckbox c0 = NexCheckbox(6, , "c0");    // 1:2 I:E checkbox option
NexCheckbox c1 = NexCheckbox(6, , "c1");    // 1:3 I:E checkbox option
NexCheckbox c2 = NexCheckbox(6, , "c2");    // 1:4 I:E checkbox option
NexCheckbox c3 = NexCheckbox(6, , "c3");    // Male gender checkbox option
NexCheckbox c4 = NexCheckbox(6, , "c4");    // Female gender checkbox option

//Objects window 7//////////////////////////////////////////////
NexButton b31 = NexButton(7, , "b31");      //  FIO2 increment Button
NexButton b32 = NexButton(7, , "b32");      //  FIO2 decrement Button
NexButton b33 = NexButton(7, , "b33");      //  BPM increment Button
NexButton b34 = NexButton(7, , "b34");      //  BPM decrement Button
NexButton b35 = NexButton(7, , "b35");      //  PEEP increment Button
NexButton b36 = NexButton(7, , "b36");      //  PEEP decrement Button
NexButton b37 = NexButton(7, , "b37");      //  next page Button
NexButton b38 = NexButton(7, , "b38");      //  previous page Button



//Objects window 8//////////////////////////////////////////////

NexButton b38 = NexButton(8, , "b38");      //  pressure increment Button
NexButton b39 = NexButton(8, , "b39");      //  pressure decrement Button
NexButton b40 = NexButton(8, , "b40");      //  apnea time increment Button
NexButton b41 = NexButton(8, , "b41");      //  apnea time decrement Button
NexButton b42 = NexButton(8, , "b42");      //  begin ventilation Button
NexButton b43 = NexButton(8, , "b43");      //  previous page Button


NexCheckbox c5 = NexCheckbox(8, , "c5");    // 1:2 I:E checkbox option
NexCheckbox c6 = NexCheckbox(8, , "c6");    // 1:3 I:E checkbox option
NexCheckbox c7 = NexCheckbox(8, , "c7");    // 1:4 I:E checkbox option
NexCheckbox c8 = NexCheckbox(8, , "c8");    // Male gender checkbox option
NexCheckbox c9 = NexCheckbox(8, , "c9");    // Female gender checkbox option

//Objects window 9//////////////////////////////////////////////

NexButton b44 = NexButton(9, , "b44");      //  FIO2 increment Button
NexButton b45 = NexButton(9, , "b45");      //  FIO2 decrement Button
NexButton b46 = NexButton(9, , "b46");      //  PEEP increment Button
NexButton b47 = NexButton(9, , "b47");      //  PEEP decrement Button
NexButton b48 = NexButton(9, , "b48");      //  Heith increment Button
NexButton b49 = NexButton(9, , "b49");      //  Heith decrement Button
NexButton b50 = NexButton(9, , "b50");      //  begin ventilation Button
NexButton b51 = NexButton(9, , "b51");      //  previous page Button


NexCheckbox c10 = NexCheckbox(9, , "c10");  // Male gender checkbox option
NexCheckbox c11 = NexCheckbox(9, , "c11");  // Female gender checkbox option


//Objects window 10//////////////////////////////////////////////

NexButton b52 = NexButton(10, , "b52");      //  max input pressure increment Button
NexButton b53 = NexButton(10, , "b53");      //  max input pressure decrement Button
NexButton b54 = NexButton(10, , "b54");      //  min input pressure increment Button
NexButton b55 = NexButton(10, , "b55");      //  min input pressure decrement Button
NexButton b56 = NexButton(10, , "b56");      //  max output pressure increment Button
NexButton b57 = NexButton(10, , "b57");      //  max output pressure decrement Button
NexButton b58 = NexButton(10, , "b58");      //  min output pressure increment Button
NexButton b59 = NexButton(10, , "b59");      //  min output pressure decrement Button
NexButton b60 = NexButton(10, , "b60");      //  previous page Button
NexButton b61 = NexButton(10, , "b61");      //  next page Button

//Objects window 11//////////////////////////////////////////////

NexButton b62 = NexButton(11, , "b62");      //  max tidal volume increment Button
NexButton b63 = NexButton(11, , "b61");      //  max tidal volume decrement Button
NexButton b64 = NexButton(11, , "b62");      //  min tidal volume increment Button
NexButton b65 = NexButton(11, , "b63");      //  min tidal volume decrement Button
NexButton b66 = NexButton(11, , "b64");      //  previous page Button

//Objects window 12//////////////////////////////////////////////

//Objects window 13//////////////////////////////////////////////

//Objects window 14//////////////////////////////////////////////

//Objects window 15//////////////////////////////////////////////

//Objects window 16//////////////////////////////////////////////

//Objects window 17//////////////////////////////////////////////
NexButton b67 = NexButton(17, , "b67");      //  previous page Button
NexButton b68 = NexButton(17, , "b68");      //  Main menu Button
NexButton b69 = NexButton(17, , "b69");      //  alarm state page Button

NexDSButton bt0 = NexDSButton(17, , "bt0");  // pause or not pause dual state Button

//Objects window 18//////////////////////////////////////////////

NexButton b69 = NexButton(18, , "b69");      //  previous page Button

//Objects window 19//////////////////////////////////////////////
NexDSButton bt0 = NexDSButton(17, , "bt0");  //  silence or not silence sound alarms Button
NexButton b67 = NexButton(17, , "b67");      //  other opt Button




#endif /*SCREEN_OBJECTS_H*/