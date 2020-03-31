
#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <stdint.h>


//general main states
typedef enum
{
  
  kMainInit,
  kMainIdle,
  kMainTunning,
  kMainBreathing
  
} MainStates;

//sub main or breathins states
typedef enum
{
  kBreathingOutPause,
  kBreathingInPause,
  kBreathingOutCicle,
  kBreathingInCicle,
  kBreathingDelay,
} BreathingStates;

//this struct is used by FMSDelaySet to create "virtual" delays 
typedef struct 
{
  uint32_t ref_time;
  uint32_t delay_time;
  union
  {
    BreathingStates next_state;
  };
  union 
  {
    BreathingStates *actual_state;
  };
  union
  {
    BreathingStates delay_state;
  };

}FMSDelay;

//this funtion is dedicated to state machine delay before transition, is highly recommended to use at the end
//of state
void FMSDelaySet(FMSDelay *object, uint32_t delay_time, int nex_state);


//this struct saves all errors of execution process 
typedef union
{
  struct
  {
    bool init_hardware:1;
    bool init_driver:1;
    bool main_state_fault:1;
    bool breathing_state_fault:1;
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

const int kMotorMaxPos=250; //steps or mm, TBD In this position, valve is full open
const int kMotorMinPos=5; //steps or mm, TBD. In this position, valve is closed

//valves ID
typedef enum
{
  kValveIdManifold = 0x1,
  kValveIdInhalation,
  kValveIdExhalation
}ValveIDs;

//some valve constands definitions 
const bool kValveFullOpen = 1;  
const bool kValveFullClose = 0;  

typedef enum
{
  kSensorIdAirFlow,
  kSensorIdInPressure,
  kSensorOutInPressure,
}SensorID;

//comunication transfer
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


typedef struct
{
  //velocities 
  uint32_t motor_bellows_return_vel;  //steps/min or mm/s TDB
  uint32_t motor_bellows_foward_const_flow; //used when flow control is selected

  //valve position
  uint32_t motor_o2_choke_position;
  uint32_t motor_air_choke_position;

  //times 
  uint32_t breathing_in_puase_time;
  uint32_t breathing_in_time;
  uint32_t breathing_out_puase_time;
  uint32_t breathing_out_time;
  
  uint32_t motor_o2_choke_open_time; //this time denotes the needed time  to reach FiO2%
  uint32_t motor_air_choke_open_time //this time denotes the needed time  to reach FiO2%

} BreathingDinamics;


#endif
