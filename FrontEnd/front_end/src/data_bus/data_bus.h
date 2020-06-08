#ifndef DATA_BUS_H
#define DATA_BUS_H
#include <stdint.h>
#define byte uint8_t

const uint8_t kTxBufferLength = 0x45;
///////////*************************////////////////////////
//ALARMS
typedef union {
  struct
  {

//mechanical ventilator alarms
    bool apnea_alarm:1;

    bool high_breathing_rate : 1;
    bool low_breathing_rate : 1;

    bool high_out_pressure : 1;
    bool low_out_pressure : 1;
    
    bool high_in_pressure : 1;
    bool low_in_pressure : 1;

    bool high_out_volume_tidal : 1;
    bool low_out_volume_tidal : 1;
    bool near_low_out_volume_tidal:1;

    bool high_in_volume_tidal : 1;
    bool low_in_volume_tidal : 1;
    bool near_low_in_volume_tidal:1;
    
    bool high_volume_leakage:1;

    bool high_ie_ratio : 1;
    bool low_ie_ratio : 1;

    bool low_peep:1;

// mechanical problems
    bool detached_proximal_tube:1;
    bool detached_oxygen_tube:1;

//electrical problems
    bool low_battery:1;
    bool no_battery:1;

    bool no_main_supply:1;

    bool high_temp_bat:1;
    bool high_temp_motor:1;

    bool system_shutdown:1;
  };
  
  uint8_t all[25];
} ALARMS;
extern ALARMS alarms_struct;

//alarms id define
const uint8_t AlarmId[] = {
  //bool apnea_alarm:1;
  0x0,
  //bool high_breathing_rate : 1;
  0x1,
  //bool low_breathing_rate : 1;
  0x2,
  //bool high_out_pressure : 1;
  0x3,
  //bool low_out_pressure : 1;
  0x4,
  // bool high_in_pressure : 1;
  0x5,
  // bool low_in_pressure : 1;
  0x6,
  //bool high_out_volume_tidal : 1;
  0x7,
  //bool low_out_volume_tidal : 1;
  0x8,
  //bool near_low_out_volume_tidal:1;
  0x9,
  //bool high_in_volume_tidal : 1;
  0x10,
  //bool low_in_volume_tidal : 1;
  0x11,
  //bool near_low_in_volume_tidal:1;
  0x12,
  //bool high_volume_leakage:1;
  0x13,
  //bool high_ie_ratio : 1;
  0x14,
  //bool low_ie_ratio : 1;
  0x15,
  //bool low_peep:1;
  0x16,
  //bool detached_proximal_tube:1;
  0x17,
  //bool detached_oxygen_tube:1;
  0x18,
  //bool low_battery:1;
  0x19,
  //bool no_battery:1;
  0x20,
  //bool no_main_supply:1;
  0x21,
  //bool high_temp_bat:1;
  0x22,
  //bool high_temp_motor:1;
  0x23,
  //bool ShutDown;
  0x24
};
///////////*************************////////////////////////

//CONFIGURATION
typedef union
{
  struct{
    byte fio2;
    byte bpm;
    byte peep;
    byte pausa;
    byte apnea;
    byte ie;
    byte pressure;
	uint16_t tidal;
	byte trigger;
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
	byte max_leakage;

  };
  uint8_t all[18];

}CONFIGURATION;
extern CONFIGURATION config;
//configuration id define
const uint8_t ConfigId[] = {
};

///////////*************************////////////////////////

//DATA
typedef union
{
  struct{
  uint32_t tidal;
  uint32_t ie_ratio;
  uint32_t breathing_rate;
  uint32_t in_pressure;
  uint32_t out_pressure;
  uint32_t mixture_flow; //% 
  uint32_t battery_level;
  };
  uint32_t all[7];
}DATA;
extern DATA dataValue;
//data id define
const uint8_t DataId[] = {
    //tidal
    0x0,
    //ie_ratio
    0x1,
    //breathing_rate
    0x2,
    //in_pressure
    0x3,
    //out_pressure
    0x4,
    //mixture_flow
    0x5,
    //battery_level
    0x6
};
///////////*************************////////////////////////


#endif /*DATA_BUS_H*/


/*



*/