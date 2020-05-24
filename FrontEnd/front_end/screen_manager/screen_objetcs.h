#ifndef SCREEN_OBJECTS_H
#define SCREEN_OBJECTS_H


#include <Nextion.h>



//Objects window 1
NexButton b1 = NexButton(0, 9, "b1");       // breathing parameters Button
NexButton b2 = NexButton(0, 1, "b2");       // calibration Button 
NexButton b3 = NexButton(0, 11, "b3");      // system configuration Button 
NexButton b4 = NexButton(0, 11, "b4");      // about lung evolve Button 
NexButton b5 = NexButton(0, 11, "b5");      // help Button 
NexButton b6 = NexButton(0, 11, "b6");      // shut down/power off Button 
NexButton b7 = NexButton(0, 11, "b7");      // other options Button 

//Objets window 2


//Objets window 3


//Objets window 4



//Windows 
NexWindow window1 = NexWindow(0, 0, "window1");  // window1 as a touch event
NexWindow window2 = NexWindow(1, 0, "window2");  // window2 as a touch event
NexWindow window3 = NexWindow(1, 0, "window3");  // window3 as a touch event
NexWindow window4 = NexWindow(1, 0, "window4");  // window4 as a touch event





#endif /*SCREEN_OBJECTS_H*/