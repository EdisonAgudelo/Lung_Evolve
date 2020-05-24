#ifndef DATA_BUS_H
#define DATA_BUS_H

//ALARMS
struct ALARMS
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

}alarms_state_struct;
//extern struct ALARMS alarms_state_struct;

//CONFIGURATION
struct CONFIGURATION
{
    uint32_t peep;
    uint32_t tv;
    uint32_t fio2;
    uint32_t rr;
    uint32_t ie;

}configuration_struct;
//extern struct CONFIGURATION configuration_struct;

//DATA
struct DATA
{
    uint32_t peep;
    uint32_t tv;
    uint32_t rr;
    uint32_t ie;
    uint32_t preassure;  

}data_struct;
//extern struct DATA data_struct;

#endif /*DATA_BUS_H*/