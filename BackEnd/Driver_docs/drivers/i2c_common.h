

#ifndef _I2C_COMMON_H
#define _I2C_COMMON_H


#include <Wire.h>
#include <Arduino.h>
#include <stdint.h>

extern uint8_t command_sended; // in this variable is storage the command sended by the master

enum STATE_RECIEVE_T
{
  kGET_COMMAND = 0,
  kGET_DATA1,
  kGET_DATA2
};

union BreathingParametersT
{

  struct 
  {

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

    //working mode options
    bool is_tunning;            //calibration
    bool is_standby;            //no control
    bool is_pressure_controled; // control por volumen o por presion
    bool is_assisted;           //true: ventilator should wait for patient trigger or false: wait for time trigger
  };
  uint8_t BUFFER[22];

} ;

extern union BreathingParametersT BreathingParameters;
/**********************************************************************/
enum COMMANDS
{

  FIO2 = 0,
  IN_PRESURE,
  TIDAL_VOLUME,
  BREATHING_RATE,
  IE_RATIO,
  MAXIMUM_IN_PRESSURE,
  MAXIMUM_OUT_PRESSURE,
  MAXIMUN_VOLUME_TIDAL,
  MINIMUN_IN_PRESSURE,
  MINIMUN_OUT_PRESSURE,
  MINIMUN_TIDAL_VOLUME,
  IS_TUNNING,
  IS_STAND_BY,
  IS_PRESSURE_CONTROLLED

};



#endif