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




union CONF
  {
      struct 
    {
      //reference values
      uint8_t FiO2_1; // 21 - (100) [%]
    uint8_t FiO2_2; // 21 - (100) [%]
      uint8_t in_presure_1; // 0 ~(65*1000) [cmH2O]
    uint8_t in_presure_2; // 0 ~(65*1000) [cmH2O]
      uint8_t tidal_volume_1; // 0 ~(6500*10) [mL]
    uint8_t tidal_volume_2; // 0 ~(6500*10) [mL]
      uint8_t breathing_rate_1; //6*1000 ~ 40*1000 [breaths/min]
    uint8_t breathing_rate_2; //6*1000 ~ 40*1000 [breaths/min]
      uint8_t ie_ratio_1; //1:(1*1000) ~ 1:(3*1000) 
    uint8_t ie_ratio_2; //1:(1*1000) ~ 1:(3*1000) 
      
      //warnings
      uint8_t maximun_in_pressure_1; // 0 ~(65*1000) [cmH2O]
    uint8_t maximun_in_pressure_2; // 0 ~(65*1000) [cmH2O]
      uint8_t maximun_out_pressure_1;  // 0 ~(65*1000) [cmH2O]
    uint8_t maximun_out_pressure_2;  // 0 ~(65*1000) [cmH2O]
      uint8_t maximun_tidal_volume_1;   // 0 ~(6500*10) [mL]
    uint8_t maximun_tidal_volume_2;   // 0 ~(6500*10) [mL]
      uint8_t minimun_in_pressure_1; // 0 ~(65*1000) [cmH2O]
    uint8_t minimun_in_pressure_2; // 0 ~(65*1000) [cmH2O]
      uint8_t minimun_out_pressure_1;  // 0 ~(65*1000) [cmH2O]
    uint8_t minimun_out_pressure_2;  // 0 ~(65*1000) [cmH2O]
      uint8_t minimun_tidal_volume_1;   // 0 ~(6500*10) [mL]
    uint8_t minimun_tidal_volume_2;   // 0 ~(6500*10) [mL]
      
      //working mode options
      uint8_t is_tunning_1; //calibration
    uint8_t is_tunning_2; //calibration
      uint8_t is_standby_1;  //no control
    uint8_t is_standby_2;  //no control
      uint8_t is_pressure_controled_1; // control por volumen o por presion 
    uint8_t is_pressure_controled_2; // control por volumen o por presion 
    
  
  
    }; uint8_t  BUFFER[28];
  
};union CONF BreathingParameters;
/**********************************************************************/
enum COMMANDS{

  FIO2=0,
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

/**********************************************************************/



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
void function_send (uint8_t slave_address, uint8_t comando);
void function_request(uint8_t slave_address);
//void MASTER_Write_Data(uint8_t SLAVE_ADDRESS,uint16_t DAT);
//uint8_t MASTER_Recieve_Data(uint8_t SLAVE_ADDRESS,uint16_t SIZE_DAT);

#endif /*I2C_MASTER_H*/
