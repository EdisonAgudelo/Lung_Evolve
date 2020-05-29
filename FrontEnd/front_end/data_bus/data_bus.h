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

//DATA
typedef struct 
{
    byte peep;
    byte tv;
    byte bpm;
    byte ie;
    byte pressure;  

}DATA;


#endif /*DATA_BUS_H*/