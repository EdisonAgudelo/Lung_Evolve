//protocolo backend
//funcion para actualizar el bus de datos
//recibir datos de backend


//serial
#include "backend_manager.h"

#include "../HICOP_protocol/hicop.h"
//#include <Arduino.h>
#include <stdlib.h>
//#include <string.h>





static bool breathing_config_change = false;
static bool warnings_change = false;
static bool data_change = false;




void backend_init(void)
{
    //uint8_t test[]={0x64,0x35,0x12};
    HicopInit();
    //HicopSendData(kHicopHeaderConfig, test, sizeof(test));
}


void backend_management(void)
{
  uint8_t transfer_buffer[kTxBufferLength];
  uint8_t transfer_pointer = 0;
  HicopHeaders rx_flag;
  uint8_t i;

    if (HicopReadData(&rx_flag, transfer_buffer, &transfer_pointer))
    {
        
        switch (rx_flag)
        {
            case kHicopHeaderConfig:
                //update parameters
               
            break;
            case kHicopHeaderData:
            //update parameters
                 while (transfer_pointer--)
                {
                    dataValue.all[transfer_pointer] = transfer_buffer[transfer_pointer];
                }
                data_change = true;
                //Serial.print("store");
            break;
            case kHicopHeaderAlarm:
            //update parameters
                
                i=0;
                /*
                for(i=0;i<transfer_pointer;i++)
                {
                    Serial.write(transfer_buffer[i]);
                }*/
                 while (i<transfer_pointer)
                {
                    alarms_struct.bits[transfer_buffer[i]] = transfer_buffer[i+1];
                    //Serial.write(transfer_buffer[i]);
                   // Serial.write(transfer_buffer[i+1]);
                    i+=2;

                }
                warnings_change = true;
                //mask_alarms_struct.all=alarms_struct.all;

               /*Serial.write(alarms_struct.apnea_alarm);
                Serial.write(alarms_struct.high_breathing_rate);
                Serial.write(alarms_struct.low_breathing_rate);
                Serial.write(alarms_struct.high_out_pressure);
                Serial.write(alarms_struct.low_out_pressure);
                Serial.write(alarms_struct.high_in_pressure);
                Serial.write(alarms_struct.low_in_pressure);
                Serial.write(alarms_struct.high_in_volume_tidal);
                Serial.write(alarms_struct.low_in_volume_tidal);
                Serial.write(alarms_struct.near_low_in_volume_tidal);
                Serial.write(alarms_struct.high_volume_leakage);
                Serial.write(alarms_struct.high_ie_ratio);
                Serial.write(alarms_struct.low_ie_ratio);
                Serial.write(alarms_struct.low_peep);
                Serial.write(alarms_struct.detached_proximal_tube);
                Serial.write(alarms_struct.detached_oxygen_tube);
                Serial.write(alarms_struct.low_battery);
                Serial.write(alarms_struct.no_battery);
                Serial.write(alarms_struct.no_main_supply);
                Serial.write(alarms_struct.high_temp_bat);
                Serial.write(alarms_struct.high_temp_motor);*/
                
            break;
            default:
                //do nothing
            break;
        }
    }
    HicopLoop();
    transfer_pointer = 0;

    if (update)
    {
        /*for(i=0;i< sizeof(config.all);i++)
        {
            //transfer_buffer[transfer_pointer++] = ConfigId[i];
            transfer_buffer[i] = (config.all[i]);
            //Serial.write(transfer_buffer[i]);
            //Serial.print(i);
        }*/
        //send data
        
        HicopSendData(kHicopHeaderConfig, config.all, sizeof(config.all));
        update=false;
        Serial.write(config.tunning);
        Serial.write(config.pause);
        Serial.write(config.control);
        Serial.write(config.controlType);
        Serial.write(config.triggerSource);
        Serial.write(config.trigger);
        Serial.write(config.fio2);
        Serial.write(config.pressure);
        Serial.write(config.tidal);
        Serial.write(config.bpm);
        Serial.write(config.ie);
        Serial.write(config.apnea);
        Serial.write(config.tpause);
        Serial.write(config.maxInPressure);
        Serial.write(config.minInPressure);
        Serial.write(config.maxOutPressure);
        Serial.write(config.minOutPressure);
        Serial.write(config.maxTV);
        Serial.write(config.minTV);
        Serial.write(config.peep);
        Serial.write(config.max_leakage);
        //Serial.print("ENVIO");
    }   
    else
    {
        //do nothing
    }

}





