#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <Arduino.h>
#include <Wire.h>

struct MASTER_PARAMETER{;
uint8_t tx_DAT;
uint8_t tx_SIZE_DAT;
uint8_t rx_DAT;
uint8_t rx_SIZE_DAT;

};//extern struct MASTER_PARAMETER MASTER_parameter;

enum COMMANDS{
  IS_TUNNING=0,
  IS_STAND_BY,
  IS_PRESSURE_CONTROLLED,
  FIO2,
  IN_PRESURE,
  TIDAL_VOLUME,
  BREATHING_RATE,
  IE_RATIO,
  MAXIMUM_IN_PRESSURE,
  MAXIMUN_VOLUME_TIDAL,
  MINIMUN_IN_PRESSURE,
  MINIMUN_OUT_PRESSURE,
  MINIMUN_TIDAL_VOLUME,
  
  
};



typedef union
  {
      struct CONFIGURATION
    {
      //reference values
      uint8_t FiO2; // 21 - (100) [%]
      uint16_t in_presure; // 0 ~(65*1000) [cmH2O]
      uint16_t tidal_volume; // 0 ~(6500*10) [mL]
      uint16_t breathing_rate; //6*1000 ~ 40*1000 [breaths/min]
      uint16_t ie_ratio; //1:(1*1000) ~ 1:(3*1000) 
      
      //warnings
      uint16_t maximun_in_pressure; // 0 ~(65*1000) [cmH2O]
      uint16_t maximun_out_pressure;  // 0 ~(65*1000) [cmH2O]
      uint16_t maximun_tidal_volume;   // 0 ~(6500*10) [mL]
      uint16_t minimun_in_pressure; // 0 ~(65*1000) [cmH2O]
      uint16_t minimun_out_pressure;  // 0 ~(65*1000) [cmH2O]
      uint16_t minimun_tidal_volume;   // 0 ~(6500*10) [mL]
      
      //working mode options
      bool is_tunning; //calibration
      bool is_standby;  //no control
      bool is_pressure_controled; // control por volumen o por presion 
    
    }; uint16_t BUFFER[12];
  
}BreathingParameters;



uint8_t command_sended;     // in this variable is storage the command sended by the master
uint8_t is_command_sent;    // this is a flag, its 1 when a command is already been send, its 0 when a command is not sended yet


uint16_t working_mode;


#define MASTER_ADDRESS 1
#define SLAVE1_ADDRESS 2

#define SIZE_DATA 2

#define TUNNING 0x3;              //0011
#define STAND_BY 0x6;             //0110
#define PRESSURE_CONTROLLED 0x6C; //1100

void MASTER_init(void);
void MASTER_Write_Data(uint8_t SLAVE_ADDRESS,uint16_t DAT);
uint8_t MASTER_Recieve_Data(uint8_t SLAVE_ADDRESS,uint16_t SIZE_DAT);

#endif /*I2C_MASTER_H*/
