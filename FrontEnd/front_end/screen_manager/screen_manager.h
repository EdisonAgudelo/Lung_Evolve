#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H
#include "screen_objetcs.h"

enum ScreenStates
{
    kpage0,
    kpage1,
    kpage2,
    kpage3,
    kpage4,
    kpage5,
    kpage6,
    kpage7,
    kpage8,
    kpage9,
    kpage10,
    kpage11,
    kpage12,
    kpage13,
    kpage14,
    kpage15,
    kpage16,
    kpage17,
    kpage18,
    kpage19,
    kpage20
    

};

NexTouch *nex_listen_list[] = 
{
  &b1,  
  &b2,
  &b3, 
  &b4,
  &b5,
  &b6,
  &b7,    
  &page1,  
  &page2,  
  NULL  
};  


void b1PushCallback(void *ptr);
void b1PopCallback(void *ptr);
void b0PushCallback(void *ptr);
void handlers (void);
void screen_management(void);
#endif /*SCREEN_MANAGER_H*/