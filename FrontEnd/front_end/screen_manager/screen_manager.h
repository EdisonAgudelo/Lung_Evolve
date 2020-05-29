#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H
#include "screen_objetcs.h"
#include "data_bus/data_bus.h"
#include "alarm_manager/alarm_manager.h"

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

#define kfio2 0xc8
#define kbpm 0xc9
#define kpeep 0xca
#define kheigh 0xcb
#define kapnea 0xcc
#define kie 0xcd
#define kgender 0xce
#define kpressure 0xcf
#define kmaxInPressure 0xD0
#define kminInPressure 0xD1
#define kmaxOutPressure 0xD2
#define kminOutPressure 0xD3
#define kmaxVT 0xD4
#define kminTV 0xD5



#define NUM_BYTES1 19 //for volume control
#define NUM_BYTES2  //for pressure control
#define NUM_BYTES3  //for assistive control
#define NUM_BYTES4  //for alarm configuration

const int NUM_BYTES=18;



extern CONFIGURATION config;
extern Alarm_state AlarmState;
extern DATA dataValue;
extern ALARMS alarms_data;

void init_screen_management(void);
void handlers (void);
void b1PushCallback(void *ptr); 
void b2PushCallback(void *ptr);
void b3PushCallback(void *ptr);
void b4PushCallback(void *ptr);
void b5PushCallback(void *ptr);
void b6PushCallback(void *ptr);
void b7PushCallback(void *ptr);
void b8PushCallback(void *ptr);
void b9PushCallback(void *ptr);
void b10PushCallback(void *ptr);
void b11PushCallback(void *ptr);
void b12PushCallback(void *ptr);
void b13PushCallback(void *ptr);
void b14PushCallback(void *ptr);
void b15PushCallback(void *ptr);
void b16PushCallback(void *ptr);
void 23PushCallback(void *ptr);
void b24PushCallback(void *ptr);
void b29PushCallback(void *ptr);
void b30PushCallback(void *ptr);
void b37PushCallback(void *ptr);
void b38PushCallback(void *ptr);
void b42PushCallback(void *ptr);
void b43PushCallback(void *ptr);
void b50PushCallback(void *ptr);
void b51PushCallback(void *ptr);
void b60PushCallback(void *ptr);
void b61PushCallback(void *ptr);
void b66PushCallback(void *ptr);
void b73PushCallback(void *ptr);
void b67PushCallback(void *ptr);
void b68PushCallback(void *ptr);
void b69PushCallback(void *ptr);
void b70PushCallback(void *ptr);
void b71PushCallback(void *ptr);
void bt0PushCallback(void *ptr);
void bt0PopCallback(void *ptr);
void bt1PushCallback(void *ptr);
void bt1PopCallback(void *ptr);
void screen_management(void);
void recieveData(CONFIGURATION *config,byte *data, int dataLength);
void clear_buffer(byte *data,int nbytes);
#endif /*SCREEN_MANAGER_H*/