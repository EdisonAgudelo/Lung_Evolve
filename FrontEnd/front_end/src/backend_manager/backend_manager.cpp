//protocolo backend
//funcion para actualizar el bus de datos
//recibir datos de backend


//serial
#include "backend_manager.h"
//#include <Arduino.h>
#include <stdlib.h>
//#include <string.h>

STATE_backend state_backend;
bool status_send,status_recieve;

uint8_t *Buffer=(uint8_t*)calloc(1,sizeof(uint8_t));  //buffer to recieve data from backend serial port, with length 1 byte
uint8_t *Buffer2=(uint8_t*)calloc(1,sizeof(uint8_t));  //buffer to check crc_8, with length 1 byte
uint8_t *Buffersend=(uint8_t*)calloc(38,sizeof(uint8_t));  //buffer to check crc_8, with length 1 byte
int nbytes;

void serial_backend_init(void)
{
    nbytes=1;
    Serial.begin(9600);
    
}


bool recieve(void)
{
    bool done;

  if(Serial.available())
  {
    Buffer[nbytes-1]=Serial.read();
    if(Buffer[nbytes-1]==0xff)//stop byte recieved
    {
      check_data(Buffer,nbytes);
      nbytes=1;
      Buffer = (uint8_t*)realloc(Buffer,nbytes*sizeof(uint8_t));
      Buffer[0]=0;
      done =true; //if recieving process is done
    }else //data recieved
    {
      nbytes++; //increment number of bytes of the buffer
      Buffer = (uint8_t*)realloc(Buffer,nbytes*sizeof(uint8_t));
      done=false; //if recieving process is not done
    }
    
  }
  else
  {
      done=false;//if there is no data available
  }
  
  return done;
}

void check_data(uint8_t *Buffer, int nbytes)
{   
    int payload_length,length_buffer2;
    bool status;
    uint8_t crc8;
    payload_length=Buffer[1];
    length_buffer2=2+payload_length;
    Buffer2 = (uint8_t*)realloc(Buffer2,length_buffer2*sizeof(uint8_t));
    CRC8Configure(0x31, 0x0);
    for(int i=0;i<length_buffer2;i++)
    {
        Buffer2[i]=Buffer[i];
    }
    crc8=CRC8Calculate(Buffer2, length_buffer2);
   if(crc8==Buffer[2+payload_length]) //2(->command + payload_length) + payload
    {
        status_recieve=true;
        get_data(Buffer2,payload_length);
        //sendACK(); //confirm data recieved correctly
    }
    else
    {
        status_recieve=false;
        //sendNACK();//send no ACK data recieved incorrectly
    }
    
}

void get_data(uint8_t *Buffer, int nbytes)
{
  uint8_t command;
  command=Buffer[0];
  switch(command)
  {
      case alarms:
        for(int i=2;i<nbytes;i+2)
        {
            if(Buffer[i]==kApneaAlarm)
            {
                alarms_struct.ApneaAlarm=Buffer[i+1];
            }
            if(Buffer[i]==kHighBreathRate)
            {
                alarms_struct.HighBreathRate=Buffer[i+1];
            }
            if(Buffer[i]==kLowInspP)
            {
                alarms_struct.LowInspP=Buffer[i+1];
            }
            if(Buffer[i]==kHighInspP)
            {
                alarms_struct.HighInspP=Buffer[i+1];
            }
            if(Buffer[i]==kHighVte)
            {
                alarms_struct.HighVte=Buffer[i+1];
            }
            if(Buffer[i]==kLowVte)
            {
                alarms_struct.LowVte=Buffer[i+1];
            }
            if(Buffer[i]==kNearToLowVte)
            {
                alarms_struct.NearToLowVte=Buffer[i+1];
            }
            if(Buffer[i]==kHighVti)
            {
                alarms_struct.HighVti=Buffer[i+1];
            }
            if(Buffer[i]==kProximalTube)
            {
                alarms_struct.ProximalTube=Buffer[i+1];
            }
            if(Buffer[i]==kVteNotAchived)
            {
                alarms_struct.VteNotAchived=Buffer[i+1];
            }
            if(Buffer[i]==kVteOv)
            {
                alarms_struct.VteOv=Buffer[i+1];
            }
            if(Buffer[i]==kPatientLeaks)
            {
                alarms_struct.PatientLeaks=Buffer[i+1];
            }
            if(Buffer[i]==kShutDown)
            {
                alarms_struct.ShutDown=Buffer[i+1];
            }
            if(Buffer[i]==kBackUpOn)
            {
                alarms_struct.BackUpOn=Buffer[i+1];
            }
            if(Buffer[i]==kLowBattery)
            {
                alarms_struct.LowBattery=Buffer[i+1];
            }
            if(Buffer[i]==kNoBattery)
            {
                alarms_struct.NoBattery=Buffer[i+1];
            }
            if(Buffer[i]==kHighTemp)
            {
                alarms_struct.HighTemp=Buffer[i+1];
            }
            if(Buffer[i]==kUnderPeep)
            {
                alarms_struct.UnderPeep=Buffer[i+1];
            }
            if(Buffer[i]==kNoOxygen)
            {
                alarms_struct.NoOxygen=Buffer[i+1];
            }
        }
      break;
      case datas:
        for(int i=2;i<nbytes;i+2)
        {
            if(Buffer[i]==kppeep)
            {
                dataValue.peep=Buffer[i+1];
            }
            if(Buffer[i]==kttv)
            {
                dataValue.tv=Buffer[i+1];
            }
            if(Buffer[i]==kbbpm)
            {
                dataValue.bpm=Buffer[i+1];
            }
            if(Buffer[i]==kiie)
            {
                dataValue.ie=Buffer[i+1];
            }
            if(Buffer[i]==kppressure)
            {
                dataValue.pressure=Buffer[i+1];
            }
        }
      break;
      case sign:
        if(Buffer[2]==ACK)
        {
            status_send=true;
        }else if(Buffer[2]==NACK)
        {
            status_send=false;
        }
      break;
  }

}

void sendDATA(void)
{
    uint8_t crc_calc,length_buff_send=38;
    Buffersend = (uint8_t*)realloc(Buffersend,length_buff_send*sizeof(uint8_t));
    //uint8_t BufferSend[38];
    Buffersend[0]=0x2;
    Buffersend[1]=length_buff_send-2;
    Buffersend[2]=cfio2;
    Buffersend[3]=config.fio2;
    Buffersend[4]=cbpm;
    Buffersend[5]=config.bpm;
    Buffersend[6]=cpeep;
    Buffersend[7]=config.peep;
    Buffersend[8]=cheigh;
    Buffersend[9]=config.heigh;
    Buffersend[10]=capnea;
    Buffersend[11]=config.apnea;
    Buffersend[12]=cie;
    Buffersend[13]=config.ie;
    Buffersend[14]=cgender;
    Buffersend[15]=config.gender;
    Buffersend[16]=cpressure;
    Buffersend[17]=config.pressure;
    Buffersend[18]=ccontrolType;
    Buffersend[19]=config.controlType;
    Buffersend[20]=ccontrol;
    Buffersend[21]=config.control;
    Buffersend[22]=coff;
    Buffersend[23]=config.off;
    Buffersend[24]=cpause;
    Buffersend[25]=config.pause;
    Buffersend[26]=cmaxInPressure;
    Buffersend[27]=config.maxInPressure;
    Buffersend[28]=cminInPressure;
    Buffersend[29]=config.minInPressure;
    Buffersend[30]=cmaxOutPressure;
    Buffersend[31]=config.maxOutPressure;
    Buffersend[32]=cminOutPressure;
    Buffersend[33]=config.minOutPressure;
    Buffersend[34]=cmaxTV;
    Buffersend[35]=config.maxTV;
    Buffersend[36]=cminTV;
    Buffersend[37]=config.minTV;
    Buffersend[38]=cfio2;
    CRC8Configure(0x31, 0x0);
    crc_calc=CRC8Calculate(Buffersend, length_buff_send );
    Buffersend = (uint8_t*)realloc(Buffersend,(length_buff_send +1)*sizeof(uint8_t));
    Buffersend[39]=crc_calc;

    for(int i=0;i<length_buff_send+1;i++)
    {
        Serial.write(Buffersend[i]);
    }
}

void sendACK(void)
{
    uint8_t crc_calc,length_buff_send=3;
    Buffersend = (uint8_t*)realloc(Buffersend,(length_buff_send)*sizeof(uint8_t));
    Buffersend[0] = 0x3;
    Buffersend[1] = 0x1;
    Buffersend[2] = 0x1c;
    CRC8Configure(0x31, 0x0);
    crc_calc=CRC8Calculate(Buffersend, length_buff_send );
    Buffersend = (uint8_t*)realloc(Buffersend,(length_buff_send +1)*sizeof(uint8_t));
    Buffersend[3] = crc_calc;

    for(int i=0;i<length_buff_send +1;i++)
    {
        Serial.write(Buffersend[i]);
    }

}

void sendNACK(void)
{
    uint8_t crc_calc,length_buff_send=3;
    Buffersend = (uint8_t*)realloc(Buffersend,(length_buff_send)*sizeof(uint8_t));
    Buffersend[0] = 0x3;
    Buffersend[1] = 0x1;
    Buffersend[2] = 0x1d;
    CRC8Configure(0x31, 0x0);
    crc_calc=CRC8Calculate(Buffersend, length_buff_send );
    Buffersend = (uint8_t*)realloc(Buffersend,(length_buff_send +1)*sizeof(uint8_t));
    Buffersend[3] = crc_calc;

    for(int i=0;i<length_buff_send +1;i++)
    {
        Serial.write(Buffersend[i]);  
    }
  
}


void backend_management(void)
{
    bool done;
    switch(state_backend)
    {
        case krecieve:
        done=recieve(); 
        if(done==true)
        {
            state_backend=kcheckStatus;
        }else 
        {
            state_backend=kcheckScreenUpdate;
        }
        
        break;
        case kcheckStatus:
            if(status_recieve==true)
            {
                sendACK();
            }else
            {
                sendNACK();
            }
            if(status_send==true)
            {
                state_backend=kcheckScreenUpdate;
            }else
            {
                state_backend=ksendData;   
            }   
        break;
        case kcheckScreenUpdate:
        if(update)
        {
            state_backend=ksendData;
        }
        else
        {
            state_backend=krecieve;
        }
        
        break;
        case ksendData:
            sendDATA();
            update=false;
            state_backend=krecieve;
        break;
        default:
        state_backend=krecieve;
        break;
    
    }
    
    
    

}


