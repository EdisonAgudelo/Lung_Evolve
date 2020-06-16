/*
    Lung Evolve Mechanical Ventilator
    Copyright (C) 2020  Edison Agudelo, Mateo Garcia, Alejandra Londoño

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see https://www.gnu.org/licenses/gpl-3.0.html.
    
    */


#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <stdint.h>

//general main states
typedef enum
{

  kMainInit= 0x0,
  kMainIdle,
  kMainTunning,
  kMainBreathing,
  kMainDelay

} MainStates;

//sub main or breathins states
typedef enum
{
  kBreathingOutPause = 0x0,
  kBreathingInPause,
  kBreathingOutCicle,
  kBreathingInCicle
} BreathingStates;

//submain init states 
typedef enum
{
  kInitStartFuelBellow,
  kInitWaitFuelBellow,
  kInitMoveChokeDefault,
  kInitWaitChokeDefault,
  kInitFinalStep
} InitStates;

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
    bool apnea : 1; //x

    bool high_breathing_rate : 1; //x
    bool low_breathing_rate : 1;  //x

    bool high_in_volume_tidal : 1;     //x
    bool low_in_volume_tidal : 1;      //x
    bool near_low_in_volume_tidal : 1; //x

    bool high_ie_ratio : 1; //x
    bool low_ie_ratio : 1;  //x

    bool low_peep : 1; //x

    bool no_main_supply : 1; //x

    bool high_out_pressure : 1; //x
    bool low_out_pressure : 1;  //x

    bool high_in_pressure : 1; //x
    bool low_in_pressure : 1;  //x

    bool high_volume_leakage : 1; //x

    bool detached_proximal_tube : 1; //x
    bool detached_oxygen_tube : 1;   //review

    bool low_battery : 1; //x
    bool no_battery : 1;  //x

    bool high_temp_bat : 1;   //x
    bool high_temp_motor : 1; //x
  };
  uint32_t all;
} WarningType;

//used to turn off flags when ventilator is not breathing
const uint32_t kWarningMaskBreathingFlags = 0x1fdff; //no include electrical flags

//to generate warnings
const float kWarningNearToLowFactor = 0.1;
const float kWarningMaximunBreathingRateFactor = 0.3;
const float kWarningMaximunIeRatioFactor = 0.1;

//motor IDs
typedef enum
{
  kMotorIdBellows = 0x0,
  kMotorIdO2Choke,
  kMotorIdAirChoke
} MotorIDs;

//review
const float kMotorBellowMaxPos = 280.0; // mm. In this position, bellow is full closed
const float kMotorBellowMinPos = 5.0;   // mm. In this position, bellow is full open
const float kMotorChokeMaxPos = 6.0;   // mm. In this position, choke valve is full open
const float kMotorChokeMinPos = 0.0;    // mm. In this position, choke valve is full closed

const float kMotorDefaultReturnVelBellows = 10.0; //mm/s //to go to home position
const float kMotorDefaultVelAirChoke = 0.5;       //mm/s //normal operation
const float kMotorDefaultVelO2Choke = 0.5;        //mm/s //normal operation

//valve IDs
typedef enum
{
  kValveIdManifold = 0x0,
  kValveIdInhalation,
  kValveIdExhalation
} ValveIDs;

//some valve constants definitions
const bool kValveFullOpen = true;
const bool kValveFullClose = false;

//sensor IDs
typedef enum
{
  kSensorIdAirFlowIn = 0x0,
  kSensorIdAirFlowOut,
  kSensorIdPressureIn,
  kSensorIdPressureOut,
  kSensorIdTempMotor,
  kSensorIdTempBattery,
  kSensorIdVoltageSource,
  kSensorIdVoltageBattery,
  kSensorIdAirFlowMixture
} SensorID;

//This struct saves all Front end commands or information
typedef union {
  struct
  {
    //working mode options
    bool is_tunning;          //calibration
    bool is_standby;          //no breathing
    bool is_volume_controled; //control by volumen or pressure
    bool is_assisted;         //true: ventilator should wait for patient trigger or false: wait for time trigger

    //reference values
    bool trigger_source_pressure; //pressure true or flow false

    uint32_t trigger_value;  //1 ~ 3 slm or -3 ~ -5 cmh20
    uint32_t FiO2;           // 21 - 100 [%]
    uint32_t in_presure;     // 0 ~ 65 [cmH2O]
    uint32_t volume_tidal;  // 200 ~ 650 [mL]
    uint32_t breathing_rate; //6 ~ 40 [breaths/min]
    uint8_t ie_ratio;       //1:1 ~ 1:3

    uint32_t apnea_time; // 0 ~ 30000 [ms]
    uint32_t pause_time; // 0 ~ 30000 [ms]

    //warnings
    uint32_t maximun_in_pressure;   // 0 ~ 65 [cmH2O]
    uint32_t minimun_in_pressure;   // 0 ~ 65 [cmH2O]
    uint32_t maximun_out_pressure;  // 0 ~ 65 [cmH2O]
    uint32_t minimun_out_pressure;  // 0 ~ 65 [cmH2O]
    uint32_t maximun_volume_tidal; // 0 ~ 650  [mL]
    uint32_t minimun_volume_tidal; // 0 ~ 650  [mL]
    uint32_t minimun_peep;          // 0 ~ 20 [cmH2O]
    uint32_t maximum_leakage;      //0-600 [mL]
  };

  uint8_t all[66];

} BreathingParameters;

typedef union {
  struct
  {
    bool manifold : 1;
    bool inhalation : 1;
    bool exhalation : 1;
  };
  uint8_t all;
} ValvePos;

//This struct saves all working parameters, most of them are on execution calculated
typedef struct
{
  //velocities
  float motor_return_vel_bellows;    //mm/s
  float motor_flow_bias_vel_bellows; //mm/s

  //control ref
  float sensor_air_flow_ref; //control reference for flow controlled mode
  float sensor_pressure_ref; //control reference for pressure controlled mode

  //valve position
  float motor_position_o2_choke;  //valve position to control O2 flow through pneumatic system
  float motor_position_air_choke; //valve position to control air flow through pneumatic system

  //timing
  uint32_t breathing_in_puase_time;
  uint32_t breathing_in_time;
  uint32_t breathing_out_puase_time;
  uint32_t breathing_out_time;

  //triggers
  float sensor_pressure_trigger_ins_value; //minimun pressure value to trigger an inspiration cicle
  float sensor_flow_trigger_ins_value;     //minimun flow value to trigger an inspiration cicle

  //this save valve position for every breathing state
  ValvePos valve_position[4];
} BreathingDinamics;


const float kMinimunFiO2 = 21.0;

//review
const float kConstantO2Choke = 1 / (100.0 - 21.0);  //to convert fio in choke position
const float kConstantAirChoke = 1 / (100.0 - 21.0); //to convert fio in choke position

const float kBellowArea = 232.35219; //cm^2
//to convert flow in bellow vel 
const float kBellowFactor = 10000.0 / (60.0 * kBellowArea); // [mm/s]/[slm]

//this struct saves all measured data
typedef union {
  struct
  {
    float in_pressure;    //cmH2O x
    float out_pressure;   //cmH2O x
    float patient_flow;   //slm x
    float patient_volume; //instantaneous value x

    float tidal;           //L x
    float breathing_rate;  //bpm x
    float ie_ratio;        //1:x x
    float battery_level;   //%
    float patient_leakage; //L x

    float mixture_flow;    //slm x
    float motor_temp;      // °C x
    float battery_temp;    // °C x
    float source_volatge;  // V x
    float battery_voltage; // V x
    uint32_t apnea_time;   //ms
  };

  //for frontend pruporse
  struct
  {
    uint8_t fast_data[4*4];
    uint8_t slow_data[5*4];
    float anonymous_data[6];
  };

  uint8_t all[15*4];

} MeasureType;

const float kBatMinVoltage = 10.5;   //V
const float kBatMaxVolatge = 13.4;   //V
const float kLowBatThreshold = 10.0; //%
const float kNoBatThreshold = 6.0;   //V

const float kBatDischargerConversion = 100.0 / (kBatMaxVolatge - kBatMinVoltage); // %/V

//////////////////////// front end comunication definitions ///////////////////

//temporal buffer used to build a receive all frames from other mcu
const uint8_t kTxBufferLength = 0xff;

const uint8_t kTxFastDataId[] = {
    // in_pressure    
    0x0,
    // out_pressure
    0x1,
    // patient_flow
    0x2,
    // patient_volume
    0x3,

};

const uint8_t kTxSlowDataId[] = {
    // tidal
    0x4,
    // breathing_rate
    0x5,
    // ie_ratio;
    0x6,
    // battery_level
    0x7,
    // patient_leakage
    0x8,
};

typedef struct
{
  uint32_t fast_data;
  uint32_t slow_data;
} DataTimeRef;

enum
{
  kFastDataPeriod = 40,  //ms each kTxDataPeriod ms send data to mcu
  kSlowDataPeriod = 5000 //ms each kTxDataPeriod ms send data to mcu
};

const uint8_t kTxAlarmId[] = {
  
    
    //    bool apnea_alarm; 
    0x0,
    //bool high_breathing_rate; 
    0x1,
    //bool low_breathing_rate ; 
    0x2,
    //bool high_in_volume_tidal ; 
    0x3,
    //bool low_in_volume_tidal ; 
    0x4,
    //bool near_low_in_volume_tidal; 
    0x5,
    //bool high_ie_ratio ; 
    0x6,
    //bool low_ie_ratio; 
    0x7,
    //bool low_peep; 
    0x8,
    //bool no_main_supply; 
    0x9,
    //bool high_out_pressure;
    0xA,
    //bool low_out_pressure ;
    0xB,
    //bool high_in_pressure ; 
    0xC,
    //bool low_in_pressure;  
    0xD,
    //bool high_volume_leakage; 
    0xE,
    //bool detached_proximal_tube; 
    0xF,
    //bool detached_oxygen_tube; 
    0x10,
    //bool low_battery;
    0x11,
    //bool no_battery; 
    0x12,
    //bool high_temp_bat; 
    0x13,
    //bool high_temp_motor; 
    0x14,
    
    };

#endif
