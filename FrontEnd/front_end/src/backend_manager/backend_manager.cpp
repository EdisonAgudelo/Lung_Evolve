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
  uint16_t i;

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
                Serial.print("store");
            break;
            case kHicopHeaderAlarm:
            //update parameters
                 while (transfer_pointer--)
                {
                    alarms_struct.all[transfer_pointer] = transfer_buffer[transfer_pointer];
                }
                warnings_change = true;
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
        for(i=0;i< sizeof(config.all);i++)
        {
            //transfer_buffer[transfer_pointer++] = ConfigId[i];
            transfer_buffer[transfer_pointer++] = (config.all[i]);
        }
        //send data
        HicopSendData(kHicopHeaderConfig, transfer_buffer, transfer_pointer);
        update=false;
    }   
    else
    {
        //do nothing
    }

}





