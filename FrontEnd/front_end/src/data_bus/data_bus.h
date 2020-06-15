#ifndef DATA_BUS_H
#define DATA_BUS_H
#include <stdint.h>


const uint8_t kTxBufferLength = 0xff;
///////////*************************////////////////////////
//ALARMS
typedef union {
  struct
  {

    bool apnea_alarm; 

    bool high_breathing_rate; 
    bool low_breathing_rate ; 

    bool high_in_volume_tidal ; 
    bool low_in_volume_tidal ; 
    bool near_low_in_volume_tidal; 

    bool high_ie_ratio ; 
    bool low_ie_ratio; 

    bool low_peep; 

    bool no_main_supply; 

    bool high_out_pressure;
    bool low_out_pressure ;
    
    bool high_in_pressure ; 
    bool low_in_pressure;  

    bool high_volume_leakage; 

    bool detached_proximal_tube; 
    bool detached_oxygen_tube; 

    bool low_battery;
    bool no_battery; 

    bool high_temp_bat; 
    bool high_temp_motor; 
  };
  
  uint32_t all;
  bool bits[21];
} ALARMS;
extern ALARMS alarms_struct;
extern ALARMS mask_alarms_struct;

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
  0xa,
  //bool low_in_volume_tidal : 1;
  0xb,
  //bool near_low_in_volume_tidal:1;
  0xc,
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
  0x21
  //bool high_temp_bat:1;

};
///////////*************************////////////////////////

//CONFIGURATION
typedef union
{
  struct{
    uint32_t fio2;
    uint32_t bpm;
    uint32_t peep;
    uint32_t tpause; //tiempo?
    uint32_t apnea;
    uint8_t ie;
    uint32_t pressure;
	  uint32_t tidal;
	  uint32_t trigger;
    uint8_t controlType;//assistive(0x1) or controlled(0x0)
    uint8_t control; //volume(true) or pressure(false)
    uint8_t off; //ventilator is off(0x1) or not(0x0)
    uint8_t pause; //ventilation operation is paused(0x1) or not (0x0)
    uint32_t maxInPressure;
    uint32_t minInPressure;
    uint32_t maxOutPressure;
    uint32_t minOutPressure;
    uint32_t maxTV;
    uint32_t minTV;
	  uint32_t max_leakage;

  };
  uint8_t all[65];

}CONFIGURATION;
extern CONFIGURATION config;
//configuration id define
const uint8_t ConfigId[] = {
  0x0,
  0x1,
  0x2,
  0x3,
  0x4,
  0x5,
  0x6,
  0x7,
  0x8,
  0x9,
  0x10,
  0x11,
  0x12,
  0x13,
  0x14,
  0x15,
  0x16,
  0x17,
  0x18,
  0x19
 
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
*/