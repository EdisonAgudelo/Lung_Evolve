
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

//this struct saves execution warning variables
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
  bool bits[32];
  uint32_t all;
} WarningType;



//motor IDs
typedef enum
{
  kMotorIdBellows = 0x0,
  kMotorIdO2Choke,
  kMotorIdAirChoke
} MotorIDs;

const float kMotorMaxPos = 280.0; // mm. In this position, valve is full open
const float kMotorMinPos = 5.0;   // mm. In this position, valve is closed

//valve IDs
typedef enum
{
  kValveIdManifold = 0x0,
  kValveIdInhalation,
  kValveIdExhalation
} ValveIDs;

//some valve constants definitions
const bool kValveFullOpen = 1;
const bool kValveFullClose = 0;

//sensor IDs
typedef enum
{
  kSensorIdAirFlowIn,
  SensorIdAirFlowOut,
  kSensorIdPressureIn,
  kSensorIdPressureOut,
  kSensorIdTempMotor,
  kSensorIdTempBattery,
  kSensorIdVoltageSource,
  kSensorIdVoltageBattery
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
  uint16_t volume_tidal;   // 0 ~(65*1000) [L/min]
  uint16_t breathing_rate; //6*1000 ~ 40*1000 [breaths/min]
  uint16_t ie_ratio;       //1:(1*1000) ~ 1:(3*1000)

  //warnings
  uint16_t maximun_in_pressure;  // 0 ~(65*1000) [cmH2O]
  uint16_t maximun_out_pressure; // 0 ~(65*1000) [cmH2O]
  uint16_t maximun_volume_tidal; // 0 ~(6500*10) [mL]
  uint16_t minimun_in_pressure;  // 0 ~(65*1000) [cmH2O]
  uint16_t minimun_out_pressure; // 0 ~(65*1000) [cmH2O]
  uint16_t minimun_volume_tidal; // 0 ~(6500*10) [mL]

} BreathingParameters;

const float kMaximun_deviation_breathing_rate = 0.05; //[]
const float kMaximun_deviation_ie_ratio = 0.05;       //[]

//This struct saves all working parameters, most of them are on execution calculated
typedef struct
{
  //velocities
  uint32_t motor_return_vel_bellows;            //mm/s
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

const float kMotorDefaultReturnVelBellows = 100.0; //mm/s 
const float kMotorDefaultVelAirChoke = 100.0;      //mm/s 
const float kMotorDefaultVelO2Choke = 100.0;       //mm/s

typedef struct
{
  uint32_t tidal;
  uint32_t ie_ratio;
  uint32_t breathing_rate;
  uint32_t in_pressure;
  uint32_t out_pressure;
  uint32_t mixture_flow; //% 
  uint32_t battery_level;
}MeasureType;



//////////////////////// HICOP definitions ///////////////////

const uint8_t kHicopDataId[]={
  //uint32_t tidal;
  0x0,
  //uint32_t ie_ratio;
  0x1,
  //uint32_t breathing_rate;
  0x2,
  //uint32_t in_pressure;
  0x3,
  //uint32_t out_pressure;
  0x4,
  //uint32_t mixture_flow;
  0x5,
  //battery_level
  0x6,
};

const uint8_t kHicopAlarmId[] = {
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



#endif
