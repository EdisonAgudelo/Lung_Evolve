
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

#include "backend_manager.h"
#define HICOP_MAX_RESPONSE_TIME 500
#include "../HICOP_protocol/hicop_config.h"
//#include <Arduino.h>
#include <stdlib.h>
//#include <string.h>



union
{
  float var;
  uint8_t all[4];
}temp;

static bool breathing_config_change = false;
static bool warnings_change = false;
bool data_change = false;




void backend_init(void)
{
    //uint8_t test[]={0x64,0x35,0x12};
    HicopInit();
    Serial.println("backend managment initialization done");
}


void backend_management(void)
{
  uint8_t transfer_buffer[kTxBufferLength];
  uint8_t transfer_pointer = 0;
  HicopHeaders rx_flag;
  uint8_t idtemp;
  uint8_t i;
if(!new_conf_screen)
{
    Serial.print("recieving");
    if (HicopReadData(&rx_flag, transfer_buffer, &transfer_pointer))
    {
        
        switch (rx_flag)
        {
            case kHicopHeaderConfig:
                //update parameters
               
            break;
            case kHicopHeaderData:
            //update parameters
            //Serial.println(transfer_pointer);
            i=0;
                 while (i<transfer_pointer)
                {
                    idtemp=transfer_buffer[i++];
                    temp.all[0]=transfer_buffer[i++];
                    temp.all[1]=transfer_buffer[i++];
                    temp.all[2]=transfer_buffer[i++];
                    temp.all[3]=transfer_buffer[i++];
                    dataValue.all[idtemp] = (uint32_t) temp.var;
                    //Serial.print(temp.var);
                }
                //Serial.print("\n");
               
                //Serial.print("done   ");


                data_change = true;
                //Serial3.write(dataValue.all, 32);//send data to the datalogger to be stored
                //Serial.println("backend data recieved");
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
                //Serial3.write(alarms_struct, 21);//send alarms to the datalogger to be stored
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
                //Serial.println("backend alarm recieved");
            break;
            default:
                //do nothing
            break;
        }
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
        if(config.pause)
        {
            Serial.print("pause sended");
        }
        update=false;
        //Serial.println("configuration sended to backend");
        /*Serial.write(config.tunning);
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
        //Serial.print("ENVIO");*/


        //Serial3.write(config.all, 66);//send configuration to the datalogger to be stored
    }   
    else
    {
        //do nothing
    }

}





