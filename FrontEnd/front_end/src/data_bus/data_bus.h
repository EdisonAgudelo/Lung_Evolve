#ifndef DATA_BUS_H
#define DATA_BUS_H
#include <stdint.h>
#define byte uint8_t
//ALARMS
typedef struct 
{

    bool ApneaAlarm;
    bool HighBreathRate;
    bool LowInspP;
    bool HighInspP;
    bool HighVte;
    bool LowVte;
    bool NearToLowVte;
    bool HighVti;
    bool ProximalTube;
    bool VteNotAchived;
    bool VteOv;
    bool PatientLeaks;
    bool ShutDown;
    bool BackUpOn;
    bool LowBattery;
    bool NoBattery;
    bool HighTemp;
    bool UnderPeep;
    bool NoOxygen;

} ALARMS;
extern ALARMS alarms_struct;

//alarms id define
 #define kApneaAlarm 0x1
 #define kHighBreathRate 0x2
 #define kLowInspP 0x3
 #define kHighInspP 0x4
 #define kHighVte 0x5
 #define kLowVte 0x6
 #define kNearToLowVte 0x7
 #define kHighVti 0x8
 #define kProximalTube 0x9
 #define kVteNotAchived 0xa
 #define kVteOv 0xb
 #define kPatientLeaks 0xc
 #define kShutDown 0xd
 #define kBackUpOn 0xe
 #define kLowBattery 0xf
 #define kNoBattery 0x10
 #define kHighTemp 0x11
 #define kUnderPeep 0x12
 #define kNoOxygen 0x13


//CONFIGURATION
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
    bool controlType;//assistive(true) or controlled(false)
    bool control; //volume(true) or pressure(false)
    bool off; //ventilator is off(true) or not
    bool pause; //ventilation operation is paused(true)
    byte maxInPressure;
    byte minInPressure;
    byte maxOutPressure;
    byte minOutPressure;
    byte maxTV;
    byte minTV;


}CONFIGURATION;
extern CONFIGURATION config;

//DATA
typedef struct 
{
    byte peep;
    byte tv;
    byte bpm;
    byte ie;
    byte pressure;  

}DATA;
extern DATA dataValue;

//data id define
#define kppeep 0x14
#define kttv 0x15
#define kbbpm 0x16
#define kiie 0x17
#define kppressure 0x18


//commands id define
#define alarms 0x19
#define datas 0x1a
#define sign 0x1b

//signaling id define
#define ACK 0x1c
#define NACK 0x1d

extern bool status_send,status_recieve;
#endif /*DATA_BUS_H*/