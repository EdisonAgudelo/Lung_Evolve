#ifndef DATA_BUS_H
#define DATA_BUS_H

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


//CONFIGURATION
typedef struct 
{
    uint32_t peep;
    uint32_t tv;
    uint32_t fio2;
    uint32_t rr;
    uint32_t ie;

}CONFIGURATION;

//DATA
typedef struct 
{
    uint32_t peep;
    uint32_t tv;
    uint32_t rr;
    uint32_t ie;
    uint32_t preassure;  

}DATA;


#endif /*DATA_BUS_H*/