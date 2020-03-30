
#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <stdint.h>

typedef enum
{
  
  kMainInit,
  kMainIdle,
  kMainTunning,
  kMainBreathing
  
} MainStates;


//this struct saves all errors of execution process 
typedef union
{
  struct
  {
    bool init_hardware:1;
    bool init_driver:1;
  };
  uint32_t all;
}ErrorType;


//motor ID 
typedef enum
{
  kMotorIdBellows=0x1,
  kMotorIdO2Choke,
  kMotorIdAirChoke
}MotorIDs;

typedef enum
{
  kValveIdManifold = 0x1,
  kValveIdInhalation,
  kValveExhalation
}ValveIDs;


typedef struct 
{
    //working mode options
    bool is_tunning; //calibration
    bool is_standby;  //no control
    bool is_pressure_controled; // control por volumen o por presion 
    
    
    //reference values
    uint8_t FiO2; // 21 - (100) [%]
    uint16_t in_presure; // 0 ~(65*1000) [cmH2O]
    uint16_t volume_tidal; // 0 ~(6500*10) [mL]
    uint16_t breathing_rate; //6*1000 ~ 40*1000 [breaths/min]
    uint16_t ie_ratio; //1:(1*1000) ~ 1:(3*1000) 
    
    //warnings
    uint16_t maximun_in_pressure;    // 0 ~(65*1000) [cmH2O]
    uint16_t maximun_out_pressure;    // 0 ~(65*1000) [cmH2O]
    uint16_t maximun_volume_tidal;   // 0 ~(6500*10) [mL]
    uint16_t manimun_in_pressure;    // 0 ~(65*1000) [cmH2O]
    uint16_t manimun_out_pressure;    // 0 ~(65*1000) [cmH2O]
    uint16_t manimun_volume_tidal;   // 0 ~(6500*10) [mL]
    
} BreathingParameters;



#endif
