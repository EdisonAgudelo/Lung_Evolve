
#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <stdint.h>

//general main states
typedef enum
{

  kMainInit,
  kMainIdle,
  kMainTunning,
  kMainBreathing,
  kMainDelay

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

//this struct is used by FMSDelaySet to create "virtual" FMS delays
typedef struct
{
  uint32_t ref_time;
  uint32_t delay_time;

  int next_state;
  int *actual_state;
  int delay_state;

} FMSDelay;

//this funtion is dedicated to state machine delays. It's highly recommended to use at the end
//of state
void FMSDelaySet(FMSDelay *object, uint32_t delay_time, int nex_state);

//this struct saves all errors of execution process
typedef union {
  struct
  {
    bool init_hardware : 1;
    bool init_driver : 1;
    bool main_state_fault : 1;
    bool breathing_state_fault : 1;
    bool working_configuration_not_initialized : 1;
  };
  uint32_t all;
} ErrorType;

//motor IDs
typedef enum
{
  kMotorIdBellows = 0x1,
  kMotorIdO2Choke,
  kMotorIdAirChoke
} MotorIDs;

const int kMotorMaxPos = 250; //steps or mm, TBD In this position, valve is full open
const int kMotorMinPos = 5;   //steps or mm, TBD. In this position, valve is closed

//valve IDs
typedef enum
{
  kValveIdManifold = 0x1,
  kValveIdInhalation,
  kValveIdExhalation
} ValveIDs;

//some valve constands definitions
const bool kValveFullOpen = 1;
const bool kValveFullClose = 0;

//sensor IDs
typedef enum
{
  kSensorIdAirFlow,
  kSensorIdInPressure,
  kSensorIdOutPressure,
} SensorID;

//This struct saves all Front end commands or information
typedef struct
{
  //working mode options
  bool is_tunning;            //calibration
  bool is_standby;            //no control
  bool is_pressure_controled; // control por volumen o por presion
  bool is_assisted;           //true: ventilator should wait for patient trigger or false: wait for time trigger

  //reference values
  uint8_t FiO2;            // 21 - (100) [%]
  uint16_t in_presure;     // 0 ~(65*1000) [cmH2O]
  uint16_t volume_tidal;   // 0 ~(6500*10) [mL]
  uint16_t breathing_rate; //6*1000 ~ 40*1000 [breaths/min]
  uint16_t ie_ratio;       //1:(1*1000) ~ 1:(3*1000)

  //warnings
  uint16_t maximun_in_pressure;  // 0 ~(65*1000) [cmH2O]
  uint16_t maximun_out_pressure; // 0 ~(65*1000) [cmH2O]
  uint16_t maximun_volume_tidal; // 0 ~(6500*10) [mL]
  uint16_t manimun_in_pressure;  // 0 ~(65*1000) [cmH2O]
  uint16_t manimun_out_pressure; // 0 ~(65*1000) [cmH2O]
  uint16_t manimun_volume_tidal; // 0 ~(6500*10) [mL]

} BreathingParameters;

//This struct saves all working parameters, most of them are on execution calculated
typedef struct
{
  //velocities
  uint32_t motor_return_vel_bellows;            //steps/min or mm/s TDB
  uint32_t motor_foward_const_flow_vel_bellows; //used when flow control is selected

  //control ref
  uint16_t sensor_air_flow_ref; //control reference for flow controlled mode
  uint16_t sensor_pressure_ref; //control reference for pressure controlled mode

  //valve position
  uint32_t motor_position_o2_choke;  //valve postion to control O2 flow through pneumatic system
  uint32_t motor_position_air_choke; //valve postion to control O2 flow through pneumatic system

  //times
  uint32_t breathing_in_puase_time;
  uint32_t breathing_in_time;
  uint32_t breathing_out_puase_time;
  uint32_t breathing_out_time;

  uint32_t motor_open_time_o2_choke;  //this time denotes the needed time  to reach FiO2%
  uint32_t motor_open_time_air_choke; //this time denotes the needed time  to reach FiO2%

  //triggers
  uint32_t sensor_pressure_trigger_ins_value; //maximun pressure value to trigger an inspiration cicle
  uint32_t sensor_pressure_trigger_esp_value; //maximun pressure value to trigger an espiration cicle
  uint32_t sensor_flow_trigger_ins_value;     //minimun flow value to trigger an inspiration cicle
  uint32_t sensor_flow_trigger_esp_value;     //minimun flow value to trigger an espiration cicle

} BreathingDinamics;

const uint32_t kMotorDefaultReturnVelBellows = 100; //steps/min or mm/s TBD
const uint32_t kMotorDefaultVelAirChoke = 100;      //steps/min or mm/s TBD
const uint32_t kMotorDefaultVelO2Choke = 100;       //steps/min or mm/s TBD

#endif